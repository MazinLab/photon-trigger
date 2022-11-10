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

inline void unpack_thresholds(threshoffs_t toffs, threshold_t threshs[N_PHASE], interval_t holdsoffs[N_PHASE]) {

	unpack: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		threshoff_t to = toffs.range(THRESHOFF_BITS*(i+1)-1, THRESHOFF_BITS*i);
		threshs[i]=to&(2^THRESHOLD_BITS -1);
		holdsoffs[i]=to>>THRESHOLD_BITS;
	}
}

void trigger(hls::stream<phasestream_t> &instream, threshoffs_t threshoffs[N_PHASEGROUPS],  hls::stream<trigstream_t> &outstream){
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream depth=13500 register
#pragma HLS INTERFACE mode=s_axilite port=threshoffs bundle=control

	static sincegroup_t since_data[N_PHASEGROUPS], since_cache;
#pragma HLS DEPENDENCE variable=since_data intra WAR false
#pragma HLS DEPENDENCE variable=since_data intra RAW false
#pragma HLS DEPENDENCE variable=since_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=since_data


	#pragma HLS PIPELINE II=1 //rewind
	sincegroup_t sinces;
	phasestream_t in;
	ap_uint<N_PHASE> trigger=0;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];

	in = instream.read();
	since_data[group_t(in.user-1)]=since_cache;
	sinces = since_data[in.user];

	unpack_thresholds(threshoffs[in.user], threshs, hoffs);

	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;

		phase_t phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);


		trig=phase<threshs[i] && sinces.since[i]==0;
		if (trig) {
			sinces.since[i]=hoffs[i];
		} else if (sinces.since[i]!=0) {
			sinces.since[i]--;
		}

		trigger[i]=trig;

	}

	since_cache=sinces;

	trigstream_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=in.user;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=in.last;
	out.data=in.data;
	outstream.write(out);

}
