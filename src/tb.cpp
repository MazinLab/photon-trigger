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


bool verify_trigger(hls::stream<trigstream_t> &out, hls::stream<trigstream_t> &expected) {
	trigstream_t got, expect;
	bool fail=false;
	cout<<"Stream has "<<out.size()<<", expected "<<expected.size()<<endl;

	fail|=out.size()!=expected.size();

	int i=0;
	while (!out.empty() && !expected.empty()) {

		got=out.read();
		expect=expected.read();
		if (expect.data!=got.data || expect.user!=got.user || expect.last!=got.last)
			cout<<"Cycle "<<i<<": phase0 expected="<<expect.data.range(15,0)<<", got "<<got.data.range(15,0)<<endl;

		if (expect.data!=got.data ) {
			cout<<" data mismatch "<<got.data<<" is not "<<expect.data<<endl;
			fail=true;
		}

		if (expect.user!=got.user ) {
			int a, b, c, d;
			a=got.user.range(N_PHASEGROUPS_LOG2-1,0);
			b=got.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);
			c=expect.user.range(N_PHASEGROUPS_LOG2-1,0);
			d=expect.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2);

			cout<<" user mismatch, (group, trig):"<<a<<", "<<b<<" is not "<<c<<", "<<d<<endl;
			fail=true;
		}

		if (expect.last!=got.last) {
			cout<<" last mismatch "<<got.last<<" is not "<<expect.last<<endl;
			fail=true;
		}
		i++;
	}
	if (!fail) cout<<"TRIGGER PASSED!"<<endl;
	return fail;
}



bool verify_photons(hls::stream<photon_t> &out, hls::stream<photon_t> &expected) {
	photon_t got, expect;
	bool fail=false;
	cout<<"Stream has "<<out.size()<<", expected "<<expected.size()<<endl;

	fail|=out.size()!=expected.size();

	int i=0;
	while (!out.empty() && !expected.empty()) {

		got=out.read();
		expect=expected.read();
		if (expect.id!=got.id || expect.phase!=got.phase || expect.time!=got.time)
			cout<<"Photon "<<i<<": rid="<<expect.id<<endl;

		if (expect.id!=got.id ) {
			cout<<" id mismatch "<<got.id<<" is not "<<expect.id<<endl;
			fail=true;
		}

		if (expect.phase!=got.phase ) {
			cout<<" phase mismatch:"<<got.phase<<" is not "<<expect.phase<<endl;
			fail=true;
		}

		if (expect.time!=got.time) {
			cout<<" time mismatch "<<got.time<<" is not "<<expect.time<<endl;
			fail=true;
		}
		i++;
	}
	return fail;
}



bool verify_postage(hls::stream<singleiqstream_t> out[N_MONITOR], hls::stream<singleiqstream_t> expected[N_MONITOR]) {
	singleiqstream_t got, expect;
	bool fail=false;


	for (int j=0;j<N_MONITOR;j++) {
		cout<<"Monitor "<<j<<":"<<endl;
		cout<<" Stream was "<<out[j].size()<<", expected "<<expected[j].size()<<endl;
		fail|=out[j].size()!=expected[j].size();

		int i=0;
		while (!out[j].empty() && !expected[j].empty()) {

			got=out[j].read();
			expect=expected[j].read();
			cout<<" Cycle "<<i<<":"<<endl;

			if (expect.last!=got.last ) {
				cout<<"  last mismatch "<<got.last<<" is not "<<expect.last<<endl;
				fail=true;
			}

			if (expect.data!=got.data ) {
				cout<<"  data mismatch:"<<got.data<<" is not "<<expect.data<<endl;
				fail=true;
			}

			if (expect.user!=got.user) {
				cout<<"  user mismatch "<<got.user<<" is not "<<expect.user<<endl;
				fail=true;
			}
			i++;
		}
	}
	return fail;
}

