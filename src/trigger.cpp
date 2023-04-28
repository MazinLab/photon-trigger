#include "photon.hpp"


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
	static photongroupival_t photon_data[N_PHASEGROUPS], photon_cache;
#pragma HLS DEPENDENCE variable=since_data intra WAR false
#pragma HLS DEPENDENCE variable=since_data intra RAW false
#pragma HLS DEPENDENCE variable=since_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=since_data

#pragma HLS DEPENDENCE variable=photon_data intra WAR false
#pragma HLS DEPENDENCE variable=photon_data intra RAW false
#pragma HLS DEPENDENCE variable=photon_data inter RAW distance=512 true
#pragma HLS AGGREGATE compact=auto variable=photon_data
#pragma HLS AGGREGATE compact=auto variable=photon_fifos



//while (!instream.empty()){
#pragma HLS PIPELINE II=1

	sincegroup_t sinces;
	photongroupival_t photons;
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
		photon_noid_interval_t photon;
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

		trig=phase_signed<thresh_signed && sinces.since[i]==0;
		trigger[i]=trig;

		photon.phase=phase;
//		photon.time=time;
		update_photon = photons.lane[i].phase>phase;


		emit=!trig && sinces.since[i]==1;

		if (trig) {
			sinces.since[i]=hoffs[i];
			photons.lane[i].phase=photon.phase;
			photons.lane[i].time=sinces.since[i];
		} else {
			if (sinces.since[i]>0) {
				if (update_photon)  {
					photons.lane[i].phase=photon.phase;
					photons.lane[i].time=sinces.since[i];
				}
				if (sinces.since[i]==1) {
					photon_out.id=id;
					photon_out.phase=photons.lane[i].phase;
					photon_out.time=time-photons.lane[i].time;
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

	outstream.write(out);
//}
}


void photon_packetizer(hls::stream<photon_t> &photons, ap_uint<10> photons_per_packet, ap_uint<5> time_shift,
		hls::stream<photonstream_t> &photon_packets) {
#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS INTERFACE mode=s_axilite port=time_shift
#pragma HLS PIPELINE II = 1
#pragma HLS INTERFACE mode=axis port=photons depth=100 register
#pragma HLS INTERFACE mode=axis port=photon_packets depth=100 register
#pragma HLS INTERFACE mode=s_axilite port=photons_per_packet
#pragma HLS AGGREGATE compact=auto variable=photons
//#pragma HLS INTERFACE mode=s_axilite port=packet_duration

	static ap_uint<10> count=0;
	static timestamp_t _last_time;
	static bool seen_a_photon=false;

	photon_t photon;
	timestamp_t tmp_time;
	photonstream_t beat;
	ap_uint<48> packed_photon;


	photon = photons.read();
	packed_photon.range(15,0)=photon.phase;
	packed_photon.range(16+12,16)=photon.id;
	packed_photon.range(63,16+12)=photon.time;

	ap_uint<5> shft;
	shft = time_shift;
	shft = shft < 9 ? ap_uint<5>(9): shft;
	shft = shft > 20 ? ap_uint<5>(20): shft;
	tmp_time=photon.time>>shft;

	beat.data=packed_photon;
	beat.last=(count==photons_per_packet || tmp_time!=_last_time) && seen_a_photon;
	beat.dest=0;

	_last_time=tmp_time;

	photon_packets.write(beat);
	if (beat.last) count=0;
	else count++;
	seen_a_photon=true;

}


void photon_maxi(hls::stream<photon_t> &photons, photon_uint_2x_t photons_out[N_PHOTON_BUFFERS][FLAT_PHOTON_BUFSIZE/2],
				  photoncount_t n_photons[N_PHOTON_BUFFERS], unsigned char &active_buffer,
				  photoncount_t photons_per_buf, ap_uint<5> time_shift) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=photons depth=64 register
#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave max_write_burst_length=256
#pragma HLS INTERFACE mode=s_axilite port=active_buffer
#pragma HLS INTERFACE mode=s_axilite port=n_photons
#pragma HLS INTERFACE mode=s_axilite port=photons_per_buf
#pragma HLS INTERFACE mode=s_axilite port=time_shift
#pragma HLS ARRAY_PARTITION variable=photons_out type=complete
#pragma HLS DEPENDENCE direction=WAW type=inter variable=photons_out distance=8000 true
#pragma HLS DEPENDENCE direction=WAR type=inter variable=photons_out false
#pragma HLS DEPENDENCE direction=RAW type=inter variable=photons_out false
#pragma HLS AGGREGATE compact=auto variable=photons

	static ap_uint<N_PHOTON_BUFFERS_LOG2> _ab;
	photoncount_t _n_photons;

	photon_t burstcache[512];
#pragma HLS AGGREGATE compact=auto variable=burstcache
	uint16_t cache_i;
	timestamp_t _last_time;
	bool rotate_buffer=false;
	photoncount_t _photons_per_buf=photons_per_buf;
	bool seen_a_photon=false;

	buffer: while (!rotate_buffer) {

		photon_t photon;
		timestamp_t tmp_time;

		active_buffer=_ab;

		photon = photons.read();
		burstcache[cache_i] = photon;

		if (cache_i==511) {
			burst512: for (int i=0;i<256;i++) {
				photon_uint_2x_t x;
				x.range(N_PHOTON_BITS-1,0)=photon2uint(burstcache[i*2]);
				x.range(2*N_PHOTON_BITS-1,0)=photon2uint(burstcache[i*2+1]);
				photons_out[_ab][_n_photons/2+i]=x;
			}
			cache_i=0;
			_n_photons+=512;
		} else
			cache_i++;


		ap_uint<5> shft;
		shft = time_shift;
		shft = shft < 9 ? ap_uint<5>(9): shft;
		shft = shft > 20 ? ap_uint<5>(20): shft;
		tmp_time=photon.time>>shft;

		rotate_buffer=(_n_photons>=_photons_per_buf || tmp_time!=_last_time) && seen_a_photon;

		_last_time=tmp_time;
		seen_a_photon=true;
		n_photons[_ab]=_n_photons;
	}

	burstvar: for (int i=0;i<cache_i/2;i++) {
		photon_uint_2x_t x;
		x.range(N_PHOTON_BITS-1,0)=photon2uint(burstcache[i*2]);
		x.range(2*N_PHOTON_BITS-1,0)=photon2uint(burstcache[i*2+1]);
		photons_out[_ab][_n_photons/2+i]=x;
	}
	_n_photons+=cache_i;
	n_photons[_ab]=_n_photons;
	cache_i=0;
	_ab++;
	n_photons[_ab]=0;

	#ifndef __SYNTHESIS__
	cout<<" rotate buffer "<<_n_photons<<", "<<_elapsed<<endl;
	#endif

}

