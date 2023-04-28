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


void photon_packetizer(hls::stream<photon_t> &photons, ap_uint<10> photons_per_packet,// timestamp_t packet_duration,
		hls::stream<photonstream_t> &photon_packets) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS PIPELINE II = 1
#pragma HLS INTERFACE mode=axis port=photons depth=100 register
#pragma HLS INTERFACE mode=axis port=photon_packets depth=100 register
#pragma HLS INTERFACE mode=s_axilite port=photons_per_packet
//#pragma HLS INTERFACE mode=s_axilite port=packet_duration

	static ap_uint<10> count=0;
	static timestamp_t _last_time;
	static bool seen_a_photon=false;

	photon_t photon;
	timestamp_t tmp_time;
	photonstream_t beat;
	ap_uint<48> packed_photon;


	photon = photons.read();
	packed_photon.range(15,0)=photon.time;
	packed_photon.range(31,16)=photon.phase;
	packed_photon.range(47,32)=photon.id;

	tmp_time=photon.time>>9;

	beat.data=packed_photon;
	beat.last=(count==photons_per_packet || tmp_time!=_last_time) && seen_a_photon;
	beat.dest=0;

	_last_time=tmp_time;

	photon_packets.write(beat);
	if (beat.last) count=0;
	else count++;
	seen_a_photon=true;


}
