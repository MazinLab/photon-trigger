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
 * Postage stamp data comes in sequenitally and easily. Photons arrive randomly but clustered so need to be sorted
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


void postage_filter(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]) {

#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=instream  depth=74752 register
#pragma HLS INTERFACE mode=axis port=iniq depth=74752 register
#pragma HLS INTERFACE mode=axis port=iq_out depth=200 register
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
#pragma HLS INTERFACE mode=s_axilite port=monitor bundle=control
#pragma HLS ARRAY_PARTITION variable=monitor type=complete

	 static ap_shift_reg<unsigned int, N_CAPPRE+1> iqprereg[N_MONITOR];
	 static unsigned char tocapture[N_MONITOR];
	 //for (int i=0;i<N_MONITOR;i++) tocapture[i]=0;
#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete


//	while(!instream.empty()) {
	#pragma HLS PIPELINE II=1
		trigstream_t in;
		iqstreamnarrow_t iq;
		group_t group;
		ap_uint<N_PHASE> trigger;
		group512_t align;

		in=instream.read();
		iq=iniq.read();

		group=in.user.range(N_PHASEGROUPS_LOG2-1,0);
		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);

		iq_t x[N_PHASE];
		#pragma HLS ARRAY_PARTITION variable=x type=complete
		for (int i=0;i<N_PHASE;i++)
			x[i]=iq.data.range(IQ_BITS*(i+1)-1, i*IQ_BITS);

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

			if (gid==group) {
				if (tocapture[i]>0) {
					iq_out[i].write(tmp);
					tocapture[i] = nexttocap;
				}
				else if (trigger[lane]) {
					tocapture[i]=N_CAPDATA;
				}
			}
		}
//	}
}



void postage_maxi(hls::stream<singleiqstream_t> &postage, iq_t iq[N_MONITOR][POSTAGE_BUFSIZE][N_CAPDATA],
				  uint16_t event_count[N_MONITOR]){
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=postage register
#pragma HSL INTERFACE mode=s_axilite port=event_count
#pragma HLS INTERFACE mode=m_axi max_widen_bitwidth=128 port=iq offset=slave max_write_burst_length=256


	iq_t buf[N_MONITOR][N_CAPDATA];
	uint8_t sample_count[N_MONITOR];
	uint16_t _event_count[N_MONITOR];
	uint16_t _maxevents=0;
	for (int i=0;i<N_MONITOR;i++) {
		for (int j=0;j<N_CAPDATA;j++) buf[i][j]=0;
		sample_count[i]=0;
		_event_count[i]=0;
	}

	while(_maxevents<POSTAGE_BUFSIZE) {

		singleiqstream_t tmp;
		bool have_burst;

		tmp = postage.read();

		buf[tmp.user][sample_count[tmp.user]]=tmp.data;
		have_burst = sample_count[tmp.user] == N_CAPDATA-1;
		if (have_burst) {
			for (int k=0;k<N_CAPDATA;k++)
				iq[tmp.user][_event_count[tmp.user]][k]=buf[tmp.user][k];
			sample_count[tmp.user]=0;
			_event_count[tmp.user]++;
			event_count[tmp.user]=_event_count[tmp.user];
		} else {
			sample_count[tmp.user]++;
		}
		_maxevents = _maxevents>_event_count[tmp.user] ? _maxevents: _event_count[tmp.user];
	}
//	for (int k=0;k<N_CAPDATA;k++) {
//		for (int j=0;j<event_count[k];j++) {
//			for (int j=0;j<N_CAPDATA;j++) {
//				iq[i][j][k]=buf[i][k];
//			}
//		}
//	}


}


