#include "photon.hpp"


inline void unpack_thresholds(threshoffs_t toffs, threshold_t threshs[N_PHASE], interval_t holdsoffs[N_PHASE]) {

	unpack: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		threshoff_t to = toffs.range(THRESHOFF_BITS*(i+1)-1, THRESHOFF_BITS*i);
		threshs[i]=to&0xffff;
		holdsoffs[i]=to>>THRESHOLD_BITS;
	}
}

void trigger(hls::stream<phasestream_t> &phase4x_in, hls::stream<iqstream_t> &iq8x_in, threshoffs_t threshoffs[N_PHASEGROUPS],
		hls::stream<timestamp_t> &timestamp,
		hls::stream<trigstream_t> &postage_stream, bool &desync, hls::stream<photon_t> photons_lane[N_PHASE]) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=postage_stream depth=32000 register
#pragma HLS INTERFACE mode=axis port=phase4x_in depth=32000 register
#pragma HLS INTERFACE mode=axis port=iq8x_in depth=16000 register
#pragma HLS INTERFACE mode=axis port=timestamp depth=32000 register
#pragma HLS INTERFACE mode=axis depth=4 port=photons_lane register
#pragma HLS INTERFACE mode=s_axilite port=threshoffs bundle=control
#pragma HLS INTERFACE mode=s_axilite port=desync bundle=control

	static sincegroup_t since_data[N_PHASEGROUPS], since_cache;
	static photongroupival_t photon_data[N_PHASEGROUPS], photon_cache;
#pragma HLS DEPENDENCE variable=since_data intra WAR false
#pragma HLS DEPENDENCE variable=since_data intra RAW false
#pragma HLS DEPENDENCE variable=since_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=since_data

#pragma HLS DEPENDENCE variable=photon_data intra WAR false
#pragma HLS DEPENDENCE variable=photon_data intra RAW false
#pragma HLS DEPENDENCE variable=photon_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=photon_data
#pragma HLS AGGREGATE compact=auto variable=photons_lane



//while (!instream.empty()){
#pragma HLS PIPELINE II=1

	sincegroup_t sinces;
	photongroupival_t photons;
	phasestream_t in;
	timestamp_t time;
	ap_uint<N_PHASE> trigger;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];
	ap_uint<N_PHASE> overflow;
	iqstream4x_t iq4x;
	iqstream_t iq8x;

	static bool iq_buffered;
	static iqstream4x_t iq4x_buffer;

	in = phase4x_in.read();
	if (!iq_buffered) {
		iq8x = iq8x_in.read();

		iq4x.data=iq8x.data.range(127,0);
		iq4x.user=iq8x.user*2;
		iq4x.last=false;

		iq4x_buffer.data=iq8x.data.range(255,128);
		iq4x_buffer.user=iq8x.user*2+1;
		iq4x_buffer.last=iq8x.last;
		iq_buffered=true;
//		cout<<"readiq "<<in.user<<" "<<iq8x.user<<" 4x:"<<iq4x.user<<endl;
//		cout<<iq4x.data<<", "<<iq4x_buffer.data<<endl;

	} else {
		iq4x=iq4x_buffer;
		iq_buffered=false;
//		cout<<"buffiq "<<in.user<<" "<<iq4x.user<<endl;
	}

	time = timestamp.read();
	desync=in.user!=iq4x.user;

	group_t group=in.user;
	group_t last_group=group_t(in.user-1);

	since_data[last_group]=since_cache;
	sinces = since_data[group];

	photon_data[last_group]=photon_cache;
	photons = photon_data[group];

	unpack_thresholds(threshoffs[group], threshs, hoffs);
	trigger=0;
	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;
		photon_noid_interval_t photon;
		phase_t phase;
		reschan_t id;
		bool update_photon;
		photon_t photon_out;
		bool emit;

		id = N_PHASE*reschan_t(group.range()) + i;
		phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);

		ap_fixed<16,1> phase_signed;
		ap_fixed<8,1> thresh_signed;
		phase_signed.range()=(uint16_t) phase;
		thresh_signed.range()=(uint8_t) threshs[i];

		trig=phase_signed<thresh_signed && sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;
		update_photon = photons.lane[i].phase>phase;


		emit=!trig && sinces.since[i]==1;

		if (trig) {
			sinces.since[i]=hoffs[i];
			photons.lane[i].phase=photon.phase;
			photons.lane[i].time=sinces.since[i];
//			cout<<"Trigger at "<<time<<" Since: "<<(uint16_t)sinces.since[i]<<" Saving: "<<(uint16_t)photons.lane[i].time<<endl;
		} else {
			if (sinces.since[i]>0) {
				interval_t new_since;
				new_since=sinces.since[i]-1;
				if (update_photon)  {
					photons.lane[i].phase=photon.phase;
					photons.lane[i].time=new_since;
//					cout<<"Updating at "<<time<<" to "<<(uint16_t)photons.lane[i].time<<endl;
				}
				if (sinces.since[i]==1) {
					photon_out.id=id;
					photon_out.phase=photons.lane[i].phase;
					photon_out.time=time-photons.lane[i].time;
//					cout<<"Outputting photon w/time "<<photon_out.time<<" Time: "<<time<<" Saved: "<<(uint16_t)photons.lane[i].time<<endl;
				}
				sinces.since[i]=new_since;
			}
		}

		if (emit) photons_lane[i].write(photon_out);
	}

	photon_cache=photons;
	since_cache=sinces;


	trigstream_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=in.user;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=in.last;
	out.data.range(N_PHASE*PHASE_BITS-1,0)=in.data;
	out.data.range(N_PHASE*(PHASE_BITS+IQ_BITS)-1,N_PHASE*PHASE_BITS)=iq4x.data;
