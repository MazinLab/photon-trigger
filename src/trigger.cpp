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
		threshs[i]=to&0xffff;
		holdsoffs[i]=to>>THRESHOLD_BITS;
	}
}

void trigger(hls::stream<phasestream_t> &instream, threshoffs_t threshoffs[N_PHASEGROUPS],
		hls::stream<trigstream_t> &outstream,
		hls::stream<timestamp_t> &timestamp, hls::stream<photon_t> photon_fifos[N_PHASE]){
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=axis port=outstream depth=32000 register
#pragma HLS INTERFACE mode=axis port=instream depth=32000 register
#pragma HLS INTERFACE mode=axis port=timestamp depth=32000 register
#pragma HLS INTERFACE mode=axis depth=4 port=photon_fifos register
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



//while (!instream.empty()){
#pragma HLS PIPELINE II=1

	sincegroup_t sinces;
	photongroup_t photons;
	phasestream_t in;
	timestamp_t time;
	ap_uint<N_PHASE> trigger;
	threshold_t threshs[N_PHASE];
	interval_t hoffs[N_PHASE];
	ap_uint<N_PHASE> overflow;


	in = instream.read();
	time=timestamp.read();

	group_t group=in.user;
	group_t last_group=group_t(in.user-1);

	since_data[last_group]=since_cache;
	sinces = since_data[group];

	photon_data[last_group]=photon_cache;
	photons = photon_data[group];

	unpack_thresholds(threshoffs[group], threshs, hoffs);
	trigger=0;
	lanes: for (int i=0;i<N_PHASE;i++) {
		#pragma HLS UNROLL
		bool trig;
		photon_noid_t photon;
		phase_t phase;
		reschan_t id;
		bool update_photon;
		photon_t photon_out;
		bool emit;

		id = N_PHASE*reschan_t(group.range()) + i;
		phase=in.data.range(PHASE_BITS*(i+1)-1,PHASE_BITS*i);

		ap_fixed<16,1> phase_signed;
		ap_fixed<8,1> thresh_signed;
		phase_signed.range()=(uint16_t) phase;
		thresh_signed.range()=(uint8_t) threshs[i];

		//trig=phase<threshs[i] && sinces.since[i]==0;
		trig=phase_signed<thresh_signed && sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;
		photon.time=time;
		update_photon = photons.lane[i].phase>phase;

//#ifndef __SYNTHESIS__
//		if (group==1 && i<3) {
//			cout<<"Phase "<<phase_signed<<" Thresh: "<<thresh_signed<<" Since:"<<(uint16_t)sinces.since[i];
//			cout<<" id:"<<id<<" time: "<<time<<" Trig: "<<trig<<endl;
//		}
//#endif

		emit=!trig && sinces.since[i]==1;

		if (trig) {
			sinces.since[i]=hoffs[i];
			photons.lane[i]=photon;
		} else {
			if (sinces.since[i]>0) {
				if (update_photon)  photons.lane[i]=photon;
				if (sinces.since[i]==1) {
					photon_out.id=id;
					photon_out.phase=photons.lane[i].phase;
					photon_out.time=photons.lane[i].time;
//#ifndef __SYNTHESIS__
//		if (group==0 && i==0) {
//			cout<<" Photon out: "<<photon_out.phase;
//		}
//#endif
				}
				sinces.since[i]--;
			}
		}

		if (emit) photon_fifos[i].write(photon_out);
	}

	photon_cache=photons;
	since_cache=sinces;


	trigstream_t out;
	out.user.range(N_PHASEGROUPS_LOG2-1, 0)=in.user;
	out.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;
	out.last=in.last;
	out.data=in.data;
//#ifndef __SYNTHESIS__
//		if (group==1) {
////			cout<<" Trigger out: "<<trigger<<endl;
//			cout<<" User out: "<<out.user<<endl;
//		}
//#endif
	outstream.write(out);
//}
}

const int _N_PHASE = N_PHASE;

void photon_fifo_merger(hls::stream<photon_t> photon_fifos[N_PHASE], hls::stream<photon_t> &photons) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable = photon_fifos complete
#pragma HLS PIPELINE II = _N_PHASE
#pragma HLS INTERFACE mode=axis port=photons depth=_N_PHASE register
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
