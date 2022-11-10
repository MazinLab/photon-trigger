#include "photon.hpp"

void postage(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]) {

#pragma HLS INTERFACE ap_ctrl_none port=return // bundle=control
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=74752 //register
#pragma HLS INTERFACE mode=axis port=iniq register_mode=off depth=74752 //register
#pragma HLS INTERFACE mode=axis port=iq_out register_mode=off depth=200 //register
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
#pragma HLS INTERFACE mode=s_axilite port=monitor bundle=control
#pragma HLS ARRAY_PARTITION variable=monitor type=complete

	 ap_shift_reg<unsigned int, N_CAPPRE> iqprereg[N_MONITOR];
	 unsigned char tocapture[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete


//	unsigned int cycle=0;
	while(true){//!instream.empty()) {
	#pragma HLS PIPELINE II=1
		trigstream_t in;
		iqstreamnarrow_t iq;
		iqpgroup_t iqaligned;
		group_t group;
		ap_uint<N_PHASE> trigger;
		group512_t align;

		in=instream.read();
		iq=iniq.read();

		group=in.user.range(8,0);
		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);
		iqaligned = iq.data;

		iq_t x[N_PHASE];
	#pragma HLS ARRAY_PARTITION variable=x type=complete
		for (int i=0;i<N_PHASE;i++)
			x[i]=iqaligned.range(IQ_BITS*(i+1)-1, i*IQ_BITS);

		for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL
			ap_uint<N_PHASE_LOG2> lane;
			group_t gid;
			reschan_t m;
			iq_t laneiq;
			m=monitor[i];
			lane=m.range(N_PHASE_LOG2-1,0);
			gid=m>>N_PHASE_LOG2;


			laneiq = iqprereg[i].shift(x[lane], N_CAPPRE-1, gid==group);

			if (gid==group) {
				if (tocapture[i]>0) {
					singleiqstream_t tmp;
					tmp.user=i;
					tmp.last=tocapture[i]==1;
					tmp.data=laneiq;
					iq_out[i].write(tmp);
		#ifndef __SYNTHESIS__
		if (group==0 && lane==0 && i==0) {
		if (tocapture[i]==N_CAPDATA){cout<<"Cycle "<<cycle<<" ("<<group<<") Lane 0 IQ: ";
		cout<<" in "<<iq.data.range(IQ_BITS-1, 0);
		cout<<" Phase "<<in.data.range(PHASE_BITS,0)<<" trigger "<<trigger[0]<<endl;
		}
		cout<<"  Capturing "<<N_CAPDATA-tocapture[i]<<" of "<<N_CAPDATA<<" iq "<<laneiq/2048.0<<endl;
		}
//		if (group==0 && lane==0) cout<<"  Capturing, "<<N_CAPDATA-tocapture[i]<<" of "<<N_CAPDATA<<endl;
//						if (group==0 && tmp.last &&lane==0) cout<<"  Finishing capture."<<endl;

		#endif
				}

				if (tocapture[i]==0 && trigger[lane]) {
					#ifndef __SYNTHESIS__
							if (group==0 && lane==0) {
							cout<<"  Starting capture"<<endl;
							}
					#endif
					tocapture[i]=N_CAPDATA;
				} else if (tocapture[i]!=0)
					tocapture[i]--;
			}
		}
//		cycle++;
	}
}


//Nick has encouraged support for every trigger up to 10k counts per second, At 10k we have a trigger every 100us
// samples arrive every us. with a window of 100 post trigger we can just attain this. getting the data out takes 1<us so that isn't the limit
