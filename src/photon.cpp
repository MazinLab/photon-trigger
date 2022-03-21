#include "photon.hpp"
//#include "load_balance.hpp"

//This is the per-resonator trigger function that emmits photons into a fifo given the current and
//previous data It has passed sim
void phase_to_photion_res(reschan_t id, phase_t phase, threshold_t thresh, interval_t holdoff,
						  timestamp_t timestamp, previous_t &prev_dat, hls::stream<photon_t> &photon_fifo) {
#pragma HLS PIPELINE II=1
	bool trig_event;
	trig_event=prev_dat.phase>thresh && phase<prev_dat.phase;
#ifndef __SYNTHESIS__
if (id==0) {
cout<<"Phase is "<<phase<<" was "<<prev_dat.phase<<". Thresh is "<<thresh<<" Time="<<timestamp;
cout<<" Trigger event: "<<trig_event<<" Holdoff counter: "<<prev_dat.since<<"=="<<holdoff<<endl;
}
#endif
	if (trig_event && prev_dat.since==0 ) {
		prev_dat.since=holdoff;
		photon_t photon;
		photon.time=timestamp;
		photon.phase=phase;
		photon.id=id;

#ifndef __SYNTHESIS__
if (id==0) {
cout<<"EMITTING PHOTON: rid="<<photon.id<<" "<<photon.phase<<" at "<<photon.time<<endl;
}
#endif

		photon_fifo.write_nb(photon);
	} else if (prev_dat.since!=0) {
		prev_dat.since--;
	}
	prev_dat.phase=phase;
}

////This is a slight variant where the photon_t is packed into a uint for the output
//void phase_to_photion_res(reschan_t id, phase_t phase, threshold_t thresh, interval_t holdoff, timestamp_t timestamp, previous_t &prev_dat, hls::stream<photon_uint_t> &photon_fifo) {
//	bool trig_event;
//	trig_event=prev_dat.phase>thresh && phase<prev_dat.phase;
//#ifndef __SYNTHESIS__
//if (id==0) {
//cout<<"Phase is "<<phase<<" was "<<prev_dat.phase<<". Thresh is "<<thresh<<" Time="<<timestamp;
//cout<<" Trigger event: "<<trig_event<<" Holdoff counter: "<<prev_dat.since<<"=="<<holdoff<<endl;
//}
//#endif
//	if (trig_event && prev_dat.since==0 ) {
//		prev_dat.since=holdoff;
//		photon_t photon;
//		photon.time=timestamp;
//		photon.phase=phase;
//		photon.id=id;
//#ifndef __SYNTHESIS__
//if (id==0) {
//cout<<"EMITTING PHOTON"<<endl;
//}
//#endif
//		photon_fifo.write_nb(photon2uint(photon));
//	} else if (prev_dat.since!=0) {
//		prev_dat.since--;
//	}
//	prev_dat.phase=phase;
//}

void fetch_thresh(threshold_t thresholds[N_PHASEGROUPS][N_PHASE], int group, int i, threshold_t &thresh) {
#pragma HLS INLINE off
	thresh = thresholds[group][i];
}

//This is an inner partial trigger function, it passed c-sim. It outputs N_PHASE fifos that much be aggregated
void phase_to_photon_df(hls::stream<phasestream_t> &instream, threshold_t thresholds[N_PHASEGROUPS][N_PHASE],
		volatile timestamp_t &timestamp, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE]){
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=thresholds
#pragma HLS AGGREGATE compact=byte variable=photon_fifo
#pragma HLS INTERFACE ap_ctrl_none port=return


	static previous_t previous_data[N_PHASEGROUPS][N_PHASE];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=previous_data
#pragma HLS AGGREGATE compact=auto variable=previous_data

#ifndef __SYNTHESIS__
	cout<<"Called with "<<instream.size()<<" phases."<<endl;
#endif

	int group;
	group: while (!instream.empty()){
#pragma HLS PIPELINE II=1 REWIND
//	group: for (int group=0;group<512;group++){
//#pragma HLS PIPELINE II=512 REWIND
		phasestream_t in;

		in = instream.read();
		group=in.user;

		lanes: for (int i=0;i<N_PHASE;i++) {
#pragma HLS UNROLL

			bool trig_event;

		#ifndef __SYNTHESIS__
			if(i==0 && group==0) {
				cout<<"Group "<<group<<":"<<i<<endl;
			}
		#endif

			phase_t phase;
			threshold_t thresh;
			phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//			thresh=thresholds[group].range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//			thresh=thresholds[group][i];
			fetch_thresh(thresholds, group, i, thresh);
			phase_to_photion_res(N_PHASE*group+i, phase, thresh, holdoff, timestamp, previous_data[group][i], photon_fifo[i]);

		}
	}
}

