#ifndef __fine_channelizer_h__
#define __fine_channelizer_h__

#include "ap_int.h"
#include "hls_stream.h"
#include "ap_fixed.h"
#include <complex>
#include "hls_dsp.h"
#include <math.h>
#include "ap_shift_reg.h"

#define TDM_FACTOR 256

typedef ap_uint<8> group_t;
typedef ap_uint<16> resID_t;
typedef ap_uint<32> timestamp_t;
typedef ap_fixed<18, 1, AP_RND_CONV, AP_SAT_SYM> phase_t;



typedef struct {
	timestamp_t time;
	phase_t phase;
	resID_t res;
} photon_t;

typedef struct {
	phase_t data;
	ap_uint<1> last;
} phasestream_t;

void photon_detect(phasestream_t *streamin, hls::stream<photon_t> &photons,
				   phase_t threshold[TDM_FACTOR], volatile timestamp_t *timestamp);
#endif
