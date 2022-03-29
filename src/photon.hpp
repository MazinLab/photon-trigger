#include <bitset>
#include <iostream>
using namespace std;
#include <assert.h>

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"


#define N_MONITOR 10
#define N_CAPDATA 150
#define N_CAPPRE 150

// Include the Class
#include "ap_shift_reg.h"

#define N_IQ 8
#define N_PHASE 4
#define N_PHASE_LOG2 2
#define N_GROUPS 256
#define N_PHASEGROUPS 512
#define N_PHASEGROUPS_LOG2 9
#define PHASE_BITS 16
#define IQ_BITS 32

typedef ap_uint<40> timestamp_t;
//typedef ap_uint<8> group256_t;
typedef ap_uint<9> group512_t;
//typedef ap_uint<7> group128_t;
typedef ap_uint<12> reschan_t;


//typedef ap_uint<10> interval_t;
typedef unsigned short interval_t;
typedef ap_uint<7> pgroup256_t;
//typedef ap_uint<16> phase_t;

typedef group512_t group_t;
typedef unsigned short phase_t;

typedef unsigned int uint32_t;
typedef uint32_t iq_t;
typedef ap_uint<IQ_BITS*N_PHASE> iqpgroup_t;
typedef ap_uint<512> uint512_t;
typedef ap_uint<256> uint256_t;
typedef ap_uint<128> uint128_t;
typedef ap_uint<512> uint512_t;



typedef ap_uint<N_PHASE*PHASE_BITS> phases_t;
typedef phases_t thresholds_t;
typedef phase_t threshold_t;

typedef struct phaseset_t {
	phase_t phase[N_PHASE];
} phaseset_t;

typedef struct photon_t {
	timestamp_t time;
	phase_t phase;
	reschan_t id;
} photon_t;

typedef struct dataset_t {
	thresholds_t threshs;
	phases_t phases;
	group_t group;
	timestamp_t timestamp;
} dataset_t;


typedef struct datasetnotime_t {
	thresholds_t threshs;
	phases_t phases;
	group_t group;
	bool last;
} datasetnotime_t;

#define PHOTON_WID (40+16+16)

typedef ap_uint<PHOTON_WID> photon_uint_t;

inline photon_uint_t photon2uint(photon_t x) {
	photon_uint_t r;
	r.range(15,0)=x.phase;
	r.range(31, 16)=x.id;
	r.range(40+16+16-1, 32)=x.time;
	return r;
}

inline photon_t uint2photon(photon_uint_t r) {
	photon_t x;
	x.phase=r.range(15,0);
	x.id=r.range(31, 16);
	x.time=r.range(40+16+16-1, 32);
	return x;
}

typedef struct previous_t {
	interval_t since;
	phase_t phase;
} previous_t;

typedef struct previousgroup_t {
	previous_t data[N_PHASE];
} previousgroup_t;

typedef ap_axiu<N_PHASE*PHASE_BITS,9,0,0> phasestream_t;
typedef ap_axiu<N_PHASE*IQ_BITS,9,0,0> iqstream_t;

typedef ap_axiu<N_PHASE*PHASE_BITS,9+N_PHASE,0,0> trigstream_t;

typedef ap_axiu<N_PHASE*(PHASE_BITS+IQ_BITS),9+N_PHASE,0,0> trigstreamiq_t;


void phase_to_photon(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
		volatile timestamp_t &timestamp, interval_t holdoff, hls::stream<photon_t> photon_fifo[N_PHASE]);

void photons_to_axis(hls::stream<photon_t> photon_fifo[N_PHASE], hls::stream<photon_t> &photons);
void photon_trigger(hls::stream<phasestream_t> &instream, thresholds_t thresholds[N_PHASEGROUPS],
					hls::stream<timestamp_t> &timestamp, interval_t holdoff, hls::stream<photon_t> &photons);