//	cout<<"core: "<<out.data.range(N_PHASE*(PHASE_BITS+IQ_BITS)-1,N_PHASE*PHASE_BITS)<<endl;
	postage_stream.write(out);
//}
}



void fake_trigger(hls::stream<timestamp_t> &timestamp, threshoffs_t threshoffs[N_PHASEGROUPS],
		hls::stream<photon_t> photons_lane[N_PHASE]) {

	//emit photons on all channels at the holdoff rate, set to 0 to stop emission on a channel
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=timestamp depth=32000 register
#pragma HLS INTERFACE mode=axis depth=4 port=photons_lane register
#pragma HLS INTERFACE mode=s_axilite port=threshoffs bundle=control

	static sincegroup_t since_data[N_PHASEGROUPS], since_cache;
	static photongroupival_t photon_data[N_PHASEGROUPS], photon_cache;
#pragma HLS DEPENDENCE variable=since_data intra WAR false
#pragma HLS DEPENDENCE variable=since_data intra RAW false
#pragma HLS DEPENDENCE variable=since_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=since_data

#pragma HLS DEPENDENCE variable=photon_data intra WAR false
#pragma HLS DEPENDENCE variable=photon_data intra RAW false
#pragma HLS DEPENDENCE variable=photon_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=photon_data
#pragma HLS AGGREGATE compact=auto variable=photons_lane


#pragma HLS PIPELINE II=1

	sincegroup_t sinces;
	photongroupival_t photons;
	timestamp_t time;
	ap_uint<N_PHASE> trigger;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];


	time = timestamp.read();

	static group_t group=0;
	static uint16_t phase=0;
	group_t last_group=group_t(group-1);


	since_data[last_group]=since_cache;
	sinces = since_data[group];

	photon_data[last_group]=photon_cache;
	photons = photon_data[group];

	unpack_thresholds(threshoffs[group], threshs, hoffs);



	trigger=0;
	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;
		photon_noid_interval_t photon;
		phase_t phase;
		reschan_t id;
		bool update_photon;
		photon_t photon_out;
		bool emit;

		id = N_PHASE*reschan_t(group.range()) + i;


		trig=sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;


		emit=!trig && sinces.since[i]==1;

		if (trig) {
			sinces.since[i]=hoffs[i];
			photons.lane[i].phase=photon.phase;
			photons.lane[i].time=sinces.since[i];
//			cout<<"Trigger at "<<time<<" Since: "<<(uint16_t)sinces.since[i]<<" Saving: "<<(uint16_t)photons.lane[i].time<<endl;
		} else {
			if (sinces.since[i]>0) {
				interval_t new_since;
				new_since=sinces.since[i]-1;

				if (sinces.since[i]==1) {
					photon_out.id=id;
					photon_out.phase=photons.lane[i].phase;
					photon_out.time=time-photons.lane[i].time;
				}
				sinces.since[i]=new_since;
			}
		}

		if (emit) photons_lane[i].write(photon_out);
	}

	photon_cache=photons;
	since_cache=sinces;

