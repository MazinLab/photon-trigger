#include "photon.hpp"

/*
 *
 * The postage stamping plan calls for monitoring up to 8 resonators for their holdoff so
 * N_MONITOR*1MHz*4 or in bursts of (N_CAPDATA+N_CAPPRE)*4 bytes or 480 byte bursts at 32MiB/s
 *
 * The trigger core is at 512 as is the IQ stream so we need to first pull out the relevant bits
 * and then run them through a stream interconnect (8 slave 1 master w/cc)
 * Then we can grab and write them at 256 to PSRAM
 *
 *
 *
 * Postage stamp data comes in sequentially and easily. Photons arrive randomly but clustered so need to be sorted
 * worst case scenario after a full N_CAPDATA+N_CAPPRE arrives on one stream it will be 512 clocks before the next
 * sample. 1/(5000cps*1e-6MHz) means 200 samples between photons/triggers with an N_CAPDATA+N_CAPPRE of 120
 * (~8300cps) that leaves 80*512 ~40k 512MHz tics before the next sample we need to capture would arrive.
 * Bursting out the 480 bytes at 256 MHz/16B needs only ~60+overhead 512MHz tics.
 * So even if we serially read from the postage stamp channels we are looking at
 * N_MONITOR*((N_CAPDATA+N_CAPPRE)+(N_CAPDATA+N_CAPPRE)*4/16) or about 1200 256MHz clocks or 2400 512 or about
 * 6% of our time before we need need to deal with a photon.
 *
 * So deal with them serially and have a buffer long enough for 1 full postage for each channel (~3.8KiB)!
 *
 * +4 samples
 *
 *
 * Nick has encouraged support for every trigger up to 10k counts per second, At 10k we have a trigger every 100us
 * samples arrive every us. with a window of 100 post trigger we can just attain this. getting the data out
 * takes 1<us so that isn't the limit
 *
 *
 */

inline void channel_to_grouplane(reschan_t x, group_t &group, lane_t &lane){
	lane=x.range(N_PHASE_LOG2-1,0);
	group=x>>N_PHASE_LOG2;
}


void postage_filter(hls::stream<trigstream_t> &postage_stream,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]) {

#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=postage_stream  depth=74752 register
#pragma HLS INTERFACE mode=axis register_mode=off depth=200 port=iq_out
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
#pragma HLS INTERFACE mode=s_axilite port=monitor bundle=control
#pragma HLS ARRAY_PARTITION variable=monitor type=complete

	 static ap_shift_reg<unsigned int, N_CAPPRE+1> iqprereg[N_MONITOR];
	 static unsigned char tocapture[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete


//	while(!instream.empty()) {
	#pragma HLS PIPELINE II=1
		trigstream_t in;
		group_t group;
		ap_uint<N_PHASE> trigger;
		group512_t align;

		in=postage_stream.read();
		group=in.user.range(N_PHASEGROUPS_LOG2-1,0);
		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);

		iq_t x[N_PHASE];
		#pragma HLS ARRAY_PARTITION variable=x type=complete
		for (int i=0;i<N_PHASE;i++)
			x[i]=in.data.range(IQ_BITS*(i+1)-1+N_PHASE*PHASE_BITS, i*IQ_BITS+N_PHASE*PHASE_BITS);


		singleiqstream_t tmp_out[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tmp_out type=complete
		bool tmp_out_write[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tmp_out_write type=complete

		for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL
			lane_t lane;
			group_t gid;
			iq_t laneiq;
			unsigned char nexttocap;
			channel_to_grouplane(monitor[i], gid, lane);

			laneiq = iqprereg[i].shift(x[lane], N_CAPPRE, gid==group);

			nexttocap = tocapture[i]-1;

			singleiqstream_t tmp;
			tmp.user=i;
			tmp.last=tocapture[i]==1;
			tmp.data=laneiq;

			tmp_out_write[i]=false;
			if (gid==group) {
				if (tocapture[i]>0) {
//					iq_out[i].write(tmp);
					tmp_out[i]=tmp;
					tmp_out_write[i]=true;
					tocapture[i] = nexttocap;
				}
				else if (trigger[lane]) {
					tocapture[i]=N_CAPDATA-1;
				}
			}
		}

		for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL
			if (tmp_out_write[i]) iq_out[i].write(tmp_out[i]);
		}
//	}
}



