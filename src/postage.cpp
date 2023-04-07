#include "photon.hpp"


inline void channel_to_grouplane(reschan_t x, group_t &group, lane_t &lane){
	lane=x.range(N_PHASE_LOG2-1,0);
	group=x>>N_PHASE_LOG2;
}


void postage(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]) {

#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=instream  depth=74752 register
#pragma HLS INTERFACE mode=axis port=iniq depth=74752 register
#pragma HLS INTERFACE mode=axis port=iq_out depth=200 register
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
#pragma HLS INTERFACE mode=s_axilite port=monitor bundle=control
#pragma HLS ARRAY_PARTITION variable=monitor type=complete

	 ap_shift_reg<unsigned int, N_CAPPRE> iqprereg[N_MONITOR];
	 unsigned char tocapture[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete


//	unsigned int cycle=0;
	while(true){//!instream.empty()) {
	#pragma HLS PIPELINE II=1 REWIND
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
			channel_to_grouplane(monitor[i], group, lane);

			laneiq = iqprereg[i].shift(x[lane], N_CAPPRE-1, gid==group);

			nexttocap = tocapture[i]-1;

			singleiqstream_t tmp;
			tmp.user=i;
			tmp.last=tocapture[i]==0;
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
//		cycle++;
	}
}


//Nick has encouraged support for every trigger up to 10k counts per second, At 10k we have a trigger every 100us
// samples arrive every us. with a window of 100 post trigger we can just attain this. getting the data out takes 1<us so that isn't the limit