void ingest_lanify(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				   hls::stream<timestamp_t> &timestamp, hls::stream<dataset_t> &outstream) {
#pragma HLS AGGREGATE compact=byte variable=outstream
#pragma HLS INTERFACE ap_ctrl_none port=return

	group: while (!instream.empty()){
#pragma HLS PIPELINE II=1 REWIND
//	group: for (int group=0;group<512;group++){
//#pragma HLS PIPELINE II=512 REWIND
		phasestream_t in;
		dataset_t out;
		in = instream.read();
		out.phases=in.data;
		out.group=in.user;
		out.threshs=thresholds[in.user];
		out.timestamp=timestamp.read();
		outstream<<out;
	}
}

void photonizer(hls::stream<dataset_t> &instream, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE]) {
	dataset_t dataset;
	previousgroup_t previous;
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo

	static previousgroup_t previous_data[N_PHASEGROUPS];
//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=previous_data
#pragma HLS AGGREGATE compact=auto variable=previous_data

	group: while (!instream.empty()){
#pragma HLS PIPELINE II=1 REWIND
		dataset=instream.read();
		previous = previous_data[dataset.group];
		lanes: for (int i=0;i<N_PHASE;i++) {
			#pragma HLS UNROLL
			phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			phase_to_photion_res(N_PHASE*dataset.group+i, phase, thresh, holdoff, dataset.timestamp,
					previous.data[i], photon_fifo[i]);
		}
		previous_data[dataset.group]=previous;
	}
}

//This is an toplevel partial trigger function, it passed c-sim. It outputs N_PHASE fifos that much be aggregated
void phase_to_photon(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
		volatile timestamp_t &timestamp, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE]){
#pragma HLS INTERFACE mode=ap_fifo port=photon_fifo depth=1600
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo
#pragma HLS AGGREGATE compact=byte variable=photon_fifo
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register depth=
#pragma HLS INTERFACE mode=ap_none port=timestamp
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control


	static previous_t previous_data[N_PHASEGROUPS][N_PHASE];
#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=previous_data
#pragma HLS AGGREGATE compact=auto variable=previous_data

#ifndef __SYNTHESIS__
	cout<<"Called with "<<instream.size()<<" phases."<<endl;
#endif

	group: for (int group=0;group<512;group++){
#pragma HLS PIPELINE II=512 REWIND
		phasestream_t in;

		in = instream.read();
		group=in.user;

		lanes: for (int i=0;i<N_PHASE;i++) {
#pragma HLS UNROLL

			bool trig_event;

		#ifndef __SYNTHESIS__
			if(i==0 && group==0) {
				cout<<"Group "<<group<<":"<<i<<endl;
			}
		#endif

			phase_t phase, thresh;
			phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			thresh=thresholds[group].range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			phase_to_photion_res(N_PHASE*group+i, phase, thresh, holdoff, timestamp, previous_data[group][i], photon_fifo[i]);

		}

		if (instream.empty()) break;
	}
}


void photon_uint_to_photon(hls::stream<photon_uint_t> &ostrm, hls::stream<photon_t> &photons) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
	while (!ostrm.empty()) {
		photons.write(uint2photon(ostrm.read()));
	}
}

inline ap_uint<4> countOnes(ap_uint<4> y) {
#pragma HLS inline

    ap_uint<4> x0 = y;
    ap_uint<4> x1 = (x0 & 0x5) + ((x0 & 0xa) >> 1);
    ap_uint<4> x2 = (x1 & 0x3) + ((x1 & 0xc) >> 2);
    return x2;
}



