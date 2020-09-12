

//void trigger(phase_t phase, phase_t threshold, holdoff_t holdoff, timestamp_t timestamp, trigout_t* state) {
//#pragma HLS PIPELINE II=1
//	if (state->triggered) {
//		if (phase>threshold) { //trigger done
//		    state->triggered=false;
//		    state->stop=timestamp - state->start;
//	    }
//		else if (phase <= state->peak) {
//	    	state->peak=phase;
//	    	state->peak_time=timestamp-state->start;
//	    } else
//	    	state->triggered=true;
//
//	} else if (phase<threshold && (timestamp - state->start) > holdoff) {
//		state->peak=phase;
//		state->peak_time=0;
//	    state->start=timestamp;
//	    state->stop=0;
//	    state->triggered=true;
//	} else {
//		state->peak=0;
//		state->peak_time=0;
//	    state->start=timestamp;
//	    state->stop=0;
//	    state->triggered=false;
//	}
//}



//
//void package_output(photonevent_t event, streamout_t &out){
//#pragma HLS INLINE
//	out.data|=event.time;
//	out.data|=eventuint_t(event.phase)<<64;
//	out.data|=eventuint_t(event.res_id)<<(64+16);
//	out.data|=eventuint_t(event.baseline)<<(64+16*2);
//}
//
//
//void output_event_info1x(resstate_t resstate,  hls::stream<photonevent_t> &lane_event, const ap_uint<2> feedline) {
//#pragma HLS PIPELINE II=1
//#pragma HLS DATA_PACK variable=lane_event.V
//
//	photonevent_t event;
//	event.time=resstate.peak_time+resstate.trigger_start;
//	event.phase=resstate.peak;
//	event.baseline=resstate.baseline;
//	event.res_id=resstate.res+feedline*N_GROUPS*N_LANES;
//	//event.duration=resstate->trigger_stop;
//	if (resstate.trigger_stop>0)
//		lane_event.write_nb(event);
//
//};
//
//void output_event_info(resstate_t state_group[N_LANES],  hls::stream<photonevent_t> lane_events[N_LANES], const ap_uint<2> feedline) {
//#pragma HLS PIPELINE II=1
//
//	photonevent_t event_out;
//
//	event: for (int i=0;i<N_LANES;i++) {
//		resstate_t *resstate = &state_group[i];
//		photonevent_t event;
//		if (resstate->trigger_stop!=0) {
//			event.time=resstate->peak_time+resstate->trigger_start;
//			event.phase=resstate->peak;
//			event.baseline=resstate->baseline;
//			event.res_id=resstate->res+feedline*N_GROUPS*N_LANES;
//			//event.duration=resstate->trigger_stop;
//		}
//		lane_events[i].write_nb(event);
//	}
//
//};

//
//
//void attach_capture_channels(timestamp_t time, CaptureManager* capture_man, resstate_t state_group[N_LANES]){
//	for (int i=0;i<N_LANES;i++) {
//		resstate_t* state = &state_group[i];
//
//		//resonators can start getting captured only on the cycle they are
//		// triggered and only if there is an available capture channel
//		if (state->trigger_start==time) // we'd like a capture channel
//			state->capture_channel = capture_man->get_channel(state);
//	}
//}
//
//
//void capture_phases(phase_t phases[N_LANES], resstate_t state_group[N_LANES],
//					CaptureManager *capture_man) {
//
//	for (int i=0;i<N_LANES;i++) {
//		resstate_t *resstate = &state_group[i];
//		phase_t phase=phases[i];
//		bool complete;
//
//		complete = capture_man->capture(phase, resstate->capture_channel);
//		if (complete)
//			resstate->capture_channel=0;
//	}
//}