//}


	group++;
	phase++;



}


void photon_packetizer(hls::stream<photon_t> &photons, photoncount_t max_photons_per_packet_minus2, ap_uint<5> approx_time_per_packet,
		hls::stream<photonstream_t> &photon_packets) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=s_axilite port=approx_time_per_packet
#pragma HLS PIPELINE II = 1
#pragma HLS INTERFACE mode=axis port=photons depth=100 register
#pragma HLS INTERFACE mode=axis port=photon_packets depth=100 register
#pragma HLS INTERFACE mode=s_axilite port=max_photons_per_packet_minus2
#pragma HLS AGGREGATE compact=auto variable=photons

	static photoncount_t count=0;
	static ap_uint<TIMESTAMP_BITS-9> _last_time_divided;
	static bool _last_this=false;


	bool _last_on_next, read;
	photon_t photon;
	photonstream_t beat;
	ap_uint<TIMESTAMP_BITS-9> time_divided; //in units of at least 512 us

	photon = photons.read();

	ap_uint<5> shft;
	shft = approx_time_per_packet;
	shft = shft < 9 ? ap_uint<5>(9): shft;
	shft = shft > 20 ? ap_uint<5>(20): shft;
//	assert(shft>=9);
//	assert(shft<=209);
	time_divided=photon.time>>shft;

	beat.data=photon2uint(photon);
	beat.last=_last_this;
	beat.dest=0;

	_last_on_next = (count>=max_photons_per_packet_minus2 || time_divided!=_last_time_divided) &! _last_this;


	photon_packets.write(beat);
	_last_time_divided=time_divided;
	if (_last_this) count=0;
	else count++;
	_last_this=_last_on_next;

}

void photon_fifo_packetizer(hls::stream<photon_t> photon_fifos[N_PHASE],  photoncount_t max_photons_per_packet_minus2, ap_uint<5> approx_time_per_packet,
		hls::stream<photonstream_t> &photon_packets) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II = 1
#pragma HLS INTERFACE mode=s_axilite port=approx_time_per_packet
#pragma HLS INTERFACE mode=s_axilite port=max_photons_per_packet_minus2
#pragma HLS INTERFACE mode=axis port=photon_packets depth=100 register
#pragma HLS INTERFACE mode=axis port=photon_fifos depth=100 register
#pragma HLS AGGREGATE compact=auto variable=photon_fifos
#pragma HLS ARRAY_PARTITION variable = photon_fifos complete

	static ap_uint<N_PHASE_LOG2> readfrom;
	static photoncount_t count=0;
	static ap_uint<TIMESTAMP_BITS-9> _last_time_divided;
	static bool _last_this=false;

	bool _last_on_next, read;
	photon_t photon;
	photonstream_t beat;
	ap_uint<TIMESTAMP_BITS-9> time_divided; //in units of at least 512 us


	ap_uint<5> shft;
	shft = approx_time_per_packet;
	shft = shft < 9 ? ap_uint<5>(9): shft;
	shft = shft > 20 ? ap_uint<5>(20): shft;

	read=photon_fifos[readfrom++].read_nb(photon); //read round robin

	time_divided=photon.time>>shft;

	beat.data=photon2uint(photon);
	beat.last=_last_this;
	beat.dest=0;
/// 4 phot in a packet, max_photons_per_packet_minus2=2
//1st _last_this=0, 0->1, 0>=2 no
//2nd _last_this=0, 1->2, 1>=2 no
//3rd _last_this=0, 2->3, 2>=2 yes, last on next=1
//4th _last_this=1, 3->0, 3>=2 yes, last on next=0, packet of 4 photons
//5th _last_this=0, 0->1, 1>=2 no
	_last_on_next = (count>=max_photons_per_packet_minus2 || time_divided!=_last_time_divided) &! _last_this;



	if (read) {
		photon_packets.write(beat);
		_last_time_divided=time_divided;
		if (_last_this) count=0;
		else count++;
		_last_this=_last_on_next;
	}

}

