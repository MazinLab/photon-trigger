#include <bitset>
#include <iostream>
using namespace std;
#include <assert.h>

#include "ap_int.h"
#include "ap_axi_sdata.h"
#include "hls_stream.h"
#include "ap_shift_reg.h"


#define N_PHOTON_LATENCY 5120
#define MAX_TIME_LATENCY 500 //1us ticks clocks
#define MAX_CPS 5000
#define N_RES 2048
#define N_MONITOR 8
#define N_CAPDATA 90
#define N_CAPPRE 30
#define N_PHOTON_BUFFERS 2
#define PHOTON_BUFF_N 8192

#define N_IQ 8
#define N_PHASE 4
#define N_PHASE_LOG2 2
#define N_GROUPS 256
#define N_PHASEGROUPS 512
#define N_PHASEGROUPS_LOG2 9
#define PHASE_BITS 16
#define IQ_BITS 32
#define N_MONITOR_LOG2 3
#define POSTAGE_BUFSIZE 1000
#define FLAT_PHOTON_BUFSIZE 32000

typedef ap_uint<16> timestamp_t;
typedef ap_uint<8> group256_t;
typedef ap_uint<9> group512_t;
typedef ap_uint<7> group128_t;
typedef ap_uint<12> reschan_t;
typedef ap_uint<N_PHASE_LOG2> lane_t;

#define THRESHOLD_BITS 8
#define HOLDOFF_BITS 8
#define THRESHOFF_BITS (HOLDOFF_BITS+THRESHOLD_BITS)
typedef unsigned short threshoff_t;
typedef ap_uint<THRESHOFF_BITS*N_PHASE> threshoffs_t;

typedef uint8_t interval_t;


typedef struct sincegroup_t {
	interval_t since[N_PHASE];
} sincegroup_t;


typedef ap_uint<7> pgroup256_t;


typedef group512_t group_t;

typedef short phase_t;

typedef unsigned int uint32_t;
typedef int32_t iq_t;
typedef ap_uint<IQ_BITS*N_PHASE> iqpgroup_t;
typedef ap_uint<512> uint512_t;
typedef ap_uint<256> uint256_t;
typedef ap_uint<128> uint128_t;
typedef ap_uint<512> uint512_t;



typedef ap_uint<N_PHASE*PHASE_BITS> phases_t;
typedef ap_uint<N_PHASE*THRESHOLD_BITS> thresholds_t;
typedef uint8_t threshold_t;

typedef ap_uint<13> photoncount_t;

typedef struct phaseset_t {
	phase_t phase[N_PHASE];
} phaseset_t;

typedef struct photon_t {
	timestamp_t time;
	phase_t phase;
	reschan_t id;
} photon_t;

typedef struct smallphoton_t {
	timestamp_t time;
	phase_t phase;
} smallphoton_t;

typedef struct photon_noid_t {
	timestamp_t time;
	phase_t phase;
} photon_noid_t;

typedef struct destined_photon_t {
	photon_noid_t photon;
	unsigned int offset;
} destined_photon_t;


typedef struct photongroup_t {
	photon_noid_t lane[N_PHASE];
} photongroup_t;

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


typedef ap_axiu<N_PHASE*PHASE_BITS,N_PHASEGROUPS_LOG2,0,0> phasestream_t;
typedef ap_axiu<N_PHASE*IQ_BITS,0,0,0> iqstreamnarrow_t;
typedef ap_axiu<N_IQ*IQ_BITS,8,0,0> iqstream_t;
typedef ap_axiu<IQ_BITS,N_MONITOR_LOG2,0,0> singleiqstream_t;
typedef ap_axiu<N_PHASE*PHASE_BITS,N_PHASEGROUPS_LOG2+N_PHASE,0,0> trigstream_t;


void photon_fifo_merger(hls::stream<photon_t> photon_fifos[N_PHASE], hls::stream<photon_t> &photons);

void trigger(hls::stream<phasestream_t> &instream, threshoffs_t threshoffs[N_PHASEGROUPS],
		hls::stream<trigstream_t> &outstream,
		hls::stream<timestamp_t> &timestamp, hls::stream<photon_t> photon_fifos[N_PHASE]);

void postage_filter(hls::stream<trigstream_t> &instream, hls::stream<iqstreamnarrow_t> &iniq,
		reschan_t monitor[N_MONITOR], hls::stream<singleiqstream_t> iq_out[N_MONITOR]);

void postage_maxi(hls::stream<singleiqstream_t> &postage, iq_t iq[N_MONITOR][POSTAGE_BUFSIZE][N_CAPDATA],
				  uint16_t event_count[N_MONITOR]);


void photons_maxi_id(hls::stream<photon_t> &photons, photon_t photons_out[N_PHOTON_BUFFERS][PHOTON_BUFF_N],
				  photoncount_t n_photons[N_PHOTON_BUFFERS], unsigned char &active_buffer);

void photons_maxi_structured(hls::stream<photon_t> &photons, smallphoton_t photons_out[N_PHOTON_BUFFERS][N_RES][MAX_CPS],
				  	  	  	 photoncount_t n_photons[N_PHOTON_BUFFERS][N_RES], unsigned char &active_buffer);
