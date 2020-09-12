#include "defines.hpp"

typedef struct capmeta_t {
	resID_t res;
	timestamp_t trigger_start;
	phase_t baseline;
	samplendx_t n_captured;
} capmeta_t;

typedef struct lanenfo_t {
	capchanID_t next_to_cap;
	capchanID_t next_to_out;
	capchanID_t first_loading;
} lanenfo_t;


class CaptureManager {
 protected:
 private:
 public:
	phase_t phase_buffer[N_CHANNELS][MAX_CAPTURE_LENGTH];
	capmeta_t meta_buffer[N_CHANNELS];
#pragma HLS ARRAY_PARTITION cyclic factor=4 variable=meta_buffer
#pragma HLS ARRAY_PARTITION cyclic factor=4 variable=phase_buffer

	samplendx_t capture_limit;
	lanenfo_t lane_state[N_LANES];
#pragma HLS ARRAY_PARTITION complete variable=lane_state

	capchanID_t get_channel(resstate_t *state);
	bool capture(phase_t phase, capchanID_t chanID);
};