void stream_n_to_one_read_lb_jeb(hls::stream<photon_t> istrms[N_PHASE],
                             	 hls::stream<ap_uint<PHOTON_WID * N_PHASE> >& buf_lcm_strm) {
//#pragma HLS INTERFACE mode=ap_ctrl_none port=return
    const int buf_width = PHOTON_WID * N_PHASE;
    const int num_in = N_PHASE;
    const int up_nstrm = N_PHASE;
    ap_uint<2 * buf_width> buff_a = 0;
    ap_uint<buf_width> buff_b = 0;
    const ap_uint<N_PHASE> ends = -1;
    ap_uint<N_PHASE> val = 0;
    ap_uint<N_PHASE> bak_val = 0;
    int base = 0;
    ap_uint<PHOTON_WID * N_PHASE> tmpb[N_PHASE];
#pragma HLS ARRAY_PARTITION variable = tmpb complete
    photon_t ttm[N_PHASE];
#pragma HLS ARRAY_PARTITION variable = ttm complete


    while (true) {
#pragma HLS pipeline II = 1 //rewind
//    for(int group=0;group<512;group++) {
//#pragma HLS pipeline II = 1 rewind

    	//Attempt a read from all the input streams
        readfifos: for (int i = 0; i < N_PHASE; ++i) {
#pragma HLS unroll
        	ttm[i] = uint2photon(0);
            val[i] = istrms[i].read_nb(ttm[i]);
        } // for

        // move the available data to "the right position"
        // if a stream is empty there is no point in moving its data
        // since its data is 0
        tmpb[0] = photon2uint(ttm[0]);
        multimux: for (int i = 1; i < N_PHASE; ++i) {
#pragma HLS unroll
            ap_uint<N_PHASE> v = val.range(i - 1, 0);
            int ones = countOnes(v); // it's similar to round robin  if ones always is i.
            int p = ones;            // index of tmpb[i].range(), where istrm[i] is stored if it
                                     // is not empty
            photon_uint_t d = photon2uint(ttm[i]);
            tmpb[i] = d << (p * PHOTON_WID);
        } // for

        buff_b = 0;
        merge: for (int i = 0; i < N_PHASE; ++i) {
#pragma HLS pipeline II = 1
            buff_b |= tmpb[i]; // merge data
        }

        int un = countOnes(val); // how many new data are collected to buffer at this time

        // accumulate data
        if (un > 0) {
            buff_a.range((base + un) * PHOTON_WID - 1, base * PHOTON_WID) = buff_b.range(un * PHOTON_WID - 1, 0);
        }

        // output one data
        if (base + un >= num_in) {
            //  the size of buff_a is big enough
            base = base + un - num_in;
            buf_lcm_strm.write(buff_a.range(buf_width - 1, 0));
            // move the remaining data to the start position
            buff_a = buff_a >> buf_width;
        } else {
            base += un;	// accumulated data
        } // if-else
    } // while
}


void stream_n_to_one_read_lb_jb(hls::stream<photon_t> istrms[N_PHASE],
                             hls::stream<ap_uint<PHOTON_WID * N_PHASE> >& buf_lcm_strm) {
//#pragma HLS INTERFACE mode=ap_ctrl_none port=return
    const int buf_width = PHOTON_WID * N_PHASE;
    const int num_in = N_PHASE;
    const int up_nstrm = N_PHASE;
    ap_uint<2 * buf_width> buff_a = 0;
    ap_uint<buf_width> buff_b = 0;
    const ap_uint<N_PHASE> ends = -1;
    ap_uint<N_PHASE> val = 0;
    ap_uint<N_PHASE> bak_val = 0;
    int base = 0;
    ap_uint<PHOTON_WID * N_PHASE> tmpb[N_PHASE];
#pragma HLS ARRAY_PARTITION variable = tmpb complete
    ap_uint<PHOTON_WID> ttm[N_PHASE];
#pragma HLS ARRAY_PARTITION variable = ttm complete


    while (!istrms[0].empty()) {
#pragma HLS pipeline II = 1 rewind
//    for(int group=0;group<512;group++) {
//#pragma HLS pipeline II = 1 rewind
        for (int i = 0; i < N_PHASE; ++i) {
#pragma HLS unroll
            bool vl;
            vl=!istrms[i].empty();
            val[i] = vl; // flag of available data
            // neither  empty nor finished stream, read it, or default zero
            ttm[i] = vl ? photon2uint(istrms[i].read()) : (ap_uint<PHOTON_WID>)0;
        } // for

        // move the available data to "the right position"
        // if a stream is empty or finished, there is no point in moving its data
        // since its data is 0
        tmpb[0] = ttm[0];
        for (int i = 1; i < N_PHASE; ++i) {
#pragma HLS unroll
            ap_uint<up_nstrm> v = val.range(i - 1, 0);
            int ones = countOnes(v); // it's similar to round robin  if ones always is i.
            int p = ones;            // index of tmpb[i].range(), where istrm[i] is stored if it
                                     // is not empty
            ap_uint<PHOTON_WID * N_PHASE> d = ttm[i];
            tmpb[i] = d << (p * PHOTON_WID);

        } // for

        buff_b = 0;
        merge: for (int i = 0; i < N_PHASE; ++i) {
#pragma HLS pipeline II = 1
            buff_b |= tmpb[i]; // merge data
        }

        int un = countOnes(val); // how many new data are collected to buffer at this time

        // accumulate data
        if (un > 0) {
            buff_a.range((base + un) * PHOTON_WID - 1, base * PHOTON_WID) = buff_b.range(un * PHOTON_WID - 1, 0);
        }

        // output one data
        if (base + un >= num_in) {
            //  the size of buff_a is big enough
            base = base + un - num_in;
            buf_lcm_strm.write(buff_a.range(buf_width - 1, 0));
            // move the remaining data to the start position
            buff_a = buff_a >> buf_width;
        } else {
            base += un;	// accumulated data
        } // if-else
    } // while
}

