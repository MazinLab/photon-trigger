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

void trigger(hls::stream<phasestream_t> &instream, threshoffs_t threshoffs[N_PHASEGROUPS],  hls::stream<trigstream_t> &outstream,
		hls::stream<timestamp_t> &timestamp, hls::stream<photon_t> photon_fifos[N_PHASE]){
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500 register
#pragma HLS INTERFACE mode=axis port=instream depth=13500 register
#pragma HLS INTERFACE mode=axis port=timestamp depth=13500 register
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos
#pragma HLS INTERFACE mode=ap_fifo depth=13500 port=photon_fifos register
#pragma HLS INTERFACE mode=s_axilite port=threshoffs bundle=control

	static sincegroup_t since_data[N_PHASEGROUPS], since_cache;
	static photongroup_t photon_data[N_PHASEGROUPS], photon_cache;
#pragma HLS DEPENDENCE variable=since_data intra WAR false
#pragma HLS DEPENDENCE variable=since_data intra RAW false
#pragma HLS DEPENDENCE variable=since_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=since_data

#pragma HLS DEPENDENCE variable=photon_data intra WAR false
#pragma HLS DEPENDENCE variable=photon_data intra RAW false
#pragma HLS DEPENDENCE variable=photon_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=photon_data


	#pragma HLS PIPELINE II=1 //rewind
	sincegroup_t sinces;
	photongroup_t photons;
	phasestream_t in;
	timestamp_t time;
	ap_uint<N_PHASE> trigger=0;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];

	in = instream.read();
	time=timestamp.read();
	since_data[group_t(in.user-1)]=since_cache;
	sinces = since_data[in.user];

	group_t group=in.user;
	photon_data[group_t(group-1)]=photon_cache;
	photons = photon_data[group];

	unpack_thresholds(threshoffs[in.user], threshs, hoffs);

	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;
		photon_noid_t photon;

		phase_t phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);


		trig=phase<threshs[i] && sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;
		photon.time=time;
		bool update_photon  = photons.lane[i].phase<phase && sinces.since[i]!=0;
		reschan_t id = N_PHASE*reschan_t(group.range()) + i;

		if (trig) {
			sinces.since[i]=hoffs[i];
			photons.lane[i]=photon;
		} else {
			if (sinces.since[i]>0) {
				if (update_photon)  photons.lane[i]=photon;
				if (sinces.since[i]==1) {
					photon_t photon_out;
					photon_out.id=id;
					photon_out.phase=photons.lane[i].phase;
					photon_out.time=photons.lane[i].time;
					photon_fifos[i].write_nb(photon_out);
				}
				sinces.since[i]--;
			}
		}

	}

	photon_cache=photons;
	since_cache=sinces;

	trigstream_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=in.user;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=in.last;
	out.data=in.data;
	outstream.write(out);

}
