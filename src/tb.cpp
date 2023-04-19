#include "photon.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <bitset>
using namespace std;

#define MAXC 80

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
		int samp, group;
		group=i%N_PHASEGROUPS;
		samp=i/N_PHASEGROUPS;
		got=out.read();
		expect=expected.read();

//		if (group==0) {
//			cout<<"samp:"<<samp<<" expect: "<<expect.data<<", "<<expect.user<<" got: "<<got.data<<", "<<got.user<<endl;
//		}
		if (expect.data!=got.data || expect.user!=got.user || expect.last!=got.last)
			cout<<"Sample "<<samp<<" group: "<<group<<": phase0 expected="<<expect.data.range(15,0)<<", got "<<got.data.range(15,0)<<endl;

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
	hls::stream<photon_t> outlane[N_PHASE], expectlane[N_PHASE];


	while (!out.empty() && !expected.empty()) {
		got=out.read();
		expect=expected.read();
		outlane[got.id%N_PHASE].write(got);
		expectlane[expect.id%N_PHASE].write(expect);
	}
	int i=0;

	for (int i=0;i<N_PHASE;i++) {
		while (!outlane[i].empty() && !expectlane[i].empty()) {


			got=outlane[i].read();
			expect=expectlane[i].read();
			//cout<<"Photon "<<got.id<<" "<<got.phase<<" @ "<<got.time<<endl;

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
		}
	}

	if (!out.empty()) {
		cout<<"Also got:"<<endl;
		while (!out.empty()) {
			got=out.read();
			cout<<"Photon "<<got.id<<" "<<got.phase<<" @ "<<got.time<<endl;
		}
	}
	if (!fail) cout<<"PHOTON PASSED!"<<endl;
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
			if (expect.last!=got.last || expect.data!=got.data) {
				cout<<" Cycle "<<i<<":"<<endl;
			}

			if (expect.last!=got.last ) {
				cout<<"  last mismatch "<<got.last<<" is not "<<expect.last<<endl;
				fail=true;
			}

			if (expect.data!=got.data ) {
				cout<<"  data mismatch:"<<got.data.to_int()<<" is not "<<expect.data.to_int()<<endl;
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



bool drive() {


	///Gold file format: phase, trig event, photon event

	bool fail=false;
	hls::stream<phasestream_t> phases("Phase data");
	hls::stream<iqstreamnarrow_t> iqs("IQ data");
	hls::stream<timestamp_t> timestamps("Timestamps");

	hls::stream<trigstream_t> trigger_out("Trig out"), trigger_gold("Trig gold"), trigger_gold2("Trig gold2"), postage_trigger("postage Trig input");
	hls::stream<photon_t> photons_out("Photons out"), photons_gold("Photon gold"), photon_output_fifos[N_PHASE];
	hls::stream<singleiqstream_t> postage_out[N_MONITOR], postage_gold[N_MONITOR];

	hls::stream<ap_uint<N_PHASE>> photon_overflow("Overflow Out");

	threshoffs_t threshoffs[N_PHASEGROUPS];
	reschan_t monitor[N_MONITOR]={0,1,2,3,4,6,1025,2047};
	interval_t holdoff;
	threshold_t threshold;

	fstream fs("golden.dat");

	char buf[MAXC];
	if(!fs.is_open()) {
		cout<<"Can't open file!"<<endl;
	    return true;
	}
	if (!fs.getline(buf, MAXC)) {
		cout<<"Bad file!"<<endl;
		return true;
	}
	stringstream strm(buf);
	int threshtmp, holdofftmp;
	strm>>holdofftmp>>threshtmp;
	holdoff = holdofftmp;
	threshold=(uint8_t)threshtmp;

	cout<<"Holdoff: "<<(uint16_t)holdoff<<" Threshold (uint8): "<<(uint16_t)threshold<<endl;

	//Load Thresholds and Holdoffs
	for (int i=0;i<N_PHASEGROUPS;i++) {
		phaseset_t x;
		for (int j=0;j<N_PHASE;j++)
			x.phase[j] = ((uint16_t)holdoff+1)<<HOLDOFF_BITS | threshold;
		threshoffs[i]=phaseset2phases(x);
	}

	// Load times, phase input, and trigger and photon gold output
	int i=0;
	phasestream_t phasetmp;
	for (;;){
		bool trig_event, photon_event;
		short phase, photon_phase;
		if (!fs.getline(buf, MAXC)) {
			break;
		}
		stringstream strm(buf);
		strm>>phase>>trig_event>>photon_event>>photon_phase;

		ap_uint<N_PHASE> trigger;
		for (int group=0; group<N_PHASEGROUPS; group++) {

			// Load Photon gold stream and parse lane phase and trigger info
			phaseset_t x;
			for (int lane=0; lane<N_PHASE; lane++) {
				reschan_t rid;
				rid=group*N_PHASE+lane;
				trigger[lane]=0;

				bool skip=true;
				for (int mon=0; mon<N_MONITOR;mon++) {
					if (rid==monitor[mon]) {
						skip=false;
						break;
					}
				}
				if (skip) {
					x.phase[lane]=0;
					continue;
				}

				x.phase[lane]=phase;

				trigger[lane]=trig_event;
				if (photon_event) {
					photon_t photon;
					photon.time=i*N_PHASEGROUPS+group;
					photon.id=rid;
					photon.phase=phase;
					photons_gold.write(photon);
				}

			}

			// Load phase stream, trigger gold stream

			trigstream_t trigtmp;
			iqstreamnarrow_t iqstmp;
			phasetmp.data=phaseset2phases(x);
			phasetmp.last=group==(N_PHASEGROUPS-1);
			phasetmp.user=group;

			trigtmp.data=phasetmp.data;
			trigtmp.last=phasetmp.last;
			trigtmp.user.range(N_PHASEGROUPS_LOG2-1, 0)=phasetmp.user;
			trigtmp.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;

			phases.write(phasetmp);
			trigger_gold.write(trigtmp);
			postage_trigger.write(trigtmp);
			timestamps.write(i*N_PHASEGROUPS+group);
		}
		i++;
	}
//	for (int j=0;j<7;j++) {
//		phases.write(phasetmp);
//		timestamps.write(0);
//	}

	unsigned int N_SAMP=i;
	//Load IQs
	iqstreamnarrow_t iqtmp;
	iqtmp.data=-1;
	for (int i=0;i<(N_SAMP+N_CAPDATA+N_CAPPRE)*N_PHASEGROUPS*N_PHASE;i++) { //-1 because the trigger stream has 1 cycle of windup and we won't bother testing that

		int group, lane;
		group=(i/N_PHASE)%N_PHASEGROUPS;
		lane=i%N_PHASE;
		iqtmp.data.range(IQ_BITS*(lane+1)-1,IQ_BITS*lane)=i;
		iqtmp.last=group==N_PHASEGROUPS-1;
		if (lane==N_PHASE-1)
			iqs.write(iqtmp);
	}

	i=0;
	while(!phases.empty()) {
		trigger(phases, threshoffs,  trigger_out, timestamps, photon_output_fifos);
		photon_fifo_merger(photon_output_fifos, photons_out);
		i++;
	}
	fail|=verify_trigger(trigger_out, trigger_gold);
	fail|=verify_photons(photons_out, photons_gold);


	while (!iqs.empty()) {iqs.read();}

//	cout<<"Calling postage with "<<postage_trigger.size()<<", "<<iqs.size()<<" samples"<<endl;
//	postage(postage_trigger, iqs, monitor, postage_out);
//	fail|=verify_postage(postage_out, postage_gold);
//
//	const unsigned int N_SAMP = sizeof(phasedat) / sizeof(int);
	//Build phase inputs and expected outputs, let timestamp just be the clock

	//trigger needs phases for all resonators with photon events placed on specific resonators at specific points
	//Threshold level
	// holdoff time
	// verify only phase > thershold triggers even and only if holdoff after previous
	// trigger should come on phase of/immediately after peak  //todo which

	//photon, needs trigger stream and timestamps
	// should emit photons in resonator order with resonator, phase of peak, and


//
//	holdoff=3;
//	previous_t last[N_PHASEGROUPS][N_PHASE];
//	thres=10;
//
//
////
////
////	//Load some filler trigstream so we can vet the pre-samples
////	for (int i=0;i<N_CAPPRE*N_PHASEGROUPS;i++) {
////		trigstream_t trigtmp;
////		trigtmp.data=-1;
////		trigtmp.user=(i%N_PHASEGROUPS);
////		trigtmp.last=(i%N_PHASEGROUPS)==N_PHASEGROUPS-1;
////		postage_trigger.write(trigtmp);
////	}
//
//
//
//	//Zero cache data
//	for (int i=0;i<N_PHASEGROUPS;i++) {
//		for (int j=0;j<N_PHASE;j++) {
//			last[i][j].phase=0;
//			last[i][j].since=0;
//		}
//	}
//
//	//Phase data and trigger events
//	phaseset_t tmp;
//	ap_uint<N_PHASE> trigv;
//	unsigned short last_phase[N_PHASEGROUPS][N_PHASE];
//	short holdoffctr[N_PHASEGROUPS][N_PHASE];
//	phases_t inphase_previous[N_PHASEGROUPS];
//	for (int i=0;i<N_PHASEGROUPS;i++) {
//		for (int j=0;j<N_PHASE;j++) {
//			last_phase[i][j]=0;
//			holdoffctr[i][j]=-1000;
//		}
//		inphase_previous[i]=0;
//	}
//
//	group_t next_group=0;
//	for (int i=0;i<N_SAMP*N_PHASEGROUPS*N_PHASE;i++) {
//		iqstreamnarrow_t in;
//		phase_t phase;
//		threshold_t thresh;
//		int group, lane, rid, beat, samp;
//		group=(i/N_PHASE)%N_PHASEGROUPS;
//		lane=i%N_PHASE;
//		rid=i%(N_PHASEGROUPS*N_PHASE);
//		beat=i/N_PHASE;
//		samp=i/N_PHASEGROUPS/N_PHASE;
//
//
//
//		thresh=phases2phaseset(thresholds[group]).phase[lane];
//		phase=group<2 ? phasedat[samp]: 0;
//
//		tmp.phase[lane]=phase;
//
//		trigv[lane]=last_phase[group][lane]>thresh && last_phase[group][lane]>phase && holdoffctr[group][lane]<=0;
//
//		photon_t phot;
//		phot.id=rid;
//		phot.time=beat;
//		phot.phase=phasedat[samp-1];
//
//
//
//		if (lane==N_PHASE-1) {
//			phasestream_t inphase;
//			inphase.data=phaseset2phases(tmp);
//			inphase.user=group;
//			inphase.last=inphase.user==N_PHASEGROUPS-1;
//			phases.write(inphase);
//
//			trigstream_t trigtmp;
//			trigtmp.data=inphase_previous[group];
//			trigtmp.user=group;
//			trigtmp.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1,N_PHASEGROUPS_LOG2)=trigv;
//			trigtmp.last=inphase.last;
//			trigger_gold.write(trigtmp);
//			trigger_gold2.write(trigtmp);
//			if (beat>N_PHASEGROUPS-1) {
//				postage_trigger.write(trigtmp);
//				next_group++;
//			}
//
//			inphase_previous[group]=inphase.data;
//		}
//
//		if (lane==0 && (group==0||group==1)||(group==511 &&lane==3) || (group==0)){
//			if (trigv[lane]) {
//			cout<<"Cycle "<<beat<<" rid="<<rid<<" phase is "<<phase<<" was "<<last_phase[group][lane];
//			cout<<". Thresh is "<<thresh;
//			cout<<" Trigger event: "<<trigv[lane]<<" Holdoff counter: "<<holdoffctr[group][lane]<<endl;
//
//		    //cout<<"  EXPECT Photon: rid="<<phot.id<<" "<<phot.phase<<" at "<<phot.time<<endl;
//			}
//		}
//		if (trigv[lane]) {cout<<"  EXPECT Photon: rid="<<phot.id<<" "<<phot.phase<<" at "<<phot.time<<endl;}
//
//		if (trigv[lane]) {
//			photons_gold.write(phot);
//			for (int mon=0; mon<N_MONITOR;mon++) {
//				if (rid==monitor[mon] && holdoffctr[group][lane]<-(N_CAPDATA-holdoff)) {
//					if (rid==0) cout<<"  EXPECT IQ Packet: ";
//					for (int i_cap=0;i_cap<N_CAPDATA;i_cap++) {
//						singleiqstream_t tmp;
//						int x;
//						x=i+i_cap*2048;
//						tmp.data=x;//<0? 0: x;
//						tmp.last=i_cap==(N_CAPDATA-1);
//						tmp.user=mon;
//						if (rid==0 && i_cap==-N_CAPPRE) cout<<tmp.data<<" to ";
//						if (rid==0 && i_cap==N_CAPDATA-N_CAPPRE-1) cout<<tmp.data;
//						postage_gold[mon].write(tmp);
//					}
//					if (rid==0) cout<<endl;
//				}
//			}
//		}
//		if (trigv[lane])
//			holdoffctr[group][lane]=holdoff;
//		else
//			holdoffctr[group][lane]--;
//		last_phase[group][lane]=phase;
//	}
//
//	while (postage_trigger.size()!=iqs.size()) {
//		trigstream_t trigtmp;
//		trigtmp.data=0;
//		trigtmp.user=next_group++;
//		trigtmp.last=trigtmp.user==N_PHASEGROUPS-1;
//		postage_trigger.write(trigtmp);
//	}
//



//	cout<<"Calling postage, "<<postage_trigger.size()<<", "<<iqs.size()<<" samples"<<endl;
//	postage(postage_trigger, iqs, monitor, postage_out);
//	fail|=verify_postage(postage_out, postage_gold);

	return fail;
}

int main (void){

	bool fail=false;

	fail=drive();


	if (fail) {
		std::cout << "Test failed" << std::endl;
	} else {
		std::cout << "Test passed" << std::endl;
	}
	return(fail);
}
