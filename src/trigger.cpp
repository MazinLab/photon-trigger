#include "photon.hpp"
//This is a top level dataflow function that streams out phases and  aggregates the streams with a variant of the code in load_balance.hpp
void trigger_stream(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				    interval_t holdoff, hls::stream<trigstreamiq_t> &outstream){
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=axis port=iniqstream register_mode=off depth=13500
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control

	phasestream_t in;
	datasetnotime_t dataset;

	in = instream.read();
	dataset.phases=in.data;
	dataset.group=in.user;
	dataset.last=in.last;
	dataset.threshs=thresholds[in.user];

	static previousgroup_t previous_data[N_PHASEGROUPS], previous_cache;
	previousgroup_t previous;
#pragma HLS DEPENDENCE variable=previous_data intra WAR false
#pragma HLS DEPENDENCE variable=previous_data intra RAW false
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
#pragma HLS AGGREGATE compact=auto variable=previous_data


	ap_uint<N_PHASE> trigger=0;
	previous_data[dataset.group-1]=previous_cache;
	previous = previous_data[dataset.group];
	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
		threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
		bool trig;
		phase_trigger(phase, thresh, holdoff, previous.data[i], trig);
		trigger[i]=trig;
	}
	previous_cache=previous;

	trigstreamiq_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=dataset.group;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=dataset.last;
	out.data=dataset.phases;
	outstream.write(out);
}

//This is a top level dataflow function that streams out phases and  aggregates the streams with a variant of the code in load_balance.hpp
void trigger_stream(hls::stream<phasestream_t> &instream, hls::stream<iqstream_t> &iniqstream, thresholds_t thresholds[N_PHASEGROUPS],
				        interval_t holdoff, int align, hls::stream<trigstreamiq_t> &outstream, bool &misaligned){
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=axis port=iniqstream register_mode=off depth=13500
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control
#pragma HLS INTERFACE mode=s_axilite port=align bundle=control
#pragma HLS INTERFACE mode=s_axilite port=misaligned bundle=control

	phasestream_t in;
	datasetnotime_t dataset;
	iqstream_t iqin;
	iqpgroup_t iqdata;
	in = instream.read();
	dataset.phases=in.data;
	dataset.group=in.user;
	dataset.last=in.last;
	dataset.threshs=thresholds[in.user];

	static ap_shift_reg<iqpgroup_t, N_MAXALIGN> sreg;
	iqin = iniqstream.read();
	iqdata = sreg.shift(iqin.data, align, true);

	misaligned = (iqin.user - align) != in.user;

	static previousgroup_t previous_data[N_PHASEGROUPS], previous_cache;
	previousgroup_t previous;
#pragma HLS DEPENDENCE variable=previous_data intra WAR false
#pragma HLS DEPENDENCE variable=previous_data intra RAW false
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
#pragma HLS AGGREGATE compact=auto variable=previous_data


	ap_uint<N_PHASE> trigger=0;
	previous_data[dataset.group-1]=previous_cache;
	previous = previous_data[dataset.group];
	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
		threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
		bool trig;
		phase_trigger(phase, thresh, holdoff, previous.data[i], trig);
		trigger[i]=trig;
	}
	previous_cache=previous;

	trigstreamiq_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=dataset.group;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=dataset.last;
	out.data.range(N_PHASE*PHASE_BITS-1,0)=dataset.phases;
	out.data.range(N_PHASE*(PHASE_BITS+IQ_BITS)-1, N_PHASE*PHASE_BITS) = iqdata;
	outstream.write(out);
}
