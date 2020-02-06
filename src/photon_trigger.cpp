#include "photon_trigger.hpp"
#include <iostream>
using namespace std;

void photon_detect(phasestream_t *streamin, hls::stream<photon_t> &photons,
				   phase_t threshold[TDM_FACTOR], volatile timestamp_t *timestamp,
				   bool &dropped, const ap_uint<4> lane) {
#pragma HLS PIPELINE  II=1
#pragma HLS DATA_PACK variable=photons
#pragma HLS INTERFACE axis port=streamin
#pragma HLS INTERFACE ap_fifo port=photons
//#pragma HLS INTERFACE s_axilite port=return bundle=control

	static group_t group;
	static phase_t last_phase[TDM_FACTOR], last_phase_cache;
	static ap_uint<2> n_pos_dpdt[TDM_FACTOR];
	static ap_uint<2> last_dpdt;

	phasestream_t phase;
	ap_uint<2> n_pos_deriv;
	phase_t last, thresh;
	bool trigger;

	thresh=threshold[group];
	phase=*streamin;
	n_pos_deriv = n_pos_dpdt[group];
	last=last_phase[group];
	last_phase[group-1]=last_phase_cache;
	last_phase_cache=phase.data;

	if (phase.data>last) {
		trigger = n_pos_deriv>2 && phase.data>thresh;
		last_dpdt=n_pos_deriv+1;
	} else {
		trigger=false;
		last_dpdt=0;
	}

	n_pos_dpdt[group-1]=last_dpdt;

	if (trigger) {
		photon_t temp;
		temp.phase=phase.data;
		temp.time=*timestamp;
		temp.res=lane<<8 + group;
		dropped=photons.write_nb(temp);
	}
	group++;

}