void postage_interconnect(hls::stream<singleiqstream_t> postage_iq[N_MONITOR], hls::stream<singleiqstream_t> &out, bool &overflow){
#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=out  depth=74752 register
#pragma HLS INTERFACE mode=axis register_mode=off depth=200 port=postage_iq
#pragma HLS ARRAY_PARTITION variable=postage_iq type=complete
#pragma HLS PIPELINE II=1

	 static hls::stream<singleiqstreaminternal_t, N_CAPDATA+4> buff[N_MONITOR];
#pragma HLS AGGREGATE compact=bit variable=buff
#pragma HLS ARRAY_PARTITION type=complete variable=buff
	 static unsigned char sending_i=0, next_sending;
	 static bool packet_in_flight=false;
	 unsigned char sending_plus1;


	 bool _overflow;

	 for (int i=0;i<N_MONITOR;i++) {
#pragma HLS UNROLL
		 singleiqstream_t x;

		 if (postage_iq[i].read_nb(x)) {
			 singleiqstreaminternal_t foo;
#pragma HLS AGGREGATE compact=bit variable=x
			 foo.last=x.last;
			 foo.data=x.data;
			 foo.user=x.user;
			 _overflow|=!buff[i].write_nb(foo);
		 }
	 }

	 overflow=_overflow;

	 singleiqstreaminternal_t foo;

	 sending_plus1 = sending_i>=N_MONITOR ? 0:sending_i+1;

	 if (buff[sending_i].read_nb(foo)) {
		 singleiqstream_t x;
		 x.last=foo.last;
		 x.data=foo.data;
		 x.user=foo.user;
		 out.write(x);
		 if (foo.last) sending_i = sending_plus1;
		 else packet_in_flight=true;
	 } else {
		 if (!packet_in_flight) sending_i = sending_plus1;
	 }

}



void postage_filter_w_interconn(hls::stream<trigstream_t> &postage_stream,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> &out, bool &overflow) {
#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=postage_stream  depth=74752 register
#pragma HLS INTERFACE mode=axis depth=74752 port=out register

#pragma HLS INTERFACE mode=s_axilite port=monitor bundle=control
#pragma HLS ARRAY_PARTITION variable=monitor type=complete

#pragma HLS PIPELINE II=1

	 static ap_shift_reg<unsigned int, N_CAPPRE+1> iqprereg[N_MONITOR];
	 static unsigned char tocapture[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete


//	while(!instream.empty()) {
//	#pragma HLS PIPELINE II=1
		trigstream_t in;
		group_t group;
		ap_uint<N_PHASE> trigger;
		group512_t align;

		iq_t x[N_PHASE];
		#pragma HLS ARRAY_PARTITION variable=x type=complete

		in=postage_stream.read();
		group=in.user.range(N_PHASEGROUPS_LOG2-1,0);
		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);


		for (int i=0;i<N_PHASE;i++)
			x[i]=in.data.range(IQ_BITS*(i+1)-1+N_PHASE*PHASE_BITS, i*IQ_BITS+N_PHASE*PHASE_BITS);



		static hls::stream<singleiqstreaminternal_t, N_CAPDATA+4> buff[N_MONITOR];
#pragma HLS AGGREGATE compact=bit variable=buff
#pragma HLS ARRAY_PARTITION type=complete variable=buff
		 bool _overflow;
		 static unsigned char sending_i=0, next_sending;
		 static bool packet_in_flight=false;
		 unsigned char sending_plus1;



		for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL
			lane_t lane;
			group_t gid;
			iq_t laneiq;
			unsigned char nexttocap;
			channel_to_grouplane(monitor[i], gid, lane);

			laneiq = iqprereg[i].shift(x[lane], N_CAPPRE, gid==group);

			nexttocap = tocapture[i]-1;

			//singleiqstream_t tmp;
			singleiqstreaminternal_t tmp;
			tmp.user=i;
			tmp.last=tocapture[i]==1;
			tmp.data=laneiq;

			if (gid==group) {
				if (tocapture[i]>0) {
					_overflow|=!buff[i].write_nb(tmp);
					tocapture[i] = nexttocap;
				}
				else if (trigger[lane]) {
					tocapture[i]=N_CAPDATA-1;
				}
			}
		}



		 overflow=_overflow;

		 singleiqstreaminternal_t foo;

		 sending_plus1 = sending_i>=N_MONITOR ? 0:sending_i+1;

		 if (buff[sending_i].read_nb(foo)) {
			 singleiqstream_t x;
			 x.last=foo.last;
			 x.data=foo.data;
			 x.user=foo.user;
			 out.write(x);
			 if (foo.last) sending_i = sending_plus1;
			 else packet_in_flight=true;
		 } else {
			 if (!packet_in_flight) sending_i = sending_plus1;
		 }

}




