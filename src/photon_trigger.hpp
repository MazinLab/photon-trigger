#ifndef __photon_trigger_h__
#define __photon_trigger_h__

#include "defines.hpp"
#include "hls_stream.h"
#include "hls_math.h"
#include <complex>
#include <math.h>

void photon_trigger(phasestream_t streamin[N_GROUPS], hls::stream<photonevent_t> &events_out,
				    triggercfg_t triggercfg[N_GROUPS][N_LANES], timedelta_t holdoff, baseinoutctr_t baseline_shrink,
					baseinoutctr_t baseline_grow,
				    timestamp_t timestamp[N_GROUPS], const ap_uint<2> lane, unsigned long &dropped);

#endif
