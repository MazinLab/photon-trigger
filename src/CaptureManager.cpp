#include "CaptureManager.hpp"

capchanID_t CaptureManager::get_channel(resstate_t *state) {
	phase_t baseline;
	capmeta_t meta;
	lanenfo_t lanenfo;
	capchanID_t chan;
	lane_t lane;

	//Determine the channel to use for this resid
	// channels for consecutive resonators much always be writable in the same clock
	lane=state->res % N_LANES;
	lanenfo = lane_state[lane];
	if (lanenfo.next_to_cap==0) { // we need to set things up
		chan = lane + 1;
		lanenfo.next_to_cap = chan+1;
		lanenfo.first_loading = chan;
		lanenfo.next_to_out = chan;
	}
	else {
		if (lanenfo.next_to_cap==lanenfo.next_to_out)
			chan=0;
		else {
			chan=lanenfo.next_to_cap;
			//handle rollover
			capchanID_t last_chan_for_lane = N_CHANNELS-N_LANES+1+lane;
			if (lanenfo.next_to_cap==last_chan_for_lane)
				lanenfo.next_to_cap=lane + 1;
			else
				lanenfo.next_to_cap+=N_LANES;
		}
	}


	if (chan!=0) {
		meta.baseline=state->baseline;
		meta.res=state->res;
		meta.trigger_start=state->trigger_start;
		meta.n_captured=0;
		meta_buffer[chan-1]=meta;
		lane_state[lane]=lanenfo;
	}

	return chan;
}

bool CaptureManager::capture(phase_t phase, capchanID_t chanID) {
//return true if capture complete
	if (chanID==0)
		return false;

	samplendx_t capndx;
	bool complete;

	capndx = meta_buffer[chanID-1].n_captured;


	if (capndx==MAX_CAPTURE_LENGTH-1)
		return false;

	complete = capndx==MAX_CAPTURE_LENGTH-2;

	phase_buffer[chanID-1][capndx] = phase;
	meta_buffer[chanID-1].n_captured++;

	if (complete) {
		lane_t lane = chanID%N_LANES;
		capchanID_t last_chan_for_lane = N_CHANNELS-N_LANES+1+lane;
		if (lane_state[lane].first_loading==last_chan_for_lane)
			lane_state[lane].first_loading=lane + 1;
		else
			lane_state[lane].first_loading+=N_LANES;
	}

	return complete;
}