void postage_maxi(hls::stream<singleiqstream_t> &postage, iq_4x_t iq[POSTAGE_BUFSIZE][N_CAPDATA_MAXI],
				  uint16_t &event_count, uint16_t max_events){
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=postage register
#pragma HLS INTERFACE mode=s_axilite port=event_count
#pragma HLS INTERFACE mode=s_axilite port=max_events
#pragma HLS INTERFACE mode=m_axi max_widen_bitwidth=128 port=iq offset=slave max_write_burst_length=256


	iq_4x_t buf[N_CAPDATA/4];
	uint16_t _max_events, _count;
	_count=0;
	_max_events = max_events;

	assert(_max_events<=POSTAGE_BUFSIZE);
	assert(_max_events>0);

const int _maxtripcount=POSTAGE_BUFSIZE;
	capture: while(_count<_max_events) {
#pragma HLS LOOP_TRIPCOUNT max=_maxtripcount min=1


		singleiqstream_t tmp;
		iq_4x_t x;
		uint8_t _chan;
		uint16_t _event;
		bool desync;

		desync=false;

		tmp = postage.read();
		_chan = tmp.user;

		desync|=tmp.last;
		x.range(IQ_BITS-1,0)=_chan;
		x.range(IQ_BITS*2-1,IQ_BITS)=tmp.data;

		tmp=postage.read();
		desync|=tmp.last;
		x.range(IQ_BITS*3-1,IQ_BITS*2)=tmp.data;

		tmp=postage.read();
		desync|=tmp.last;
		x.range(IQ_BITS*4-1,IQ_BITS*3)=tmp.data;

		buf[0]=x;

		read: for (int i=1; i<N_CAPDATA/4;i++) {
#pragma HLS PIPELINE II=4 rewind
			iq_4x_t x;
			singleiqstream_t tmp;
			desync=false;
			tmp=postage.read();
			desync|=tmp.last;
			x.range(IQ_BITS-1,0)=tmp.data;

			tmp=postage.read();
			desync|=tmp.last;
			x.range(IQ_BITS*2-1,IQ_BITS)=tmp.data;

			tmp=postage.read();
			desync|=tmp.last;
			x.range(IQ_BITS*3-1,IQ_BITS*2)=tmp.data;

			tmp=postage.read();
			desync|=(tmp.last && i!=N_CAPDATA/4-1);
			x.range(IQ_BITS*4-1,IQ_BITS*3)=tmp.data;

			buf[i]=x;
		}

		//probably should throw away the first photon to deal with stale data
		burst: for (int i=0; i<N_CAPDATA_MAXI;i++) {
			iq[_count][i]=buf[i];
		}

		if (!desync) _count++;
		else while (!postage.read().last); //resync

		event_count=_count;
	}

}


