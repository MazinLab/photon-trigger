#include "photon.hpp"


void phase_trigger(phase_t phase, threshold_t thresh, interval_t holdoff, previous_t &prev_dat, bool &trigger) {
#pragma HLS PIPELINE II=1
	bool trig_event;
	trig_event=prev_dat.phase>thresh && phase<prev_dat.phase;
	if (trig_event && prev_dat.since==0 ) {
		prev_dat.since=holdoff;
		trigger=true;
	} else if (prev_dat.since!=0) {
		prev_dat.since--;
		trigger=false;
	} else {
		trigger=false;
	}
	prev_dat.phase=phase;
}

//This is a top level dataflow function that streams out phases and  aggregates the streams with a variant of the code in load_balance.hpp
void trigger(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
			 interval_t holdoff, hls::stream<trigstream_t> &outstream){

#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control

	static previousgroup_t previous_data[N_PHASEGROUPS], previous_cache;

#pragma HLS DEPENDENCE variable=previous_data intra WAR false
#pragma HLS DEPENDENCE variable=previous_data intra RAW false
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
#pragma HLS AGGREGATE compact=auto variable=previous_data

//	int cycle=0;
	while (!instream.empty()) {
	#pragma HLS PIPELINE II=1 rewind
		previousgroup_t previous;
		phasestream_t in;
		datasetnotime_t dataset;
		in = instream.read();
		dataset.phases=in.data;
		dataset.group=in.user;
		dataset.last=in.last;
		dataset.threshs=thresholds[in.user];

		ap_uint<N_PHASE> trigger=0;
		previous_data[group_t(dataset.group-1)]=previous_cache;
		previous = previous_data[dataset.group];
		phases_t outputdata;
		for (int j=0;j<N_PHASE;j++) {
		#pragma HLS UNROLL
			outputdata.range(PHASE_BITS*(j+1)-1, PHASE_BITS*j)=previous.data[j].phase;
		}

		lanes: for (int i=0;i<N_PHASE;i++) {
			#pragma HLS UNROLL
			phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			bool trig;
	//		phase_trigger(phase, thresh, holdoff, previous.data[i], trig);

			trig=previous.data[i].phase>thresh && phase<previous.data[i].phase && previous.data[i].since==0;

//#ifndef __SYNTHESIS__
//	if ((dataset.group==511 || dataset.group==0) && i==0) {
//		cout<<"T Cycle "<<cycle<<" ("<<dataset.group<<") phase0: "<<phase<<" was "<< previous.data[i].phase<<" holdoffctr="<<previous.data[i].since<<" trig="<<trig<<endl;
//	}
////	if ((dataset.group==511 || dataset.group==0) && i==3) {
////		cout<<"T Cycle "<<cycle<<" ("<<dataset.group<<") phase0: "<<phase<<" was "<< previous.data[i].phase<<" holdoffctr="<<previous.data[i].since<<" trig="<<trig<<endl;
////	}
//#endif
			previous.data[i].phase=phase;
			if (trig) {
				previous.data[i].since=holdoff;
			} else if (previous.data[i].since!=0) {
				previous.data[i].since--;
			}

			trigger[i]=trig;

		}

		previous_cache=previous;

		trigstream_t out;
		out.user.range(N_PHASEGROUPS_LOG2-1, 0)=dataset.group;
		out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
		out.last=dataset.last;
		out.data=outputdata;
		outstream.write(out);
//		cycle++;
	}
}
