#include "photon.hpp"
iq_t select_iq(iqpgroup_t data, ap_uint<N_PHASE_LOG2> lane) {
#pragma HLS INLINE off
	iq_t x[N_PHASE];
#pragma HLS ARRAY_PARTITION variable=x type=complete
	for (int i=0;i<N_PHASE;i++)
		x[i]=data.range(IQ_BITS*(i+1)-1, i*IQ_BITS);
	return x[lane];
}

inline void resonator_watcher(iqpgroup_t datain, group_t group, bool trigger, ap_uint<N_PHASE_LOG2> lane, group_t gid, hls::stream<iq_t> &out) {
//#pragma HLS ALLOCATION function instances=resonator_watcher limit=10
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS PIPELINE II=1

	static ap_shift_reg<iq_t, N_CAPPRE> sreg;
	static unsigned char capturing=0;
	static iq_t lastv;
//	ap_uint<N_PHASE_LOG2> lane;
	iq_t outdata, indata;

	iq_t x[N_PHASE];
#pragma HLS ARRAY_PARTITION variable=x type=complete
	for (int i=0;i<N_PHASE;i++)
		x[i]=datain.range(IQ_BITS*(i+1)-1, i*IQ_BITS);

	outdata = sreg.shift(lastv, N_CAPPRE-1, true);
	lastv=x[lane];
	if (capturing>0) {
		out.write(outdata);
		capturing--;
	} else if (group==gid && trigger) {
		capturing=N_CAPDATA;
	}
}

void resonator_watcher(iqpgroup_t datain, group_t group, bool trigger, reschan_t id, hls::stream<iq_t> &out, hls::stream<bool> &last) {
#pragma HLS INTERFACE ap_ctrl_none port=return

	static ap_shift_reg<iq_t, N_CAPPRE> sreg;
	static unsigned char capturing=0;
	ap_uint<N_PHASE_LOG2> lane;
	iq_t outdata, indata;

	lane=id%N_PHASE;
	indata = datain.range(IQ_BITS*(lane+1)-1, IQ_BITS*lane);
	outdata = sreg.shift(indata, N_CAPPRE-1, true);

	if (capturing>0) {
		out.write(outdata);
		last.write(capturing==1);
		capturing--;
	} else if (group==id/N_PHASE && trigger) {
		capturing=N_CAPDATA;
	}
}

//
//void resonator_monitor_nested(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
//		reschan_t monitor[N_MONITOR], int alignment, hls::stream<bool> &done, hls::stream<iq_t> iq_out[N_MONITOR]) {
//#pragma HLS PIPELINE II=1
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=axis port=iniq register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=axis port=iq_out register_mode=off depth=13500 //register
//#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
//#pragma HLS INTERFACE mode=s_axilite port=monitor
//#pragma HLS INTERFACE mode=s_axilite port=alignment
//#pragma HLS ARRAY_PARTITION variable=monitor type=complete
//
//	ap_shift_reg<iqpgroup_t, N_MAXALIGN> sreg;
//
//	bool finished=false;
//	group: while (!finished){
//#pragma HLS PIPELINE II=1 REWIND
//		trigstream_t in;
//		iqstreamnarrow_t iq;
//		iqpgroup_t iqaligned;
//		group_t group;
//		ap_uint<N_PHASE> trigger;
//		group512_t align;
//
//		in=instream.read();
//		finished=instream.empty();
//		iq=iniq.read();
//
//		//The iq stream should be several ahead of the phase AND may be be a cycle or so in-front
//		group=in.user.range(8,0);
//
////		align=iq.user-group>>1;
//
//		iqaligned = sreg.shift(iq.data, alignment, true);
//		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);
//
//		for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//			ap_uint<N_PHASE_LOG2> lane;
//			group_t gid;
//			lane=monitor[i].range(N_PHASE_LOG2-1,0);
//			gid=monitor[i]>>N_PHASE_LOG2;
//			resonator_watcher(iqaligned, group, trigger[i], lane, gid, iq_out[i]);
//		}
//
//	}
//	done.write(true);
//}



//
//void resonator_monitor(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
//		reschan_t monitor[N_MONITOR], int alignment, hls::stream<bool> &done, hls::stream<iq_t> iq_out[N_MONITOR]) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
//#pragma HLS ARRAY_PARTITION variable=monitor type=complete
//
//	ap_shift_reg<iqpgroup_t, N_MAXALIGN> sreg;
//
//	ap_shift_reg<unsigned int, N_CAPPRE> iqprereg[N_MONITOR];
//	unsigned char tocapture[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=tocapture type=complete
//#pragma HLS ARRAY_PARTITION variable=iqprereg type=complete
//
//	bool finished=false;
//	group: while (!finished){
//#pragma HLS PIPELINE II=1 REWIND
//		trigstream_t in;
//		iqstreamnarrow_t iq;
//		iqpgroup_t iqaligned;
//		group_t group;
//		ap_uint<N_PHASE> trigger;
//		group512_t align;
//
//		in=instream.read();
//		iq=iniq.read();
//
//		//The iq stream should be several ahead of the phase AND may be be a cycle or so in-front
//		group=in.user.range(8,0);
//
////		align=iq.user-group>>1;
//
//		iqaligned = sreg.shift(iq.data, alignment, true);
//		trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);
//
//		iq_t x[N_PHASE];
//	#pragma HLS ARRAY_PARTITION variable=x type=complete
//		for (int i=0;i<N_PHASE;i++)
//			x[i]=iqaligned.range(IQ_BITS*(i+1)-1, i*IQ_BITS);
//
//		for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//			ap_uint<N_PHASE_LOG2> lane;
//			group_t gid;
//			reschan_t m;
//			m=monitor[i];
//			lane=m.range(N_PHASE_LOG2-1,0);
//			gid=m>>N_PHASE_LOG2;
//			iq_t laneiq;
//
//			laneiq = iqprereg[i].shift(x[lane], N_CAPPRE-1, gid==group);
//
//			if (gid==group) { //
//				if (tocapture[i]>0)
//					iq_out[i].write(laneiq);
//				if (tocapture[i]==0 && trigger[lane])
//					tocapture[i]=N_CAPDATA;
//				else if (tocapture[i]!=0)
//					tocapture[i]--;
//			}
//		}
//		finished=instream.empty();
//	}
//	done.write(true);
//}