bool drive_phase2photon() {


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

	const unsigned int N_SAMP = sizeof(phasedat) / sizeof(int);
	//Build phase inputs and expected outputs, let timestamp just be the clock

	//trigger needs phases for all resonators with photon events placed on specific resonators at specific points
	//Threshold level
	// holdoff time
	// verify only phase > thershold triggers even and only if holdoff after previous
	// trigger should come on phase of/immediately after peak  //todo which

	//photon, needs trigger stream and timestamps
	// should emit photons in resonator order with resonator, phase of peak, and
	bool fail=false;
	hls::stream<phasestream_t> phases("Phase data");
	hls::stream<iqstreamnarrow_t> iqs("IQ data");
	hls::stream<timestamp_t> timestamps("Timestamps");

	hls::stream<trigstream_t> trigger_out("Trig out"), trigger_gold("Trig gold"), trigger_gold2("Trig gold2"), trigger_gold3("Trig gold3");
	hls::stream<photon_t> photons_out("Photons out"), photons_gold("Photon gold");
	hls::stream<singleiqstream_t> postage_out[N_MONITOR], postage_gold[N_MONITOR];

	thresholds_t thresholds[N_PHASEGROUPS];
	reschan_t monitor[N_MONITOR]={0,1,2,3,4,6,1025,2047};
	int alignment;
	interval_t holdoff;
	threshold_t thres;


//	unsigned int phase_data[N_SAMP][N_PHASEGROUPS][N_PHASE];
//	unsigned int iq_data[N_SAMP+N_CAPDATA][N_PHASEGROUPS][N_PHASE];

	holdoff=3;
	previous_t last[N_PHASEGROUPS][N_PHASE];
	thres=10;
	alignment=3;

	//Load Thresholds
	for (int i=0;i<N_PHASEGROUPS;i++) {
		phaseset_t x;
		for (int j=0;j<N_PHASE;j++)
			x.phase[j] = thres;
		thresholds[i]=phaseset2phases(x);
	}

	//Load IQs
	for (int i=0;i<(N_SAMP+N_CAPDATA)*N_PHASEGROUPS*N_PHASE;i++) {
		iqstreamnarrow_t in;
		int group, lane;
		group=(i/N_PHASE)%N_PHASEGROUPS;
		lane=i%N_PHASE;
		in.data.range(IQ_BITS*(lane+1)-1,IQ_BITS*lane)=i;
		in.last=group==N_PHASEGROUPS-1;
		iqs.write(in);
	}

	//Load Timestampss
	for (int i=0;i<N_SAMP*N_PHASEGROUPS;i++) {
		timestamps.write(i);
	}


	//Zero cache data
	for (int i=0;i<N_PHASEGROUPS;i++) {
		for (int j=0;j<N_PHASE;j++) {
			last[i][j].phase=0;
			last[i][j].since=0;
		}
	}

	//Phase data and trigger events
	phaseset_t tmp;
	ap_uint<N_PHASE> trigv;
	unsigned short last_phase[N_PHASEGROUPS][N_PHASE];
	short holdoffctr[N_PHASEGROUPS][N_PHASE];
	phases_t inphase_previous[N_PHASEGROUPS];
	for (int i=0;i<N_PHASEGROUPS;i++) {
		for (int j=0;j<N_PHASE;j++) {
			last_phase[i][j]=0;
			holdoffctr[i][j]=-1000;
		}
		inphase_previous[i]=0;
	}
	for (int i=0;i<N_SAMP*N_PHASEGROUPS*N_PHASE;i++) {
		iqstreamnarrow_t in;
		phase_t phase;
		threshold_t thresh;
		int group, lane, rid, beat, samp;
		group=(i/N_PHASE)%N_PHASEGROUPS;
		lane=i%N_PHASE;
		rid=i%(N_PHASEGROUPS*N_PHASE);
		beat=i/N_PHASE;
		samp=i/N_PHASEGROUPS/N_PHASE;



		thresh=phases2phaseset(thresholds[group]).phase[lane];
		phase=phasedat[samp];

		tmp.phase[lane]=phase;

		trigv[lane]=last_phase[group][lane]>thresh && last_phase[group][lane]>phase && holdoffctr[group][lane]<=0;

		photon_t phot;
		phot.id=rid;
		phot.time=beat;
		phot.phase=phasedat[samp-1];



		if (lane==N_PHASE-1) {
			phasestream_t inphase;
			inphase.data=phaseset2phases(tmp);
			inphase.user=group;
			inphase.last=inphase.user==N_PHASEGROUPS-1;
			phases.write(inphase);

			trigstream_t trigtmp;
			trigtmp.data=inphase_previous[group];
			trigtmp.user=group;
			trigtmp.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1,N_PHASEGROUPS_LOG2)=trigv;
			trigtmp.last=inphase.last;
			trigger_gold.write(trigtmp);
			trigger_gold2.write(trigtmp);
			trigger_gold3.write(trigtmp);

			inphase_previous[group]=inphase.data;
		}

		if (lane==0 && (group==0||group==1)||(group==511 &&lane==3) || (group==0)){
			if (trigv[lane]) {
			cout<<"Cycle "<<beat<<" rid="<<rid<<" phase is "<<phase<<" was "<<last_phase[group][lane];
			cout<<". Thresh is "<<thresh;
			cout<<" Trigger event: "<<trigv[lane]<<" Holdoff counter: "<<holdoffctr[group][lane]<<endl;


				cout<<"  EXPECT Photon: rid="<<phot.id<<" "<<phot.phase<<" at "<<phot.time<<endl;
			}
		}

		if (trigv[lane]) {
			photons_gold.write(phot);
			for (int mon=0; mon<N_MONITOR;mon++) {
				if (rid==monitor[mon] && holdoffctr[group][lane]<-(N_CAPDATA-holdoff)) {
					if (rid==0) cout<<"  EXPECT IQ Packet: ";
					for (int i_cap=-N_CAPPRE;i_cap<N_CAPDATA-N_CAPPRE;i_cap++) {
						singleiqstream_t tmp;
						tmp.data=i+i_cap*2048+alignment;
						tmp.last=i_cap==(N_CAPDATA-N_CAPPRE-1);
						tmp.user=mon;
						if (rid==0 && i_cap==-N_CAPPRE) cout<<i+i_cap*2048+alignment<<" to ";
						if (rid==0 && i_cap==N_CAPDATA-N_CAPPRE-1) cout<<i+i_cap*2048+alignment;
						postage_gold[mon].write(tmp);
					}
					if (rid==0) cout<<endl;
				}
			}
		}
		if (trigv[lane])
			holdoffctr[group][lane]=holdoff;
		else
			holdoffctr[group][lane]--;
		last_phase[group][lane]=phase;
	}



	//Drive
	cout<<"Calling trigger, "<<phases.size()<<" samples"<<endl;
	trigger(phases, thresholds, holdoff, trigger_out);
	fail|=verify_trigger(trigger_out, trigger_gold);

	cout<<"Calling photon, "<<trigger_gold2.size()<<" samples"<<endl;
	photon(trigger_gold2, timestamps, photons_out);
	fail|=verify_photons(photons_out, photons_gold);

	cout<<"Calling postage, "<<trigger_gold3.size()<<" samples"<<endl;
	postage(trigger_gold3, iqs, monitor, alignment, postage_out);
	fail|=verify_postage(postage_out, postage_gold);

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
