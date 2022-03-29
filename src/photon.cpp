#include "photon.hpp"

//This is the per-resonator trigger function that emmits photons into a fifo given the current and
//previous data It has passed sim
void phase_to_photion_res(reschan_t id, phase_t phase, threshold_t thresh, interval_t holdoff,
						  timestamp_t timestamp, previous_t &prev_dat, hls::stream<photon_t> &photon_fifo) {
#pragma HLS PIPELINE II=1
	bool trig_event;
	trig_event=prev_dat.phase>thresh && phase<prev_dat.phase;
#ifndef __SYNTHESIS__
if (id==0 || id==4) {
cout<<"ID:"<<id<<" Phase is "<<phase<<" was "<<prev_dat.phase<<". Thresh is "<<thresh<<" Time="<<timestamp;
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
if (id<8) {
cout<<"EMITTING PHOTON: rid="<<photon.id<<" "<<photon.phase<<" at "<<photon.time<<endl;
}
#endif

		photon_fifo.write_nb(photon);
	} else if (prev_dat.since!=0) {
		prev_dat.since--;
	}
	prev_dat.phase=phase;
}



void ingest_lanify(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				   hls::stream<timestamp_t> &timestamp, hls::stream<dataset_t> &outstream, hls::stream<bool> &done) {
#pragma HLS AGGREGATE compact=byte variable=outstream
#pragma HLS INTERFACE ap_ctrl_none port=return
	bool finished=false;
	group: while (!finished){
#pragma HLS PIPELINE II=1 REWIND
		phasestream_t in;
		dataset_t out;
		in = instream.read();
		out.phases=in.data;
		out.group=in.user;
		out.threshs=thresholds[in.user];
		out.timestamp=timestamp.read();
		outstream<<out;
		done.write(false);
		finished=instream.empty();
	}
	done.write(true);
}

previousgroup_t data_fetcher(previousgroup_t previous_data[N_PHASEGROUPS], group_t group) {
#pragma HLS PIPELINE II=1
#pragma HLS INLINE off
#pragma HLS DEPENDENCE variable=previous_data intra WAR true
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
	return previous_data[group];
}

void photonizer(hls::stream<dataset_t> &instream, hls::stream<bool> &done, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE], hls::stream<bool> &doneout) {
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifo

	previousgroup_t previous_data[N_PHASEGROUPS];
//#pragma HLS BIND_STORAGE variable=previous_data type=ram_s2p impl=bram latency=1
#pragma HLS DEPENDENCE variable=previous_data intra WAR false
#pragma HLS DEPENDENCE variable=previous_data intra RAW false
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
#pragma HLS AGGREGATE compact=auto variable=previous_data

	bool finished=false;
	group_t group;
	previousgroup_t previous_data_cache;
	group: while (!finished){
#pragma HLS PIPELINE II=1 REWIND
		previousgroup_t previous;
		dataset_t dataset;
		dataset=instream.read();
		previous_data[group-1]=previous_data_cache;
		previous = previous_data[group];
		lanes: for (int i=0;i<N_PHASE;i++) {
			#pragma HLS UNROLL
			phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			phase_to_photion_res(N_PHASE*reschan_t(dataset.group)+i, phase, thresh, holdoff, dataset.timestamp,
					previous.data[i], photon_fifo[i]);
		}
		previous_data_cache=previous;
		group++;
		finished=done.read();
	}
	doneout.write(true);
}


void read_distribute(hls::stream<photon_t> istrms[N_PHASE], hls::stream<bool> &done, hls::stream<photon_t> &out) {
#pragma HLS ARRAY_PARTITION variable = istrms complete

	ap_uint<N_PHASE_LOG2> n=0; // current arbitration grant
	ap_uint<N_PHASE> readable;
	bool finished=false;
	distin: while (!finished) {
	#pragma HLS pipeline II = 1 //rewind
		//determine which streams have data
		canread2: for (int i = 0; i < N_PHASE; ++i) {
		#pragma HLS unroll
			readable[i] = !istrms[i].empty();
		}
		if (readable>0){
			//read from the next stream. next stream is last+1
			shift: while (!readable[n++]);

			photon_t photon;
			photon=istrms[ap_uint<N_PHASE_LOG2>(n-1)].read();
			out.write(photon);
#ifndef __SYNTHESIS__
if (photon.id<8) {
cout<<"OUTPUT PHOTON: rid="<<photon.id<<" "<<photon.phase<<" at "<<photon.time<<endl;
}
#endif

		} else {
			done.read_nb(finished);
		}
	}
}


