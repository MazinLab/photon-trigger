#include "ap_int.h"
#include "ap_fixed.h"
#ifndef __photon_trigger_types_h__
#define __photon_trigger_types_h__

#define N_GROUPS 512
#define N_LANES 4
#define N_CHANNELS 8192
#define MAX_CAPTURE_LENGTH 120

typedef unsigned char lane_t;
typedef unsigned short capchanID_t;
typedef unsigned char samplendx_t;

typedef ap_fixed<32, 17, AP_RND_CONV, AP_WRAP>  phaseaccum_t;
typedef ap_uint<9> group_t;
typedef ap_uint<16> resID_t;
typedef ap_uint<64> timestamp_t;
typedef ap_fixed<16, 1, AP_RND_CONV, AP_WRAP> phase_t;
typedef ap_ufixed<16, 0, AP_RND_CONV, AP_SAT> phasewin_t;

typedef struct photonevent_t {
	timestamp_t time;
	phase_t phase;
	resID_t res_id;
	phase_t baseline;
} photonevent_t;

typedef ap_uint<64+9+16> eventuint_t;
typedef struct {
	eventuint_t data;
} eventstream_t;

typedef struct {
	phase_t data[N_LANES];
	bool last;
} phasestream_t;


typedef struct streamout_t {
	ap_uint<64+16*3> data;
	bool last;
} streamout_t;

typedef struct triggercfg_t {
	phase_t threshold;
} triggercfg_t;

typedef struct globalcfg_t {
	unsigned int holdoff;
} globalcfg_t;

typedef struct {
	phase_t max_phase;
	timestamp_t time;
	bool triggered;
} triggerstat_t;

typedef ap_uint<10> timedelta_t;

typedef struct resstate_t {
	phase_t peak;
	resID_t res;
	timestamp_t trigger_start;
	timedelta_t peak_time;
	timedelta_t trigger_stop;
	phase_t baseline;
	bool triggered;
	//capchanID_t capture_channel;
} resstate_t;


typedef struct pulsestate_t {
	phase_t peak;
	timestamp_t start;
	timedelta_t peak_time;
	timedelta_t stop;
	phase_t baseline;
	bool trigger;
} pulsestate_t;

typedef struct pulseout_t {
	phase_t peak;
	group_t group;
	timestamp_t start;
	timedelta_t peak_time;
	timedelta_t stop;
	phase_t baseline;
} pulsedescr_t;


#define BASELINE_NSAMP 64  //pwr of 2

typedef ap_uint<9> baseinoutctr_t;
typedef ap_uint<6> baseaddr_t;

typedef struct basestate_t {
	phase_t baseline;
	phasewin_t win;
	baseaddr_t addr;  // must match BASELINE_NSAMP
	phaseaccum_t accum;
	baseinoutctr_t num_in;
	baseinoutctr_t num_out;
} basestate_t;

typedef struct trigstate_t {
	phase_t peak;
	timestamp_t start;
	timedelta_t peak_time;
	timedelta_t stop;
	bool triggered;
} trigstate_t;

typedef struct phasebase_t {
	phase_t phase;
	phase_t baseline;
} phasebase_t;

typedef struct tphasebase_t {
	phase_t phase;
	phase_t baseline;
	bool trigger;
} triggerout_t;

typedef unsigned char holdoff_t;  //MAX_VAL/1MHz (bin sample rate) = holdofftime

typedef struct resstategroup_t {
	resstate_t lane[N_LANES];
} resstategroup_t;

#endif