//
//void output_phase_captures(CaptureManager *capture_man, hls::stream<capture_out_t> &capture_out) {
//
//
//	static lane_t next_out;
//
//	lanenfo_t lanenfo = capture_man->lane_state[next_out];
//	ap_uint<256> out;
//	if(lanenfo.next_to_out!=lanenfo.first_loading) {
//	   //TODO build output data
//	   out=capture_man->meta_buffer[lanenfo.next_to_out];
//	   for(int i=0;i<capture_man->capture_limit;i++)
//		   out=capture_man->phase_buffer[lanenfo.next_to_out][i];
//
//	   capture_out.write_nb(out);
//
//	   lane_t lane = lanenfo.next_to_out%N_LANES;
//	   capchanID_t last_chan_for_lane = N_CHANNELS-N_LANES+1+lane;
//	   if (lanenfo.next_to_out==last_chan_for_lane)
//		   lanenfo.next_to_out=lane + 1;
//	   else
//		   lanenfo.next_to_out+=N_LANES;
//
//	}
//
//	next_out++;
//
//
//}
//
//void photon_trigger2(hls::stream<phasestream_t> &phasesin, hls::stream<photonevent_t> &events_out,
//					 hls::stream<capture_out_t> &capture_out,
//				     triggercfg_t triggercfg[N_GROUPS][N_LANES], globalcfg_t globalcfg,
//				     timestamp_t timestamp, const ap_uint<2> lane) {
//#pragma HLS PIPELINE  II=1
//#pragma HLS INTERFACE axis register port=phasesin
//#pragma HLS INTERFACE axis register port=photon_events
//#pragma HLS DATA_PACK variable=photon_events[0].data
//#pragma HLS ARRAY_PARTITION variable=photon_events complete
//#pragma HLS ARRAY_PARTITION variable=triggercfg dim=2 complete
////#pragma HLS INTERFACE s_axilite port=return bundle=control
//#pragma HLS INTERFACE s_axilite port=triggercfg bundle=control
//#pragma HLS INTERFACE s_axilite port=globalcfg bundle=control
//#pragma HLS INTERFACE ap_ctrl_none port=return
//
//	static group_t group;
//
//	static triggerstat_t trigger_state[N_GROUPS][N_LANES];
//#pragma HLS ARRAY_PARTITION variable=trigger_status dim=2 complete
//	static triggerstat_t last_status[N_LANES];
//#pragma HLS ARRAY_PARTITION variable=last_status complete
//
//	static resstate_t resstate[N_GROUPS][N_LANES];
//
//	static CaptureManager capture_man;
//
//	phasestream_t transactionin=phasesin.read();
//
//	phase_t phases[N_LANES];
//	resstate_t state_group[N_LANES];
//	triggercfg_t cfg[N_LANES];
//
//	for (int i=0;i<N_LANES;i++) {
//		phases[i]=transactionin.data[i];
//		state_group[i]=resstate[group][i];
//		cfg[i]=triggercfg[group][i];
//	}
//
//	update_baselines(phases, state_group, globalcfg);
//	update_trigger_info(phases, state_group, cfg, globalcfg, timestamp);
//	output_event_info(state_group, events_out);
//	attach_capture_channels(timestamp, &capture_man, state_group);
//	capture_phases(phases, state_group, &capture_man);
//	output_phase_captures(&capture_man, capture_out);
//
//
//	group = transactionin.last ? group_t(0):group_t(group+1);
//
//}



//
//
//bool run_trigger(resID_t resid, phase_t phase, triggerstat_t &state, triggercfg_t cfg, globalcfg_t globalcfg, timestamp_t time, photonevent_t &event) {
//
//	if (state.triggered) {
//		if (phase>cfg.threshold) { //trigger done
//			event.phase = state.max_phase;
//		    event.phase = state.time;
//		    event.res_id = resid;
//		    state.triggered=false;
//		    return true;
//	    }
//		else if (phase < state.max_phase) {
//	    	state.max_phase=phase;
//	    	state.time=time;
//	    	state.triggered=true;
//	    }
//	} else if (phase<cfg.threshold && (time-state.time) > globalcfg.holdoff) {
//		state.max_phase = phase;
//	    state.time = time;
//	    state.triggered=true;
//	}
//
//	return false;
//}

//void ingest_stream(phasesin[N_GROUPS], phasestream_t &phases) {
//	streamin_t in=phasesin.read();
//	phases.last=in.last;
//	for (int i=0;i<N_LANES;i++)
//		phases.data[i]=phase_t( (in.data>>(i*16))& 0xffff);
//}