//This is a top level dataflow function that aggregates the streams with a variant of the code in load_balance.hpp
void photon_trigger(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
					hls::stream<timestamp_t> &timestamps, interval_t holdoff, hls::stream<photon_t> &photons){
#pragma HLS DATAFLOW
//#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=photons depth=6400
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=axis port=timestamps depth=13500
//#pragma HLS INTERFACE mode=ap_none port=timestamps
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control

	hls::stream<photon_t> photon_fifos[N_PHASE];
#pragma HLS AGGREGATE compact=byte variable=photon_fifos
#pragma HLS stream variable = photon_fifos depth = 128
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos

	hls::stream<dataset_t> datastream;
#pragma HLS stream variable = datastream depth = 32
#pragma HLS AGGREGATE compact=byte variable=datastream

	hls::stream<bool> done1, done2;
#pragma HLS stream variable = done1 depth = 32
#pragma HLS stream variable = done2 depth = 32


	ingest_lanify(instream, thresholds, timestamps, datastream, done1);
	photonizer(datastream, done1, holdoff, photon_fifos, done2);
#ifndef __SYNTHESIS__
	cout<<photon_fifos[0].size()<<" "<<photon_fifos[1].size()<<" "<<photon_fifos[2].size()<<" "<<photon_fifos[3].size()<<endl;
#endif
	read_distribute(photon_fifos, done2, photons);
}


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


void ingest_trigstream(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				  	   hls::stream<datasetnotime_t> &outstream, hls::stream<bool> &done) {
#pragma HLS AGGREGATE compact=byte variable=outstream
#pragma HLS INTERFACE ap_ctrl_none port=return
	bool finished=false;
	group: while (!finished){
#pragma HLS PIPELINE II=1 REWIND
		phasestream_t in;
		datasetnotime_t out;
		in = instream.read();
		out.phases=in.data;
		out.group=in.user;
		out.last=in.last;
		out.threshs=thresholds[in.user];
		outstream<<out;
		done.write(false);
		finished=instream.empty();
	}
	done.write(true);
}

void trigger_tag(hls::stream<datasetnotime_t> &instream, hls::stream<bool> &done, interval_t holdoff, hls::stream<trigstream_t> &outstream) {
	datasetnotime_t dataset;
	bool finished=false;
	previousgroup_t previous;

	static previousgroup_t previous_data[N_PHASEGROUPS];
#pragma HLS DEPENDENCE variable=previous_data intra WAR false
#pragma HLS DEPENDENCE variable=previous_data intra RAW false
#pragma HLS DEPENDENCE variable=previous_data inter RAW distance=128 true
#pragma HLS AGGREGATE compact=auto variable=previous_data

	group: while (!done.read()){
#pragma HLS PIPELINE II=1 REWIND
		ap_uint<N_PHASE> trigger=0;
		dataset=instream.read();
		previous = previous_data[dataset.group];
		lanes: for (int i=0;i<N_PHASE;i++) {
			#pragma HLS UNROLL
			phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
			bool trig;
			phase_trigger(phase, thresh, holdoff, previous.data[i], trig);
			trigger[i]=trig;
		}
		previous_data[dataset.group]=previous;
		trigstream_t out;
		out.user.range(N_PHASEGROUPS_LOG2-1, 0)=dataset.group;
		out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
		out.last=dataset.last;
		out.data=dataset.phases;
		outstream.write(out);
	}
}