void stream_n_to_one_distribute_lb_jb(hls::stream<ap_uint<PHOTON_WID * N_PHASE>> &buf_lcm_strm,
									 hls::stream<photon_uint_t> &ostrm) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
    const int buf_width = PHOTON_WID * N_PHASE;
    const int num_out = N_PHASE;

    ap_uint<buf_width> inner_buf;
    int low = 0;
    int up = 0;
    int c = num_out;
    unsigned int up_pos = -1;
    distribute: while (!buf_lcm_strm.empty()) {
#pragma HLS pipeline II = 1 rewind
//	distribute: for(int group=0;group<512*N_PHASE;group++) {
//#pragma HLS pipeline II = 1 rewind
        if (c == num_out) {
            inner_buf = buf_lcm_strm.read();
            c = 0;
        }

        if ((c + 1) * PHOTON_WID <= up_pos)
            ostrm.write(inner_buf.range((c + 1) * PHOTON_WID - 1, c * PHOTON_WID));
        c++;
    } // loop
}

//This is a top level dataflow function that aggregates the streams with a variant of the code in load_balance.hpp
//void photon_trigger_hotmess_wxilinx(hls::stream<phasestream_t> &instream, threshold_t thresholds[N_PHASEGROUPS][N_PHASE],
//					volatile timestamp_t &timestamp, interval_t holdoff, hls::stream<photon_t> &photons){
//#pragma HLS DATAFLOW
////#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=axis port=photons depth=6400
//#pragma HLS INTERFACE mode=ap_none port=timestamp
//#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
//#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control
//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=thresholds
//
//	hls::stream<photon_t> photon_fifos[N_PHASE];
//#pragma HLS AGGREGATE compact=byte variable=photon_fifos
//#pragma HLS stream variable = photon_fifos depth = 8
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos
//
//    hls::stream<ap_uint<PHOTON_WID * N_PHASE> > buf_n_strm;
//#pragma HLS stream variable = buf_n_strm depth = 8
//
//    hls::stream<photon_uint_t> ostrm;
//#pragma HLS stream variable = ostrm depth = 8
//
//	phase_to_photon_df(instream, thresholds, timestamp, holdoff, photon_fifos);
//	stream_n_to_one_read_lb_jeb(photon_fifos, buf_n_strm);
//    stream_n_to_one_distribute_lb_jb(buf_n_strm, ostrm);
//    photon_uint_to_photon(ostrm, photons);
//
//}
//
//
void read_distribute(hls::stream<photon_t> istrms[N_PHASE], hls::stream<photon_t> &out) {
#pragma HLS ARRAY_PARTITION variable = istrms complete

	ap_uint<N_PHASE> n=0; // current arbitration grant
	ap_uint<N_PHASE> readable;
	canread: for (int i = 0; i < N_PHASE; ++i) {
	#pragma HLS unroll
		readable[i] = !istrms[i].empty();
	}
	distin: while (readable>0) {
	#pragma HLS pipeline II = 1 //rewind
	//    for(int group=0;group<512;group++) {
	//#pragma HLS pipeline II = 1 rewind

	    	//determine which streams have data
			canread2: for (int i = 0; i < N_PHASE; ++i) {
			#pragma HLS unroll
				readable[i] = !istrms[i].empty();
			}

			//read from the next stream. next stream is last+1
			shift: while (readable!=0 && !readable[n]) n++;
			if (readable[n]) out.write(istrms[n].read());
	}

}


//This is a top level dataflow function that aggregates the streams with a variant of the code in load_balance.hpp
void photon_trigger(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
					hls::stream<timestamp_t> &timestamps, interval_t holdoff, hls::stream<photon_t> &photons){
#pragma HLS DATAFLOW
//#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=axis port=photons depth=6400
#pragma HLS INTERFACE mode=ap_none port=timestamp
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control

	hls::stream<photon_t> photon_fifos[N_PHASE];
#pragma HLS AGGREGATE compact=byte variable=photon_fifos
#pragma HLS stream variable = photon_fifos depth = 8
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos

	hls::stream<dataset_t> datastream;
#pragma HLS stream variable = datastream depth = 8
#pragma HLS AGGREGATE compact=byte variable=datastream

	//	phase_to_photon_df(instream, thresholds, timestamp, holdoff, photon_fifos);


	ingest_lanify(instream, thresholds, timestamps, datastream);
	photonizer(datastream, holdoff, photon_fifos);
#ifndef __SYNTHESIS__
	cout<<photon_fifos[0].size()<<" "<<photon_fifos[1].size()<<" "<<photon_fifos[2].size()<<" "<<photon_fifos[3].size()<<endl;
#endif
	read_distribute(photon_fifos, photons);
}




