#include "photon.hpp"
#include <iostream>
#include <bitset>
using namespace std;



inline phaseset_t phases2phaseset(phases_t p) {
	phaseset_t r;
	for (int j=0;j<N_PHASE;j++)
		r.phase[j]=p.range(PHASE_BITS*(j+1)-1, PHASE_BITS*j);
	return r;
}

inline phases_t phaseset2phases(phaseset_t p) {
	phases_t r;
	for (int j=0;j<N_PHASE;j++)
		r.range(PHASE_BITS*(j+1)-1, PHASE_BITS*j)=p.phase[j];
	return r;
}

//photons trigger at 10
// 3, 5,4,3
// 3, 10,10,3
// 3, 10,15,15,12,4
// 3, 10,15,12,3,.....,15,14,3

//phase = {3, 5, 4, 3xholdoff, 3, 10, 15, 15, 12, 4, 3xholdoff, 3, 10, 15, 12, 3, 15, 14, 3, 0xholdoff};
//event ={0, 1, 0, 0xholdoff, 0,  0,  0, 1,   0, 0, 3xholdoff, 0,  0,  1,  0, 0,  0,  0, 0, 0xholdoff};


bool drive_phase2photon() {

	bool fail=false;
	hls::stream<phasestream_t> phases;
	thresholds_t thresholds[N_PHASEGROUPS], thres;
	timestamp_t timestamp;
	hls::stream<timestamp_t> timestamps;
	interval_t holdoff;
	hls::stream<photon_t> photon_fifos[N_PHASE], photons_out, expected;

	holdoff=3;
	previous_t last[N_PHASEGROUPS][N_PHASE];
	thres=10;

	unsigned int phasedat[] = {3, 5, 4, //3
			0,0,0,
			3, 10, 15, 15, 12, 4, //6
			0,0,0,
			3, 10, 15, 12, 3, 15, 14, 3, //8
			0,0,0};
	bool photonevent[] ={0, 0, 0,
			0,0,0,
			0, 0, 0, 0, 1, 0,
			0,0,0,
			0, 0, 0, 1, 0, 0, 0, 1,
			0,0,0};

	const unsigned int NSAMP = sizeof(phasedat) / sizeof(int);
	//Build phase inputs and expected outputs, let timestamp just be the clock

	//Thresholds
	for (int i=0;i<N_PHASEGROUPS;i++) {
		phaseset_t x;
		for (int j=0;j<N_PHASE;j++) {
			last[i][j].phase=0;
			last[i][j].since=0;
			x.phase[j] = thres;
		}
		thresholds[i]=phaseset2phases(x);
	}

	//Phases & photons
	for (int i=0;i<NSAMP;i++) {
		timestamps.write(i);
		for (int group=0; group<512; group++) {

			reschan_t idbase;
			phasestream_t inphase;
			phaseset_t tmp;

			idbase=group*N_PHASE; //(i%N_PHASEGROUPS)*N_PHASE;

			//The phase
			for (int j=0;j<N_PHASE;j++)
				tmp.phase[j]=phasedat[i] + group/128;

			//Into the input stream
			inphase.data=phaseset2phases(tmp);
			inphase.user=group;  // i%N_PHASEGROUPS;
			inphase.last=inphase.user==N_PHASEGROUPS-1;
			phases.write(inphase);

			//"Photon" detection
			for (int j=0;j<N_PHASE;j++) {

				if (i==0) {
					last[group][j].phase=tmp.phase[j];
					continue;
				}

				phaseset_t threshs;
				bool trig_event;

				trig_event=tmp.phase[j]<last[group][j].phase && last[group][j].phase> threshs.phase[j];

				if (last[group][j].since>0)
					trig_event=false;

				threshs=phases2phaseset(thresholds[group]);

				if (j==0 && group==0){
					cout<<"Cycle "<<i<<" phase is "<<tmp.phase[j]<<" was "<<last[group][j].phase;
					cout<<". Thresh is "<<threshs.phase[j]<<". Photon expected: "<<photonevent[i];
					cout<<" Trigger event: "<<trig_event<<" Holdoff counter: "<<last[group][j].since<<endl;
					if (trig_event!=photonevent[i]){
						cout<<"predicted vs expected photon disagrement. pred:";
						cout<<photonevent[i]<<" compute:"<<trig_event<<endl;
					}
				}

				if (trig_event) {
					last[group][j].since=holdoff;
//					holdoff_ctr[group]=holdoff;
					photon_t phot;
					phot.time=i;
					phot.id=idbase+j;
					phot.phase=tmp.phase[j];
					expected.write(phot);
				} else {
//					holdoff_ctr[group] = holdoff_ctr[group]>0 ? holdoff_ctr[group]-1:0;
					last[group][j].since = last[group][j].since>0 ? last[group][j].since-1:0;
				}
				last[group][j].phase=tmp.phase[j];
			}
		}
	}
	cout<<"Calling with "<<phases.size()<<" phases"<<endl;
	//Drive
	timestamp=0;
	photon_trigger(phases, thresholds, timestamps, holdoff, photons_out);
	cout<<"Got "<<photons_out.size()<<" photons, expected "<<expected.size()<<endl;

//	for (int samp=0;samp<NSAMP;samp++) {
//		timestamp=samp;
//		phase_to_photon(phases, thresholds, timestamp, holdoff, photon_fifos);
//	}
//	for (int i=0;i<N_PHASE;i++)
//		cout<<"Lane "<<i<<" photons "<<photon_fifos[i].size()<<endl;
//	photons_to_axis(photon_fifos, photons_out);

	//Verify
	if (photons_out.size()!=expected.size()) {
		fail=true;
	}
	while (!photons_out.empty() && !expected.empty()) {
		photon_t got, expect;
		got=photons_out.read();
		expect=expected.read();
		if (expect.phase!=got.phase || expect.id!=got.id || expect.time!=got.time) {
			cout<<"Expected photon "<<got.id<<":"<<got.phase<<" to be "<<expect.id<<":"<<expect.phase<<" at time ";
			cout<<expect.time<<" was at time "<<got.time;
			cout<<" fail"<<endl;
			fail=true;
		}
	}

	return fail;
}


int main (void){

	bool fail=false;

	fail=drive_phase2photon();


	if (fail) {
		std::cout << "Test failed" << std::endl;
	} else {
		std::cout << "Test passed" << std::endl;
	}
	return(fail);
}