//bool finished=false;
//group_t group;
//previousgroup_t previous_data_cache;
//group: while (!finished){
//#pragma HLS PIPELINE II=1 REWIND
//	previousgroup_t previous;
//	dataset_t dataset;
//	dataset=instream.read();
//	previous_data[group-1]=previous_data_cache;
//	previous = previous_data[group];
//	lanes: for (int i=0;i<N_PHASE;i++) {
//		#pragma HLS UNROLL
//		phase_t phase=dataset.phases.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//		threshold_t thresh=dataset.threshs.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);
//		phase_to_photion_res(N_PHASE*reschan_t(dataset.group)+i, phase, thresh, holdoff, dataset.timestamp,
//				previous.data[i], photon_fifo[i]);
//	}
//	previous_data_cache=previous;
//	group++;
//	finished=done.read();
//}
//doneout.write(true);



//This is a top level dataflow function that streams out phases and  aggregates the streams with a variant of the code in load_balance.hpp
void trigger_stream_flat(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				   interval_t holdoff, hls::stream<trigstream_t> &outstream){
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
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

	trigstream_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=dataset.group;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=dataset.last;
	out.data=dataset.phases;
	outstream.write(out);
}

#define N_MAXALIGN 50

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






//This is a top level dataflow function that streams out phases and  aggregates the streams with a variant of the code in load_balance.hpp
void trigger_stream_df(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
				   interval_t holdoff, hls::stream<trigstream_t> &outstream){
#pragma HLS DATAFLOW
//#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE mode=s_axilite port=thresholds bundle=control


	hls::stream<datasetnotime_t> datastream;
#pragma HLS stream variable = datastream depth = 8
#pragma HLS AGGREGATE compact=byte variable=datastream

	hls::stream<bool> done1;
	ingest_trigstream(instream, thresholds, datastream, done1);
	trigger_tag(datastream, done1, holdoff, outstream);
}


void resonator_watcher(trigstream_t datain, reschan_t id, hls::stream<iq_t> &out) {
	static ap_shift_reg<iq_t, N_CAPPRE> sreg;
	static unsigned char capturing=0;
	ap_uint<N_PHASE> trigger;
	ap_uint<N_PHASE_LOG2> lane;
	iq_t outdata, indata;
	group_t group;
	lane=id%N_PHASE;
	group=datain.user.range(N_PHASEGROUPS_LOG2-1,0);
	trigger=datain.user.range(N_PHASE_LOG2+N_PHASEGROUPS_LOG2-1,N_PHASEGROUPS_LOG2);
	indata=datain.data.range(31,0);//todo
	outdata = sreg.shift(indata,N_CAPPRE-1,true);

	if (group==id/N_PHASE && trigger[lane] && capturing==0) {
		capturing=N_CAPDATA;
	}

	if (capturing>0) {
		out.write(outdata);
		capturing--;
	}

}


void resonator_monitor(hls::stream<trigstream_t> &instream, reschan_t monitor[N_MONITOR], hls::stream<iq_t> iq_out[N_MONITOR]) {
#pragma HLS PIPELINE II=1
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=s_axilite port=monitor

	trigstream_t in;
	in=instream.read();
	for (int i=0;i<N_MONITOR;i++) {
#pragma HLS UNROLL
		resonator_watcher(in, monitor[i], out[i]);
	}
}

void monitor_writer(hls::stream<iq_t> iq_out[N_MONITOR], volatile iq_t *iq_out[N_MONITOR]) {
	ap_uint<16> n[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=n type=complete
	for (int i=0;i<N_MONITOR;i++) {
#pragma HLS UNROLL
		iq_out[i][n[i]++]=iq_out.read();
	}
}

void resmon(hls::stream<trigstream_t> &instream, reschan_t monitor[N_MONITOR], volatile iq_t *iq_out[N_MONITOR]) {

#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out offset=slave
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=iq_out
#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
#pragma HLS INTERFACE mode=s_axilite port=monitor

#pragma HLS DATAFLOW

	hls::stream<iq_t> iq_out[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=iq_out type=complete

	resonator_monitor(instream, monitor, iq_fifos);
	monitor_writer(iq_fifos, iq_out);
}