// This is an ap_ctrl_none flat implementation and was the first one ever done. It has never been tested.
//void phase_to_photon(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
//		volatile timestamp_t timestamp, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE]){
//#pragma HLS INTERFACE mode=ap_fifo port=photon_fifo depth=100
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo
//#pragma HLS AGGREGATE compact=byte variable=photon_fifo
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=100 //register depth=
//#pragma HLS INTERFACE mode=ap_none port=timestamp
//#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
//#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control
//
//
//	static previous_t previous_data[N_PHASEGROUPS][N_PHASE];
//#pragma HLS ARRAY_PARTITION dim=2 type=complete variable=previous_data
//#pragma HLS AGGREGATE compact=auto variable=previous_data
//
//	//holdoff is 10-15 us @1MHz so 10-15 samples 1 sample every 512/255/128 clocks so holdoff is 1280 clocks min with a 16x wide
//
//	group: for (int group=0;group<512;group++){
//#pragma HLS PIPELINE II=512 REWIND
//		phasestream_t in;
////		group512_t group;
//		phase_t phases[N_PHASE];
//		threshold_t threshs[N_PHASE];
//		previous_t prev_dats[N_PHASE];
//		thresholds_t thresholds_packed;
//
//		in = instream.read();
//		group=in.user;
//		thresholds_packed=thresholds[group];
//
//		unpack: for (int i=0;i<N_PHASE;i++) {
//#pragma HLS UNROLL
//			phases[i]=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//			threshs[i]=thresholds_packed.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//			prev_dats[i] = previous_data[group][i];
//		}
//
//		lanes: for (int i=0;i<N_PHASE;i++) {
//#pragma HLS UNROLL
//
//			previous_t prev_dat;
//			prev_dat=prev_dats[i];
//
//			bool trig_event;
//			trig_event=prev_dat.phase>threshs[i] && phases[i]<prev_dat.phase;
//#ifndef __SYNTHESIS__
//			if(i==0) {
//			cout<<"Group "<<group<<":"<<i<<" phase is "<<phases[i]<<" was "<<prev_dat.phase;
//			cout<<". Thresh is "<<threshs[i];
//			cout<<" Trigger event: "<<trig_event<<" Holdoff counter: "<<prev_dat.since<<"=="<<holdoff<<endl;
//			}
//#endif
//
//			if (trig_event && prev_dat.since==0 ) {
//
//				photon_t photon;
//				photon.time=timestamp;
//				photon.phase=phases[i];
//				photon.id=N_PHASE*group+i;
//				photon_fifo[i].write_nb(photon);
//				prev_dat.since=holdoff;
//			} else if (prev_dat.since!=0) {
//				prev_dat.since--;
//			}
//
//			prev_dat.phase=phases[i];
//
//			prev_dats[i]=prev_dat;
//		}
//
//		repack: for (int i=0;i<N_PHASE;i++) {
//#pragma HLS UNROLL
//#ifndef __SYNTHESIS__
//			if(i==0) {
//			cout<<"Prev_data"<<prev_dats[i].since<<":"<<prev_dats[i].phase<<endl;
//			}
//#endif
//			previous_data[group][i]=prev_dats[i];
//		}
//		if (instream.empty()) break;
//	}
//}


//// This is the second half of the aggregation function
//void photons_to_axis(hls::stream<photon_t> photon_fifo[N_PHASE], hls::stream<photon_t> &photons) {
//#pragma HLS INTERFACE mode=ap_fifo port=photon_fifo depth=1600
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo
//#pragma HLS AGGREGATE compact=byte variable=photon_fifo
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=axis port=photons depth=6400 register
//
//	ap_uint<N_PHASE> run=-1;
//	while (run!=0) {
//		for (int i=0;i<N_PHASE;i++) {
//	#pragma HLS UNROLL
//			photon_t tmp;
//			run[i]=photon_fifo[i].read_nb(tmp);
//			if (run[i])
//				photons.write(tmp);
//		}
//	}
//
//}


