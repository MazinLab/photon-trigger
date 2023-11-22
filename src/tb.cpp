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

void flatten_postage_streams(hls::stream<singleiqstream_t> multi[N_MONITOR], hls::stream<singleiqstream_t> &flat) {
	int i=0;
	while (true) {
		int done=0;
		for (int i=0;i<N_MONITOR;i++)
			done+=multi[i].empty();
		if (done==N_MONITOR)
			break;
		while (not multi[i].empty()) {
			singleiqstream_t x;
			x=multi[i].read();
			flat.write(x);
			if (x.last) break;
		}
		i = i<N_MONITOR-1 ? i+1:0;
	}
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
			cout<<" data mismatch "<<endl;
			for (int i=0;i<N_PHASE;i++) {
				uint16_t gp,ep;
				uint32_t giq,eiq;
				gp=got.data.range((i+1)*PHASE_BITS-1,i*PHASE_BITS);
				ep=expect.data.range((i+1)*PHASE_BITS-1,i*PHASE_BITS);

				giq=got.data.range(N_PHASE*PHASE_BITS+IQ_BITS*(i+1)-1, N_PHASE*PHASE_BITS+i*IQ_BITS);
				eiq=expect.data.range(N_PHASE*PHASE_BITS+IQ_BITS*(i+1)-1, N_PHASE*PHASE_BITS+i*IQ_BITS);
				if (gp!=ep) cout<<" phase data mismatch"<<i<<" "<<gp<<" is not "<<ep<<endl;
				if (giq!=eiq) cout<<" iq data mismatch"<<i<<" "<<giq<<" is not "<<eiq<<endl;
			}
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


bool verify_postage(hls::stream<singleiqstream_t> &out, hls::stream<singleiqstream_t> &expected) {
	singleiqstream_t got, expect;
	bool fail=false;


	cout<<"Stream was "<<out.size()<<", expected "<<expected.size()<<endl;
	fail|=out.size()!=expected.size();

	int i=0, stamps[N_MONITOR];
	for (int i=0;i<N_MONITOR;i++) stamps[i]=0;
	while (!out.empty() && !expected.empty()) {

		got=out.read();
		expect=expected.read();

		if (expect.last) {
			//cout<<"Expect last of stamp "<<stamps[expect.user]++<<" with iq "<<expect.data<<endl;
			stamps[expect.user]++;
		} else if (i==0) {
			cout<<"Expect start of stamp "<<stamps[expect.user]<<" on mon "<<expect.user<<" with iq "<<expect.data<<" (sample "<<expect.data/2048<<")"<<endl;
		}

		if (expect.last!=got.last || expect.data!=got.data || expect.user!=got.user) {
			cout<<" Sample "<<i<<" of event "<<stamps[expect.user]<<" for monitor "<<expect.user<<":"<<endl;
		}

		if (expect.last!=got.last ) {
			cout<<"  last mismatch "<<got.last<<" is not "<<expect.last<<endl;
			fail=true;
		}

		if (expect.data!=got.data ) {
			cout<<"  data mismatch:"<<got.data.to_uint()<<" is not "<<expect.data.to_uint()<<endl;
			fail=true;
		}

		if (expect.user!=got.user) {
			cout<<"  user mismatch "<<got.user<<" is not "<<expect.user<<endl;
			fail=true;
		}
		if (expect.last) i=0;
		else i++;
	}

	if (!fail) cout<<"POSTAGE FILTER PASSED!"<<endl;
	else cout<<"POSTAGE FILTER FAILED!"<<endl;
	return fail;
}


bool verify_postage_maxi(iq_4x_t iq_cap_buffer[POSTAGE_BUFSIZE][N_CAPDATA/4], uint16_t event_count,
		iq_4x_t iq_cap_buffer_gold[POSTAGE_BUFSIZE][N_CAPDATA/4], uint16_t event_count_gold) {
	bool fail=false;

	cout<<"Expected "<<event_count_gold<<" postage stamps, got "<<event_count<<endl;
	fail|=event_count_gold!=event_count;

	for (int j=0;j<POSTAGE_BUFSIZE;j++){
		for (int k=0;k<N_CAPDATA/4;k++) {
			if (iq_cap_buffer[j][k]!=iq_cap_buffer_gold[j][k]){
				fail|=true;
				cout<<"mismatch "<<","<<j<<","<<k<<": "<<iq_cap_buffer[j][k]<<" should be "<<iq_cap_buffer_gold[j][k]<<endl;
			}
		}
	}
	if (!fail) cout<<"POSTAGE MAXI PASSED!"<<endl;
	return fail;
}





void load_postage_testvec(hls::stream<trigstream_t> &postage_trigger, reschan_t monitor[N_MONITOR],
		hls::stream<singleiqstream_t> &postage_gold,
		iq_4x_t iq_cap_gold[POSTAGE_BUFSIZE][N_CAPDATA_MAXI], uint16_t &event_count_gold) {


	//Test vector:
	//mon0: chan0 sample triggers: 0 (not possible in design), 37, 128
	//mon1: chan1 sample triggers: 30, 31, 128+30
	//mon2: chan2 sample triggers: 30, 31, 128+30
	//mon3: chan3 sample triggers: 30, 31, 128+30
	//mon4: chan4 sample triggers: 30, 31, 128+33
	//mon5: chan5 sample triggers: 30, 31, 128+30
	//mon6: chan531 sample triggers: 30, 31, 128+30
	//mon7: chan1337 sample triggers: 30, 31, 128+33

	// this needs (128+33+128)*2048 IQ values piped through in 4x
	// let IQ count from 0
	// will be 16 events
	// order data gets to maxi should be (mon, lane beat/samp event): 0,0 1-7,30 0,128 1-3,128+30 5-6,128+30 4,128+33 7,128+33

	reschan_t _mon[N_MONITOR]={0,1,2,3,4,5,531,1337};
	int32_t _last_event[N_MONITOR];
	for (int i=0;i<N_MONITOR;i++) {
		monitor[i]=_mon[i];
		_last_event[i]=-1000;//no previous event
	}
	const uint8_t N_TRIG=3;
	uint16_t trig_samples[N_MONITOR][N_TRIG]={	{0,37,129},
											{30,31,128+31},
											{30,31,128+31},
											{30,31,128+31},
											{30,31,128+31},
											{30,31,128+31},
											{30,31,128+31},
											{30,31,128+31}};
//observed output order 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
	event_count_gold=0;

	//Load the rest of the needed postage data
	for (uint32_t samp=0; samp<(128+33+128+1); samp++) {

		for (uint32_t group=0; group<N_PHASEGROUPS; group++) {

			uint32_t beat=samp*N_PHASEGROUPS+group;
			trigstream_t trigtmp;

			trigtmp.data.range(N_PHASE*PHASE_BITS-1,0)=0; //don't care
			trigtmp.last=group==(N_PHASEGROUPS-1);
			trigtmp.user.range(N_PHASEGROUPS_LOG2-1, 0)=group;

			for (uint32_t lane=0; lane<N_PHASE; lane++) {

				reschan_t chan=group*N_PHASE+lane;
				uint32_t iq=lane+N_PHASE*(group+samp*N_PHASEGROUPS);

				trigtmp.data.range(N_PHASE*PHASE_BITS+IQ_BITS*(lane+1)-1, N_PHASE*PHASE_BITS+IQ_BITS*lane)=iq;

				bool triggered=false;

				for (uint32_t mon=0;mon<N_MONITOR; mon++) {

					bool trigger_samp=false;

					for (uint8_t trig=0;trig<N_TRIG;trig++) {
						//cout<<"mon: "<<mon<<" trigger: "<<trig<<" "<<trig_samples[mon][trig]<<endl;
						trigger_samp |= trig_samples[mon][trig]==samp;
					} //trig

					if (trigger_samp && monitor[mon]==chan) { //this sample is a trigger on a monitored channel

						triggered = true;
						cout<<"Inserting a trigger on channel "<<chan<<" ("<<group<<", "<<lane<<") sample "<<samp<<" ";
						if (samp-_last_event[mon]>N_CAPDATA) { //we will capture this so go ahead and compute all the samples and spew them
							cout<<"and capture data from mon "<<mon<<", iqs from "<<((int64_t)iq)-(N_CAPPRE+1)*N_PHASE*N_PHASEGROUPS<<" to ";
							_last_event[mon]=samp; ///record when it happened

							iq_4x_t iq4x;
							for (uint32_t future_samp=0;future_samp<N_CAPDATA;future_samp++) {
								singleiqstream_t x;
								x.user=mon;
								//x.data=iq+future_samp*N_PHASE*N_PHASEGROUPS;
								if (N_CAPPRE*N_PHASE*N_PHASEGROUPS>(iq+future_samp*N_PHASE*N_PHASEGROUPS)) {
									x.data=0;
//									if (mon==0) {
//										cout<<"mon=0 sample "<<samp<<" cap_sample "<<future_samp<<" iq "<<iq<<endl;
//									}
								} else {
									x.data=iq+future_samp*N_PHASE*N_PHASEGROUPS-(N_CAPPRE)*N_PHASE*N_PHASEGROUPS;
								}
								x.last=future_samp==N_CAPDATA-1;
								postage_gold.write(x);

								if (future_samp==0) iq4x.range(IQ_BITS-1,0)=mon;
								else iq4x.range(IQ_BITS*(future_samp%4+1)-1, IQ_BITS*(future_samp%4))=x.data;

								if (future_samp%4==3) {
									iq_cap_gold[event_count_gold][future_samp/4]=iq4x;
									iq4x=0;
								} // store the packed iq value

							} // emitting each of the captured IQ samples for the event
							cout<<iq+(N_CAPDATA-1-N_CAPPRE)*N_PHASE*N_PHASEGROUPS<<" event "<<event_count_gold;
							event_count_gold++;

						} // if postage was captured
						else cout<<"but skipping capture because of previous cap.";
						cout<<endl;
					} //if its a trigger event on the monitored channel
				} // each mon channel

				trigtmp.user[N_PHASEGROUPS_LOG2+lane]=triggered;

			}//lane
			//if (trigtmp.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2)>0) cout<<"Trigger on beat "<<beat<<", sample "<<samp<<endl;
			postage_trigger.write(trigtmp);

		}//phasegroup
	}//sample

	cout<<"Loaded "<<postage_trigger.size()<<" beats for postage filter."<<endl;
	cout<<"Loaded "<<event_count_gold<<" stamps with a postage gold stream length of "<<postage_gold.size()<<endl<<endl;
}


bool drive() {


	///Gold file format: phase, trig event, photon event

	bool fail=false;
	hls::stream<phasestream_t> phases("Phase data");
	hls::stream<iqstream_t> iqs("IQ data");
	hls::stream<timestamp_t> timestamps("Timestamps");

	hls::stream<trigstream_t> trigger_out("Trig out"), trigger_gold("Trig gold"), trigger_gold2("Trig gold2"), postage_trigger("postage Trig input");
	hls::stream<photon_t> photons_out("Photons out"), photons_gold("Photon gold"), photons_gold2("Photon gold2"), photon_output_fifos[N_PHASE];
	hls::stream<singleiqstream_t> postage_out("postage out"), postage_gold_flat("postage gold flat"), postage_gold_flat2("postage gold flat2");

	// The postage output buffer
	auto iq_cap_buffer = new iq_4x_t [POSTAGE_BUFSIZE][N_CAPDATA_MAXI];
	auto iq_cap_buffer_gold = new iq_4x_t [POSTAGE_BUFSIZE][N_CAPDATA_MAXI];

	uint16_t event_count=0, event_count_gold=0;

	//The (packed) photon output buffer
	auto photon_buffer_out = new photon_uint_2x_t [N_PHOTON_BUFFERS][FLAT_PHOTON_BUFSIZE/2];
	auto photon_buffer_out_gold = new photon_uint_2x_t [N_PHOTON_BUFFERS][FLAT_PHOTON_BUFSIZE/2];

	photoncount_t n_photons[N_PHOTON_BUFFERS], n_photons_gold[N_PHOTON_BUFFERS];

	unsigned char active_buffer;

	// What to monitor ad settings
	threshoffs_t threshoffs[N_PHASEGROUPS];
	reschan_t monitor[N_MONITOR];
	unsigned int photon_trigger_sample_ndx[10];
	unsigned int n_photon_triggers=0;
	interval_t holdoff;
	threshold_t threshold;

	for (int i=0;i<POSTAGE_BUFSIZE;i++) {
		for (int j=0;j<N_CAPDATA_MAXI;j++) {
			iq_cap_buffer[i][j]=0;
			iq_cap_buffer_gold[i][j]=0;
		}
	}

	for (int i=0;i<N_PHOTON_BUFFERS;i++) {
		n_photons_gold[i]=0;
		n_photons[i]=0;
		for (int j=0;j<FLAT_PHOTON_BUFSIZE/2;j++) {
				photon_buffer_out[i][j]=0;
				photon_buffer_out_gold[i][j]=0;
		}
	}


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
	unsigned int i=0; //this is a sample from the gold file
	phasestream_t phasetmp;
	for (;;){
		bool trig_event, photon_event;
		short phase, photon_phase;
		if (!fs.getline(buf, MAXC)) {
			break;
		}
		stringstream strm(buf);
		strm>>phase>>trig_event>>photon_event>>photon_phase;

		if (trig_event) {
			photon_trigger_sample_ndx[n_photon_triggers]=i;
			n_photon_triggers++;
		}
		ap_uint<N_PHASE> trigger;

		uint8_t active_buff=0;
		uint32_t rotate_buffer_time, rotate_buffer_count, last_photon_time;
		rotate_buffer_time=1000000;
		rotate_buffer_count=10000;
		for (unsigned int group=0; group<N_PHASEGROUPS; group++) {

			// Load Photon gold stream and parse lane phase and trigger info
			phaseset_t x;
			for (unsigned int lane=0; lane<N_PHASE; lane++) {
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
					photon.time=i+28800000000l;//*N_PHASEGROUPS+group;
					photon.id=rid;
					photon.phase=phase;

					photons_gold.write(photon);
					photons_gold2.write(photon);

					assert(rid.to_int()<N_RES);
					assert(n_photons_gold[active_buff]<FLAT_PHOTON_BUFSIZE/2);
					if (n_photons_gold[0]%2) {
						photon_uint_2x_t tmp = photon_buffer_out_gold[active_buff][n_photons_gold[active_buff]/2];
						tmp.range(127, 64) = photon2uint(photon);
						photon_buffer_out_gold[active_buff][n_photons_gold[active_buff]/2]=tmp;
					} else {
						photon_uint_2x_t tmp;
						tmp.range(63, 0) = photon2uint(photon);
						photon_buffer_out_gold[active_buff][n_photons_gold[active_buff]/2]=tmp;
					}
					n_photons_gold[active_buff]++;
					last_photon_time = i;
					if (n_photons_gold[active_buff]> rotate_buffer_count || (i-last_photon_time)>rotate_buffer_time) {
						active_buff++;
						cout<<"Rotation needed on gold buffer. This isn't going to simulate properly!!!!!!!\n";
					}
					assert(active_buff<2);
				}
			}

			// Load phase stream, trigger gold stream
			trigstream_t trigtmp;
			phasetmp.data=phaseset2phases(x);
			phasetmp.last=group==(N_PHASEGROUPS-1);
			phasetmp.user=group;

			trigtmp.data.range(N_PHASE*PHASE_BITS-1,0)=phasetmp.data;
			uint32_t iqval;
			for (unsigned int lane=0;lane<N_PHASE;lane++) {
				//iq vals start at 0 (in the next section) and just count up 1 per lane and per sample
				trigtmp.data.range(N_PHASE*PHASE_BITS+IQ_BITS*(lane+1)-1, N_PHASE*PHASE_BITS+IQ_BITS*lane)=lane+group*N_PHASE+i*2048;
			}

			trigtmp.last=phasetmp.last;
			trigtmp.user.range(N_PHASEGROUPS_LOG2-1, 0)=phasetmp.user;
			trigtmp.user.range(N_PHASEGROUPS_LOG2+N_PHASE-1, N_PHASEGROUPS_LOG2) = trigger;

			phases.write(phasetmp);
			trigger_gold.write(trigtmp);

			timestamps.write(i+28800000000l);//*N_PHASEGROUPS+group);
		}
		i++;
	}

	int n_samples_from_file=i;

	//Load IQs
	unsigned int N_SAMP=i;
	iqstream_t iqtmp;
	iqtmp.data=0;
	for (int i=0;i<(N_SAMP+N_CAPDATA+N_CAPPRE)*N_PHASEGROUPS*N_PHASE;i++) { //-1 because the trigger stream has 1 cycle of windup and we won't bother testing that

		int iqgroup, pgroup, plane, iqlane, samp;
		samp=i/2048 - N_CAPPRE;
		pgroup=(i/N_PHASE)%N_PHASEGROUPS;
		iqgroup=(i/N_IQ)%N_GROUPS;
		plane=i%N_PHASE;
		iqlane=i%N_IQ;


		//8x iq
		iqtmp.data.range(IQ_BITS*(iqlane+1)-1,IQ_BITS*iqlane)=i;
		iqtmp.last=iqgroup==N_GROUPS-1;
		iqtmp.user=iqgroup;
		if (iqlane==N_IQ-1) {
			if (i<100) cout<<"iq "<<iqtmp.data.range(127,0)<<" "<<iqtmp.data.range(255,128)<<endl;
			iqs.write(iqtmp);
		}
	}



	//Now test things out

	i=0;
	while(!phases.empty()) {
		bool desync;
		desync=false;
		trigger(phases, iqs, threshoffs,  timestamps, trigger_out, desync, photon_output_fifos);
		photon_fifo_merger(photon_output_fifos, photons_out);
		i++;
		fail|=desync;
		if (desync) cout<<"Desync at "<<i<<endl;
	}
	fail|=verify_trigger(trigger_out, trigger_gold);
	fail|=verify_photons(photons_out, photons_gold);

	if (!iqs.empty()) {
		cout<<"IQs left: "<<iqs.size()<<endl;
		while(!iqs.empty()) iqs.read();
	}


//	photon_maxi(photons_gold2, photon_buffer_out, n_photons, active_buffer);
//	fail|=verify_photon_maxi(photon_buffer_out, n_photons, active_buffer, photon_buffer_out_gold_2d, n_photons_gold2d);



	bool overflow;

//	load_postage_testvec(postage_trigger, monitor, postage_gold_flat, iq_cap_buffer_gold, event_count_gold);
//	postage_filter_w_interconn(postage_trigger, monitor, postage_out, overflow);
//	fail|=verify_postage(postage_out, postage_gold_flat);
//	fail|=overflow;
//	if (overflow) cout<<"Got an overflow from postage"<<endl;


	if (!postage_trigger.empty()) {
		cout<<" Samples left in postage: "<<postage_trigger.size()<<endl;
		while(!postage_trigger.empty()) postage_trigger.read();
	}

	load_postage_testvec(postage_trigger, monitor, postage_gold_flat2, iq_cap_buffer_gold, event_count_gold);
	postage_maxi(postage_gold_flat2, iq_cap_buffer, event_count, event_count_gold);
	fail|=verify_postage_maxi(iq_cap_buffer, event_count, iq_cap_buffer_gold, event_count_gold);

	free(photon_buffer_out_gold);
	free(photon_buffer_out);
	free(iq_cap_buffer);
	free(iq_cap_buffer_gold);
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
