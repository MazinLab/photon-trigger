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
		hls::stream<timestamp_t> &timestamp, hls::stream<photon_t> photon_fifos[N_PHASE],
		hls::stream<ap_uint<N_PHASE>> &photon_overflow){
#pragma HLS INTERFACE mode=axis port=photon_overflow
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=13500 register
#pragma HLS INTERFACE mode=axis port=instream depth=13500 register
#pragma HLS INTERFACE mode=axis port=timestamp depth=13500 register
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=photon_fifos
#pragma HLS INTERFACE mode=ap_fifo depth=4 port=photon_fifos register
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

	static bool fifo_empty[N_PHASE];
#pragma HLS ARRAY_PARTITION variable=fifo_empty type=complete

#pragma HLS PIPELINE II=1

	sincegroup_t sinces;
	photongroup_t photons;
	phasestream_t in;
	timestamp_t time;
	ap_uint<N_PHASE> trigger=0;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];
	ap_uint<N_PHASE> overflow;

	for (int i=0;i<N_PHASE;i++) overflow[i]=fifo_empty[i];
	photon_overflow.write(overflow);

	in = instream.read();
	time=timestamp.read();

	group_t group=in.user;
	group_t last_group=group_t(in.user-1);

	since_data[last_group]=since_cache;
	sinces = since_data[group];

	photon_data[last_group]=photon_cache;
	photons = photon_data[group];

	unpack_thresholds(threshoffs[group], threshs, hoffs);

	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;
		photon_noid_t photon;
		phase_t phase;
		reschan_t id;
		bool update_photon;

		id = N_PHASE*reschan_t(group.range()) + i;
		phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);

		trig=phase<threshs[i] && sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;
		photon.time=time;
		update_photon = photons.lane[i].phase>phase;// && sinces.since[i]!=0;  //is the second condition needed?

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
					fifo_empty[i]=!photon_fifos[i].write_nb(photon_out);
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

const int _N_PHASE = N_PHASE;

void photon_fifo_merger(hls::stream<photon_t> photon_fifos[N_PHASE], hls::stream<photon_t> &photons) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable = photon_fifos complete
#pragma HLS PIPELINE II = _N_PHASE
	for (int n=0;n<N_PHASE;n++) {
#pragma HLS UNROLL
		photon_t photon;
		bool read;
		read=photon_fifos[n].read_nb(photon);
		if (read) {
			photons.write(photon);
		}
	}
}