//while(!instream.empty()) {
//#pragma HLS PIPELINE II=1
//	trigstream_t in;
//	iqstreamnarrow_t iq;
//	iqpgroup_t iqaligned;
//	group_t group;
//	ap_uint<N_PHASE> trigger;
//	group512_t align;
//
//	in=instream.read();
//	iq=iniq.read();
//
//	//The iq stream should be several ahead of the phase AND may be be a cycle or so in-front
//	group=in.user.range(8,0);
//
//	iqaligned = sreg.shift(iq.data, alignment, true);
//	trigger=in.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);
//
//	iq_t x[N_PHASE];
//#pragma HLS ARRAY_PARTITION variable=x type=complete
//	for (int i=0;i<N_PHASE;i++)
//		x[i]=iqaligned.range(IQ_BITS*(i+1)-1, i*IQ_BITS);
//
//	for (int i=0;i<N_MONITOR;i++) {
//	#pragma HLS UNROLL
//		ap_uint<N_PHASE_LOG2> lane;
//		group_t gid;
//		reschan_t m;
//		iq_t laneiq;
//		m=monitor[i];
//		lane=m.range(N_PHASE_LOG2-1,0);
//		gid=m>>N_PHASE_LOG2;
//
//
//		laneiq = iqprereg[i].shift(x[lane], N_CAPPRE-1, gid==group);
//
//		if (gid==group) { //
//			if (tocapture[i]>0) {
//				singleiqstream_t tmp;
//				tmp.user=i;
//				tmp.last=tocapture[i]==1;
//				tmp.data=laneiq;
//				iq_out[i].write(tmp);
//			}
//			if (tocapture[i]==0 && trigger[lane])
//				tocapture[i]=N_CAPDATA;
//			else if (tocapture[i]!=0)
//				tocapture[i]--;
//		}
//	}
//}


void write_maxi(hls::stream<iq_t> iq_in[N_MONITOR], hls::stream<bool> &done, iq_t *iq_out[N_MONITOR]) {
	ap_uint<16> n[N_MONITOR];
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete
#pragma HLS ARRAY_PARTITION variable=iq_in complete

	for (int i=0;i<N_MONITOR;i++) {
#pragma HLS UNROLL
		iq_out[i][n[i]++]=iq_in[i].read();
	}
}

void write_stream(hls::stream<iq_t> istrms[N_MONITOR], hls::stream<bool> &done, hls::stream<singleiqstream_t> &out) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable = istrms complete

	ap_uint<N_MONITOR_LOG2> n=0; // current arbitration grant
	ap_uint<N_MONITOR> readable;
	bool finished=false;

	while (!finished) {
//	#pragma HLS pipeline II = 1

		//determine which streams have data
		canread2: for (int i = 0; i < N_MONITOR; i++) {
			#pragma HLS unroll
			readable[i] = !istrms[i].empty();
		}
		if (readable>0){
			//read from the next stream. next stream is last+1
			shift: while (!readable[n++]);

			readevent: for(int j=0;j<N_CAPDATA;j++) {
				singleiqstream_t tmp;
				tmp.user=n;
				tmp.last=j==(N_CAPDATA-1);
				tmp.data=istrms[n].read();
				out.write(tmp);
			}
		} else {
			done.read_nb(finished);
		}
	}

}

//stream
//void postage_df_stream(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq, int alignment, reschan_t monitor[N_MONITOR],
//		hls::stream<singleiqstream_t> &iq_out) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=axis depth=13500 port=iq_out
//#pragma HLS INTERFACE mode=axis depth=13500 port=iniq
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=s_axilite port=monitor
//#pragma HLS INTERFACE mode=s_axilite port=alignment
//
//#pragma HLS DATAFLOW
//
//	hls::stream<bool> done;
//#pragma HLS stream variable = done depth = 32
//
//	hls::stream<iq_t> iq_fifos[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=iq_fifos type=complete
//
//	resonator_monitor(instream, iniq, monitor, alignment, done, iq_fifos);
//	write_stream(iq_fifos, done, iq_out);
//}


//maxi
//void postage_maxi(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq, int alignment, reschan_t monitor[N_MONITOR], iq_t *iq_out[N_MONITOR]) {
//
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out offset=slave
//#pragma HLS INTERFACE mode=axis depth=13500 port=iniq
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=iq_out
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=s_axilite port=monitor
//#pragma HLS INTERFACE mode=s_axilite port=alignment
//
//#pragma HLS DATAFLOW
//
//	hls::stream<bool> done;
//#pragma HLS stream variable = done depth = 32
//
//	hls::stream<iq_t> iq_fifos[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=iq_fifos type=complete
//
//	resonator_monitor(instream, iniq, monitor, alignment, done, iq_fifos);
//	write_maxi(iq_fifos, done, iq_out);
//}
//
//



void postage(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]) {

#pragma HLS INTERFACE s_axilite port=return  bundle=control
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