inline photon_uint_2x_t twophot_to_phot2x(photon_t a, photon_t b) {
	photon_uint_2x_t x;
	x.range(N_PHOTON_BITS-1,0)=photon2uint(a);
	x.range(2*N_PHOTON_BITS-1,N_PHOTON_BITS)=photon2uint(b);
	return x;
}

void photon_maxi(hls::stream<photon_t> &photons, photon_uint_2x_t photons_out[N_PHOTON_BUFFERS][FLAT_PHOTON_BUFSIZE/2],
				  photoncount_t n_photons[N_PHOTON_BUFFERS], unsigned char &active_buffer,
				  photoncount_t approx_max_photons_per_packet, ap_uint<5> time_shift) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=photons depth=64 register
#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave max_write_burst_length=256
#pragma HLS INTERFACE mode=s_axilite port=active_buffer
#pragma HLS INTERFACE mode=s_axilite port=n_photons
#pragma HLS INTERFACE mode=s_axilite port=approx_max_photons_per_packet
#pragma HLS INTERFACE mode=s_axilite port=time_shift
#pragma HLS DEPENDENCE direction=WAW type=inter variable=photons_out distance=8000 true
#pragma HLS DEPENDENCE direction=WAR type=inter variable=photons_out false
#pragma HLS DEPENDENCE direction=RAW type=inter variable=photons_out false
#pragma HLS AGGREGATE compact=auto variable=photons

	static ap_uint<N_PHOTON_BUFFERS_LOG2> _ab=0;
	ap_uint<TIMESTAMP_BITS-9> _last_time_divided;

#ifndef __SYNTHESIS__
while (!photons.empty()) {
#endif


	photon_t burstcache[512];
#pragma HLS AGGREGATE compact=auto variable=burstcache
	uint16_t cache_i;
	bool rotate_buffer;
	photoncount_t _photons_per_buf, _n_photons;

	ap_uint<5> shft;
	shft = time_shift;
	shft = shft < 9 ? ap_uint<5>(9): shft;
	shft = shft > 20 ? ap_uint<5>(20): shft;
	_photons_per_buf=approx_max_photons_per_packet;
	rotate_buffer=false;
	_n_photons=0;
	cache_i=0;

	buffer: while (!rotate_buffer) {
	#pragma HLS LOOP_TRIPCOUNT min=1 max=102400+16

		photon_t photon;
		ap_uint<TIMESTAMP_BITS-9> time_divided;

		photon = photons.read();
		burstcache[cache_i] = photon;
		time_divided=photon.time>>shft;

		if (cache_i==511) {
			burst512: for (int i=0;i<256;i++) {
			#pragma HLS PIPELINE II=1
				photons_out[_ab][_n_photons/2+i] = twophot_to_phot2x(burstcache[i*2], burstcache[i*2+1]);
			}
			cache_i=0;
			_n_photons+=512;
		} else
			cache_i++;

		rotate_buffer=_n_photons>=_photons_per_buf || time_divided!=_last_time_divided;

		_last_time_divided=time_divided;

//		n_photons[_ab]=_n_photons;
	}

	//cache_i will be at most 511
	//if cache_i==0 then we will write two old photons to the next buffer location,
	// but there is space in the buffer and the user-space count won't change so its not an actual issue
	//if cache_i==511 then last i=255 and all is fine.
	burstvar: for (int i=0;i<=cache_i/2;i++) {
#pragma HLS PIPELINE II=1
#pragma HLS LOOP_TRIPCOUNT min=1 max=256
		photons_out[_ab][_n_photons/2+i] = twophot_to_phot2x(burstcache[i*2], burstcache[i*2+1]);
	}
	_n_photons+=cache_i;
	n_photons[_ab]=_n_photons;
	active_buffer=_ab;
	cache_i=0;
	_n_photons=0;
	_ab++;
	n_photons[_ab]=0;

	#ifndef __SYNTHESIS__
	cout<<" rotate buffer "<<_n_photons<<endl;
}

	#endif

}



