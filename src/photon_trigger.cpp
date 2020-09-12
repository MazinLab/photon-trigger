#include "photon_trigger.hpp"
//#include "CaptureManager.hpp"
#include <iostream>
#include <iomanip>
using namespace std;


/*We get 4 consecutive resonator phases in each clock
//Each resonator phase stream needs to be used to
 * inform the baseline tracker
 * evaluated for a trigger start/stop
 * gather trigger metadata (triggered peak, trigger time & duration, baseline level)
 and either:
 * continue to be processed by its capture channel
 * assigned an available capture channel due to a triggerable event
 * ignored due to no active trigger or no avaiable capture channels
*/


void compbaseline(phase_t phase, basestate_t &bd, phase_t samples[BASELINE_NSAMP],
				  phase_t &baseline_out,
			      baseinoutctr_t grow_n, baseinoutctr_t shrink_n) {
#pragma HLS INLINE
	bool out_window;
	phase_t avg_input;

	out_window = hls::abs(phase-bd.baseline)>bd.win;

	if (out_window) {
		if (bd.num_out>=grow_n){
			bd.num_out=0;
			bd.win+=phasewin_t(0.06905157958122354/10);  //1/10th of a gen2 BL stddev
			//bd.win.setBits(bd.win.bits_to_uint64()+1);
	    } else
			bd.num_out++;
		avg_input=bd.baseline;
	} else {
		if (bd.num_in>=shrink_n){
			bd.num_in=0;
			bd.win-=phasewin_t(0.06905157958122354/10);  //1/10th of a gen2 BL stddev
			//bd.win.setBits(bd.win.bits_to_uint64()-1);
	    } else
			bd.num_in++;
		avg_input=phase;
	}

	phase_t accum_out=samples[bd.addr+1];
	samples[bd.addr]=avg_input;
	bd.addr++;
//	accum_in=avg_input;
	bd.accum+=avg_input-accum_out;
	bd.baseline=bd.accum/BASELINE_NSAMP;
	baseline_out=bd.accum/BASELINE_NSAMP;
}


void update_baselines(group_t group, phase_t phases[N_LANES], baseinoutctr_t grow_n, baseinoutctr_t shrink_n,
		phasebase_t out[N_LANES]) {
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=phases complete
#pragma HLS ARRAY_PARTITION variable=out complete
	static basestate_t basedb[N_GROUPS][N_LANES];
#pragma HLS DATA_PACK variable=basedb
	static phase_t basline_mem[N_GROUPS][N_LANES][BASELINE_NSAMP];
#pragma HLS ARRAY_PARTITION variable=basline_mem complete dim=2
//#pragma HLS ARRAY_PARTITION variable=basline_mem complete dim=3
#pragma HLS ARRAY_PARTITION variable=basedb complete dim=2

	static bool started=false;
	static basestate_t lastbase[N_LANES];
#pragma HLS ARRAY_PARTITION variable=lastbase complete

	storebase: for (int i=0;i<N_LANES;i++) {
		basedb[group-1][i]=lastbase[i];
	}

	baseline: for (int i=0;i<N_LANES;i++) {
//#pragma HLS DEPENDENCE variable=basline_mem inter RAW distance=512 true
		phase_t base;
		phase_t phase=phases[i];
		basestate_t db=basedb[group][i];

		if (!started) {
			db.win=phasewin_t(3*0.06905157958122354);
		}

#ifndef __SYNTHESIS__

		if (group==1 && i==0) {
			printf("BL %9.6f %9.6f %9.6f", phase.to_double(), db.baseline.to_double(), hls::abs(phase-db.baseline).to_double());
		}
#endif

		compbaseline(phase, db, basline_mem[group][i], base, grow_n, shrink_n);

#ifndef __SYNTHESIS__
		if (group==1 && i==0) {
			printf("%10.6f %9.6f %3u %3u %3u %3u\n",db.accum.to_double(), db.win.to_double(),
				   db.num_in.to_uint(), db.num_out.to_uint(), grow_n.to_uint(),shrink_n.to_uint());
		}
#endif

		lastbase[i]=db;
		out[i].phase=phase;
		out[i].baseline=base;
	}

	if (group==N_GROUPS-1) started=true;
}



void update_triggers(group_t group, phasebase_t data[N_LANES], triggercfg_t trig_cfg[N_LANES],
		timestamp_t timestamp, triggerout_t trigout[N_LANES]) {
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=data complete
#pragma HLS ARRAY_PARTITION variable=trig_cfg complete
#pragma HLS ARRAY_PARTITION variable=trigout complete
//	static trigstate_t trigdb[N_GROUPS][N_LANES];
//#pragma HLS ARRAY_PARTITION variable=trigdb complete dim=2

	trigger: for (int i=0;i<N_LANES;i++) {
		phase_t phase = data[i].phase-data[i].baseline;
		//trigger(phase, trig_cfg[i].threshold, holdoff, timestamp, trigdb[i], trigout[i]);
		trigout[i].trigger=phase < trig_cfg[i].threshold;
		trigout[i].baseline=data[i].baseline;
		trigout[i].phase=data[i].phase;
#ifndef __SYNTHESIS__
		if (group==1 && i==0) cout<<"TR "<<trigout[i].trigger<<endl;
#endif
	}
}


