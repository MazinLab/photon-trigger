#include "photon_trigger.hpp"
#include <iostream>
#include <fstream>
using namespace std;



void data_gen(int xfer, phasestream_t streamin[N_GROUPS]) {

    ifstream baseline_file("phase_input.txt");
    int lineno=0, group=0;
    std::string line;


    if (!baseline_file) {
        cout<<"ERROR: Can't open input data file\n";
        return;
    }

    while (std::getline(baseline_file, line)) {
		if (lineno++<xfer) continue;

	    std::stringstream ss(line);
	    double a;
	    if (ss >> a) {
	    	for (int group=0;group<N_GROUPS;group++)
	    		for (int i=0;i<N_LANES;i++)
	    			streamin[group].data[i]=phase_t(a);
	    }
//	    group++;
//	    if (group>=N_GROUPS) break;
	    break;
	}

    baseline_file.close();
}


#define N_TRANSFERS 3000
int main(){

	bool fail=true;

	phasestream_t streamin[N_GROUPS];
	triggercfg_t triggercfg[N_GROUPS][N_LANES];
	globalcfg_t globalcfg;
	timestamp_t timestamp[N_GROUPS], timeoffset=1000;

	const ap_uint<2> lane=1;
	unsigned long dropped, last_dropped=0;
	baseinoutctr_t baseline_shrink=16, baseline_grow=8;

	hls::stream<photonevent_t> events_out;
	timedelta_t holdoff=30; //30us

	for (int i=0;i<N_GROUPS;i++)
		for (int j=0;j<N_LANES;j++)
			triggercfg[i][j].threshold=-.232;


	for (int k=0;k<N_TRANSFERS;k++) {
		data_gen(k, streamin);

		for (int i=0;i<N_GROUPS;i++)  timestamp[i]=timeoffset++;

		photon_trigger(streamin, events_out, triggercfg, holdoff, baseline_shrink,
					   baseline_grow, timestamp, lane, dropped);

		if(dropped!=last_dropped) {
			cout<<"Transfer "<<k<<": Dropped "<<dropped-last_dropped<<" photons\n";
		}

	}
	while(!events_out.empty()) {
		photonevent_t event=events_out.read();
		printf("PHOT: %u %u %9.6f %9.6f\n", event.res_id, event.time.to_uint64(), event.phase.to_double(), event.baseline.to_double());
	}

	if (fail) cout <<"FAILED.\n";
	else cout<<"PASS!\n";
	return fail;

}