//
//
//void photon_trigger(phasestream_t streamin[N_GROUPS], hls::stream<photonevent_t> &events_out,
//				    triggercfg_t triggercfg[N_GROUPS][N_LANES], globalcfg_t globalcfg,
//				    volatile timestamp_t *timestamp, const ap_uint<2> lane, unsigned long &dropped) {
////#pragma HLS PIPELINE II=512
//#pragma HLS INTERFACE axis register port=streamin
////#pragma HLS DATA_PACK variable=events_out.V
////#pragma HLS ARRAY_PARTITION variable=events_out complete
//#pragma HLS DATA_PACK variable=events_out.V
//#pragma HLS ARRAY_PARTITION variable=triggercfg dim=2 complete
////#pragma HLS INTERFACE s_axilite port=return bundle=control
//#pragma HLS INTERFACE s_axilite port=triggercfg bundle=control
//#pragma HLS INTERFACE s_axilite port=globalcfg bundle=control
//#pragma HLS INTERFACE s_axilite port=dropped bundle=control
//#pragma HLS INTERFACE ap_stable port=lane
//#pragma HLS INTERFACE ap_ctrl_none port=return
//
//	static resstategroup_t resstate[N_GROUPS];
//	resstategroup_t laststate;
//#pragma HLS DATA_PACK variable=resstate
//#pragma HLS DATA_PACK variable=laststate
//	static hls::stream<photonevent_t> lane_events[N_LANES];
//#pragma HLS STREAM variable=lane_events depth=64 dim=1
//#pragma HLS DATA_PACK variable=lane_event[0].V
//#pragma HLS ARRAY_PARTITION variable=lane_events complete
//
//	static ap_uint<2> next_out;
//
//	mainloop: for (int group=0;group<N_GROUPS;group++) {
//#pragma HLS PIPELINE II=512 rewind
//		phase_t phases[N_LANES];
//		resstategroup_t state_group;
//		triggercfg_t cfg[N_LANES];
//		phasestream_t in=streamin[group];
//		timestamp_t time=*timestamp;
//		photonevent_t event_out;
//
//		unpack: for (int i=0;i<N_LANES;i++) {
//			phases[i]=in.data[i];
//			cfg[i]=triggercfg[group][i];
//		}
//		resstate[group-1]=laststate;
//		state_group=resstate[group];
//
//		phasebase_t basedata[N_LANES];
//		tphasebase_t trigdata[N_LANES];
//		hls::stream<pulsedescr_t> pulsedata[N_LANES];
//		pulsedescr_t pulseout;
//
//		update_baselines(group, phases, 512,64, basedata);
//		update_triggers(group, basedata, cfg, *timestamp, trigdata);
//		analyze_pulse(group, trigdata, globalcfg.holdoff, *timestamp, pulsedata);
//
//		if (pulsedata[next_out++].read_nb(pulseout))
//			if (!events_out.write_nb(event_out)) {
//				dropped++;
//			}
//		}
//
////		triggermain: for (int i=0;i<N_LANES;i++) {
////			update_trigger_info1x(phases[i], &state_group.lane[i], cfg[i], globalcfg, time);
////			output_event_info1x(state_group.lane[i], lane_events[i], lane);
//////			photonevent_t event;
//////			event.time=state_group.lane[i].peak_time+state_group.lane[i].trigger_start;
//////			event.phase=state_group.lane[i].peak;
//////			event.baseline=state_group.lane[i].baseline;
//////			event.res_id=state_group.lane[i].res+lane*N_GROUPS*N_LANES;
//////			//event.duration=resstate->trigger_stop;
//////			if (state_group.lane[i].trigger_stop>0)
//////				events_out[i]<<event;
////
////		}
//////		output_event_info(state_group, lane_events, lane);
////
////		if (lane_events[next_out++].read_nb(event_out))
////			events_out.write_nb(event_out);
//
//		laststate=state_group;
////		pack: for (int i=0;i<N_LANES;i++) {
////			resstate[group][i]=state_group[i];
////		}
//	}
//
//}