void pulse(tphasebase_t trigdata, pulsestate_t &peakdata, timedelta_t holdoff, timestamp_t timestamp, pulsedescr_t &event, bool &have_pulse) {
#pragma HLS INLINE
//TODO this allows the trigger stop condition to be influenced by the baseline because the trigger is a test against phse-baseline, not phase-baseline_at_start
	event.peak=peakdata.peak;
	event.start=peakdata.start;
	event.stop=peakdata.stop;
	event.peak_time=peakdata.peak_time;
	event.baseline=peakdata.baseline;

	if (!trigdata.trigger){
		if (peakdata.trigger) {
			have_pulse=true;
			peakdata.trigger=false;
		}
	} else {
		if (!peakdata.trigger) {
			if (timestamp-peakdata.start > holdoff) {
				peakdata.trigger=true;
				peakdata.peak = trigdata.phase;
				peakdata.start = timestamp;
				peakdata.peak_time = 0;
				peakdata.stop = 0;
				peakdata.baseline = trigdata.baseline;
			}
		} else if (trigdata.phase<peakdata.peak) {
			peakdata.peak = trigdata.phase;
			peakdata.peak_time=peakdata.stop;
		}
		peakdata.stop++;
	}

}

void analyze_pulse(group_t group, tphasebase_t trigdata[N_LANES], timedelta_t holdoff, timestamp_t timestamp,
		hls::stream<pulsedescr_t> out[N_LANES]) {
#pragma HLS PIPELINE II=1
#pragma HLS ARRAY_PARTITION variable=out complete
#pragma HLS ARRAY_PARTITION variable=trigdata complete
	static pulsestate_t pulsedb[N_GROUPS][N_LANES];
#pragma HLS DATA_PACK variable=pulsedb
#pragma HLS ARRAY_PARTITION variable=pulsedb complete dim=2

	static pulsestate_t lastpulse[N_LANES];
#pragma HLS ARRAY_PARTITION variable=lastpulse complete

	peak: for (int i=0;i<N_LANES;i++) {
		pulsedescr_t pulsedes;
		bool have_pulse;
		pulsedb[group-1][i]=lastpulse[i];
		lastpulse[i]=pulsedb[group][i];
#ifndef __SYNTHESIS__
bool tmp=lastpulse[i].trigger;
unsigned long long tsince = timestamp.to_uint64()-lastpulse[i].start.to_uint64();
#endif
		pulse(trigdata[i], lastpulse[i], holdoff, timestamp, pulsedes, have_pulse);
		pulsedes.group=group;

#ifndef __SYNTHESIS__
		if (group==1 && i==0)
			printf("PU %9.6f %9.6f %u %u %u %u %u %u %u\n",
					pulsedes.baseline.to_double(), pulsedes.peak.to_double(),
					pulsedes.start.to_uint64(), pulsedes.peak_time.to_uint(),
					pulsedes.stop.to_uint(), tmp, have_pulse, tsince, holdoff.to_uint());
#endif

		if (have_pulse)
			out[i].write_nb(pulsedes);
	}
}



void photon_trigger(phasestream_t streamin[N_GROUPS], hls::stream<photonevent_t> &events_out,
				    triggercfg_t triggercfg[N_GROUPS][N_LANES], timedelta_t holdoff, baseinoutctr_t baseline_shrink,
					baseinoutctr_t baseline_grow,
				    timestamp_t timestamp[N_GROUPS], const ap_uint<2> lane, unsigned long &dropped) {
#pragma HLS INTERFACE axis register port=streamin
#pragma HLS INTERFACE ap_hs port=timestamp
#pragma HLS DATA_PACK variable=events_out.V
#pragma HLS ARRAY_PARTITION variable=triggercfg dim=2 complete
#pragma HLS INTERFACE s_axilite port=triggercfg bundle=control
#pragma HLS INTERFACE s_axilite port=holdoff bundle=control
#pragma HLS INTERFACE s_axilite port=dropped bundle=control
#pragma HLS INTERFACE s_axilite port=baseline_grow bundle=control
#pragma HLS INTERFACE s_axilite port=baseline_shrink bundle=control
#pragma HLS INTERFACE ap_stable port=lane
#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE s_axilite port=return bundle=control

	static hls::stream<pulsedescr_t> pulsedata[N_LANES];
#pragma HLS STREAM variable=pulsedata depth=64 dim=1
#pragma HLS DATA_PACK variable=pulsedata[0].V
#pragma HLS ARRAY_PARTITION variable=pulsedata complete

	static ap_uint<2> next_out;

	baseinoutctr_t shrink, grow;

	mainloop: for (int group=0;group<N_GROUPS;group++) {
#pragma HLS PIPELINE II=512 rewind
		phase_t phases[N_LANES];
		triggercfg_t cfg[N_LANES];
		timestamp_t time=timestamp[group];

		if (group==0) {
			shrink=baseline_shrink, grow=baseline_grow;
		}

		unpack: for (int i=0;i<N_LANES;i++) {
			phases[i]=streamin[group].data[i];
			cfg[i]=triggercfg[group][i];
		}

		phasebase_t basedata[N_LANES];
		tphasebase_t trigdata[N_LANES];
		pulsedescr_t pulseout;

		update_baselines(group, phases, grow, shrink, basedata);
		update_triggers(group, basedata, cfg, time, trigdata);
		analyze_pulse(group, trigdata, holdoff, time, pulsedata);

		if (pulsedata[next_out].read_nb(pulseout)){
			photonevent_t event;
			event.res_id=pulseout.group*N_LANES+next_out+lane*N_LANES*N_GROUPS;
			event.time=pulseout.start+pulseout.peak_time;
			event.baseline=pulseout.baseline;
			event.phase=pulseout.peak;
			dropped+=!events_out.write_nb(event);
		}
		next_out++;
	}
}
