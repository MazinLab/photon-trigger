#include "photon.hpp"

/*
 * Trigger streamer
 * Inputs: threshold, holdoff, phasestream  (ap_ctrl_none possible)
 * Out: triggeredstream, triggerstream?
 *
 * Photon (ap_ctl_none, 512 MHZ):
 * In: triggeredstream, timestamp
 * out: photon stream(s) w/times & resids
 *
 * Photon writer: (axis2mm core?)
 * in: photon stream(s), mem address
 * out: axi_m writes
 *
 * Postage:
 * In: trigger stream, alignment offset (could be fixed/hardcoded), resonators to monitor
 * Out (stream): stream w/ tlast at end of each photon, use internal fifos to  (ap_ctrl_none possible)
 * Out (streams): n streams w/ tlast at end of each photon (ap_ctrl_none possible)
 * Out (maxi): bursted writes to memory per-monitor memory address
 * N IQ values of photon event will take N*256 to arrive N to send out
 */



void photon_factory(hls::stream<trigstream_t> &instream, hls::stream<timestamp_t> &timestamp,
		hls::stream<photon_t> photon_fifo[N_PHASE]){//, hls::stream<bool> &done) {
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo
#pragma HLS INTERFACE ap_ctrl_none port=return


	bool finished=false;
	group: while (true){//!finished){
#pragma HLS PIPELINE II=1 REWIND
		trigstream_t dataset;
		reschan_t idbase;
		timestamp_t time;
		ap_uint<N_PHASE> trigger;

		dataset=instream.read();
		time=timestamp.read();

		idbase=N_PHASE*reschan_t(dataset.user.range(N_PHASEGROUPS_LOG2-1,0));
		trigger=dataset.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);

		lanes: for (int i=0;i<N_PHASE;i++) {
			photon_t photon;
			photon.time=time;
			photon.phase=dataset.data.range(PHASE_BITS*(1+i)-1, PHASE_BITS*i); //todo change to phases[i]?
			photon.id=idbase+i;
			if (trigger[i]) {
				photon_fifo[i].write(photon);
//				#ifndef __SYNTHESIS__
//				if (photon.id<8) {
//				cout<<"EMITTING PHOTON: rid="<<photon.id<<" "<<photon.phase<<" at "<<photon.time<<endl;
//				}
//				#endif
			}
		}
		finished=instream.empty();
	}
//	done.write(true);
}


void read_distribute(hls::stream<photon_t> istrms[N_PHASE], hls::stream<photon_t> &out) { //, hls::stream<bool> &done
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable = istrms complete

//	ap_uint<N_PHASE_LOG2> n=0; // current arbitration grant
	ap_uint<N_PHASE> readable=-1;
	bool finished=false, skipdone=false;
//	while(istrms[0].empty());
	distin: while (true){//!finished) {
	#pragma HLS pipeline II = 4 //rewind
		//determine which streams have data
//		canread2: for (int i = 0; i < N_PHASE; i++) { //I had this as ++i wont that skip [0] forever???
//		#pragma HLS unroll
//			readable[i] = !istrms[i].empty();
//		}
		skipdone=false;
		for (int n=0;n<N_PHASE;n++) {
#pragma HLS UNROLL
			photon_t photon;
			bool read;
			read=istrms[n].read_nb(photon);
			//readable[n]=read;
			if (read) {
				skipdone=true;
				out.write(photon);
			}
		}
//		if (!skipdone)
//			done.read_nb(finished);
//		if (readable>0){
			//Just go round robin 512 resonators per lane at 5000 cps gives a maximum photon rate of 2.56 Mphot
			//round robin each lane will be cleared at 128Mphot
//			photon_t photon;
//			if (istrms[ap_uint<N_PHASE_LOG2>(n++)].read_nb(photon))
//				out.write(photon);
//#ifndef __SYNTHESIS__
//if (photon.id<8) {
//cout<<"OUTPUT PHOTON: rid="<<photon.id<<" "<<photon.phase<<" at "<<photon.time<<endl;
//}
//#endif
//		} else {
//			done.read_nb(finished);
//		}
	}
}


void photon(hls::stream<trigstream_t> &instream, hls::stream<timestamp_t> &timestamps, hls::stream<photon_t> &photons){
#pragma HLS DATAFLOW
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=photons depth=6400
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=axis port=timestamps depth=13500

	hls::stream<photon_t> photon_fifos[N_PHASE];
#pragma HLS AGGREGATE compact=bit variable=photon_fifos
#pragma HLS stream variable = photon_fifos depth = 4
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos

//	hls::stream<bool> done;
//#pragma HLS stream variable = done depth = 2


	photon_factory(instream, timestamps, photon_fifos);//, done);

	#ifndef __SYNTHESIS__
		cout<<photon_fifos[0].size()<<" "<<photon_fifos[1].size()<<" "<<photon_fifos[2].size()<<" "<<photon_fifos[3].size()<<endl;
	#endif

	read_distribute(photon_fifos, photons); //done
}



