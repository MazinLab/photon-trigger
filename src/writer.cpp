#include "photon.hpp"

/*
 * Photons are generated at 2048*MAX_RATE (inbound at 68 bytes, outbound at 4 bytes if sorted 6 if w/resid)
 * thats an inbound rate of 39 or 59 MiB/s at 5000 phot/s
 * bandwidth to PSDRAM is ~300*16 4.8GB/s or 4GiB/s more likely
 *
 * when a photon is generated it will take 200 samples before another can arrive on that channel (~100k 512 MHz tics)
 * so even if every channel generates a photon successively we have at most 2048*4B of data to write and
 * ~96k 512MHz tics to do it. The write (~8K), fully bursted, would take about 1024 512MHz clocks for an axi bus speed of
 * 256Mhz. So leaving ~95k tics left (about 185 us).
 *
 * but many resonators will never have photons (bad or low light) if we write with the id then a maximal burst is
 * 682.6 photons and it would take 3 bursts+1 single write to output a single photon on every channel so ~1538 512MHz clocks
 * still leaving an eternity.
 *
 *
 * The postage stamping plan calls for monitoring up to 8 resonators for their holdoff so
 * N_MONITOR*1MHz*4 or in bursts of (N_CAPDATA+N_CAPPRE)*4 bytes or 480 byte bursts at 32MiB/s
 *
 * Postage stamp data comes in sequenitally and easily. Photons arrive randomly but clustered so need to be sorted
 * worst case scenario after a full N_CAPDATA+N_CAPPRE arrives on one stream it will be 512 clocks before the next
 * sample. 1/(5000cps*1e-6MHz) means 200 samples between photons/triggers with an N_CAPDATA+N_CAPPRE of 120
 * (~8300cps) that leaves 80*512 ~40k 512MHz tics before the next sample we need to capture would arrive.
 * Bursting out the 480 bytes at 256 MHz/16B needs only ~60+overhead 512MHz tics.
 * So even if we serially read from the postage stamp channels we are looking at
 * N_MONITOR*((N_CAPDATA+N_CAPPRE)+(N_CAPDATA+N_CAPPRE)*4/16) or about 1200 256MHz clocks or 2400 512 or about
 * 6% of our time before we need need to deal with a photon.
 *
 * So deal with them serially and have a buffer long enough for 1 full postage for each channel (~3.8KiB)!
 *
 */



void photons_maxi_structured(hls::stream<photon_t> &photons, smallphoton_t photons_out[N_PHOTON_BUFFERS][N_RES][MAX_CPS],
				  	  	  	 photoncount_t n_photons[N_PHOTON_BUFFERS][N_RES], unsigned char &active_buffer) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=photons depth=64 register
#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave
#pragma HLS INTERFACE mode=s_axilite port=active_buffer
#pragma HLS INTERFACE mode=s_axilite port=n_photons

// buffer address consists of three consecutive photon buffers each of 2048*(n) length
//format is photons_i....


	photoncount_t _n_photons[2048];
	for (int j=0;j<N_RES;j++) _n_photons[j]=0;
//#pragma HLS ARRAY_PARTITION variable = next complete

	unsigned char i=0;
	uint32_t _total_photons=0;
	timestamp_t _start=0;
	bool started=false;
	while(!photons.empty()) {

		photon_t photon;
		smallphoton_t sphot;
		photoncount_t _n_phot_rid;
		timestamp_t _elapsed;

		photon = photons.read();
		_total_photons++;
		sphot.time=photon.time;
		sphot.phase=photon.phase;

		_n_phot_rid=_n_photons[photon.id]+1;

		photons_out[i][photon.id][_n_photons[photon.id]]=sphot;
		_n_photons[photon.id]=_n_phot_rid;
		n_photons[i][photon.id]=_n_phot_rid;


		if (_start>photon.time) {
			_elapsed = photon.time + (65535 - _start);
		} else {
			_elapsed = photon.time-_start;
		}

		if (_total_photons>N_PHOTON_LATENCY || _n_phot_rid>=MAX_CPS || (_elapsed>MAX_TIME_LATENCY&&started)) {
#ifndef __SYNTHESIS__
			cout<<" rotate buffer "<<_total_photons<<", "<<_n_phot_rid<<", "<<_elapsed<<endl;
#endif
			_total_photons=0;
			_start=photon.time;
			i = i<N_PHOTON_BUFFERS-1 ? i+1: 0;
			for (int j=0;j<N_RES;j++) _n_photons[j]=0;
		}
		active_buffer=i;
		started=true;
	}
//	i = i<N_PHOTON_BUFFERS-1 ? i+1: 0;
//	active_buffer=i;
}


void photons_maxi_id(hls::stream<photon_t> &photons, photon_t photons_out[N_PHOTON_BUFFERS][PHOTON_BUFF_N],
				  photoncount_t n_photons[N_PHOTON_BUFFERS], unsigned char &active_buffer) {
#pragma HLS INTERFACE mode=s_axilite port=return
#pragma HLS INTERFACE mode=axis port=photons depth=64 register
#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave max_write_burst_length=256
#pragma HLS INTERFACE mode=s_axilite port=active_buffer
#pragma HLS INTERFACE mode=s_axilite port=n_photons
#pragma HLS ARRAY_PARTITION variable=photons_out type=complete
#pragma HLS DEPENDENCE direction=WAW type=inter variable=photons_out distance=8000 true
#pragma HLS DEPENDENCE direction=WAR type=inter variable=photons_out false
#pragma HLS DEPENDENCE direction=RAW type=inter variable=photons_out false

	static unsigned char _ab;
	photoncount_t _n_photons;
	timestamp_t _elapsed=0, _start;
	bool started=false;

	while (_n_photons<PHOTON_BUFF_N-1 && _elapsed<MAX_TIME_LATENCY) {
#pragma HLS PIPELINE ii=3
		active_buffer=_ab;

		photon_t photon;

		photon = photons.read();

		if (!started) {
			_start=photon.time;
		} else if (_start>photon.time) {
			_elapsed = photon.time + (65535 - _start);
		} else {
			_elapsed = photon.time-_start;
		}

		started=_elapsed<MAX_TIME_LATENCY;
		photons_out[_ab][_n_photons]=photon;
		_n_photons++;
		n_photons[_ab]=_n_photons;
	}

	#ifndef __SYNTHESIS__
	cout<<" rotate buffer "<<_n_photons<<", "<<_elapsed<<endl;
	#endif
	_ab = _ab<N_PHOTON_BUFFERS-1 ? _ab+1: 0;
	_n_photons=0;
	n_photons[_ab]=0;
}


//
////#define EXPLICIT_ACQUIRE_RELEASE
////#include "hls_streamofblocks.h"
//
////typedef unsigned int postageblock[N_CAPDATA];
////
////typedef hls::stream_of_blocks<postageblock, 2> capblock_t;
////
////
////void reader(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
////		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
////		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7){//, hls::stream<bool> &done) {
////#pragma HLS ARRAY_PARTITION variable=iq_in complete
//////#pragma HLS ARRAY_PARTITION variable=capblocks complete
////
////	capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
//////	capblock_t capblock;
////
////
////	unsigned char n[N_MONITOR];
////#pragma HLS ARRAY_PARTITION variable=n complete
////	while(true) {
////		for (int i=0;i<N_MONITOR;i++) {
////#pragma HLS UNROLL
//////			capblock=cb[i];
////			for (int n=0;n<N_CAPDATA;n++) {
////				hls::write_lock<postageblock> b(cb[i]);
////				b[n]=iq_in[i].read().data;
////			}
////		}
////	}
////}
//
//
//void reader(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
//		hls::stream<iq_t> iq[N_MONITOR], hls::stream<photon_t> &photout){//, hls::stream<bool> &done) {
//#pragma HLS ARRAY_PARTITION variable=iq_in complete
//#pragma HLS ARRAY_PARTITION variable=iq complete
//
//	while(true) {
//#pragma HLS PIPELINE II=1
//
//		ap_uint<N_MONITOR> read;
//		iq_t x[N_MONITOR];
//		#pragma HLS ARRAY_PARTITION variable=x complete
//		singleiqstream_t tmp[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=tmp complete
//
//		read: for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//
//			read[i]=iq_in[i].read_nb(tmp[i]);
//			if (read[i])
//				iq[i].write_nb(tmp[i].data);
//		}
////		write: for (int i=0;i<N_MONITOR;i++) {
////		#pragma HLS UNROLL
////			if (read[i])
////				iq[i].write_nb(tmp[i].data);
////		}
//
//		photon_t phot;
//		bool readphot;
//		readphot = photons.read_nb(phot);
//		if (readphot) photout.write_nb(phot);
//	}
//}
////
////void readerdf(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
////		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
////		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7){//, hls::stream<bool> &done) {
////#pragma HLS ARRAY_PARTITION variable=iq_in complete
//////#pragma HLS ARRAY_PARTITION variable=capblocks complete
////
////	capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
//////	capblock_t capblock;
////
////
////	unsigned char n[N_MONITOR];
////#pragma HLS ARRAY_PARTITION variable=n complete
////	while(true) {
////		ap_uint<N_MONITOR> read;
////		iq_t x[N_MONITOR];
////		read: for (int i=0;i<N_MONITOR;i++) {
////			#pragma HLS UNROLL
////			singleiqstream_t tmp;
////			read[i]=iq_in[i].read_nb(tmp);
////			x[i]=tmp.data;
////
////			if (read[i]) {
////				if (n[i]==0) {
////					hls::write_lock<postageblock> b(capblocks0);
////					b.write_lock(s)
////				}
////			}
////		}
////
////		read[i]=iq_in[i].read_nb().data;
////		for (int i=0;i<N_MONITOR;i++) {
////#pragma HLS UNROLL
//////			capblock=cb[i];
////			for (int n=0;n<N_CAPDATA;n++) {
////				hls::write_lock<postageblock> b(cb[i]);
////				b[n]=iq_in[i].read().data;
////			}
////		}
////	}
////}
////
////
////void write_maxi(
////		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
////		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7,
////		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7) {
//////#pragma HLS INTERFACE mode=ap_ctrl_none port=return
//////#pragma HLS ARRAY_PARTITION variable=capblocks complete
////
//////	bool finished=false, skipdone=false;
////
////	unsigned short n[N_MONITOR];
////#pragma HLS ARRAY_PARTITION variable = n complete
////
////	for (int i=0;i<N_MONITOR;i++) n[i]=0;
////
////	distin: while (true) {
//////		skipdone=true;
////		for (int i=0;i<N_MONITOR;i++) {
////		#pragma HLS UNROLL
////			int base;
////			iq_t* iq_out;
////			iq_t* iqouts[N_MONITOR]={iq_out0,iq_out1,iq_out2,iq_out3,iq_out4,iq_out5,iq_out6,iq_out7};
////			capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
//////			capblock_t &capblock;
//////			capblock=cb[i];
////			iq_out=iqouts[i];
////			base=n[i];
////			n[i]+=N_CAPDATA;
////			for (int n=0;n<N_CAPDATA;n++) {
////				hls::read_lock<postageblock> b(cb[i]);
////				iq_out[base+n]=b[n];
////			}
////		}
//////		done.read_nb(finished);
////	}
////}
//
//
//void write_maxi(hls::stream<iq_t> monstreams[N_MONITOR], hls::stream<photon_t> &photstream,
//		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7,
//		unsigned long *phot_out) {
////#pragma HLS INTERFACE mode=ap_ctrl_none port=return
//#pragma HLS ARRAY_PARTITION variable=monstreams complete
////#pragma HLS DEPENDENCE class=pointer dependent=false direction=RAW type=inter variable=phot_out
////#pragma HLS DEPENDENCE class=pointer dependent=false direction=WAR type=inter variable=phot_out
////#pragma HLS DEPENDENCE class=pointer dependent=false direction=RAW type=intra variable=phot_out
////#pragma HLS DEPENDENCE class=pointer dependent=false direction=WAR type=intra variable=phot_out
//
////	bool finished=false, skipdone=false;
//	unsigned short n[N_MONITOR];
//	unsigned int nphot;
//#pragma HLS ARRAY_PARTITION variable = n complete
//
//	for (int i=0;i<N_MONITOR;i++) n[i]=0;
//
//	writemain: while (true) {
//		mon: for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//			int base;
//			iq_t* iqouts[N_MONITOR]={iq_out0,iq_out1,iq_out2,iq_out3,iq_out4,iq_out5,iq_out6,iq_out7};
////#pragma HLS ARRAY_PARTITION variable = iqouts complete
//			base=n[i];
//			if (!monstreams[i].empty()) { //data will be coming, let all else buffer
//				burstpostage: for (int j=0;j<N_CAPDATA;j++) {
//				#pragma HLS PIPELINE II=1
//					iqouts[i][base+j]=monstreams[i].read();
//				}
//				n[i]+=N_CAPDATA;
//			}
//		}
//		if (!photstream.empty()) { //data will be coming, let all else buffer
//			burstphot: for (int j=0;j<64;j++) {
//			#pragma HLS PIPELINE II=1
//				photon_t phot;
//				unsigned long x;
//				phot=photstream.read();
//				x=((unsigned long) phot.id) | ((unsigned long) phot.phase<<16)| ((unsigned long) phot.time<<32);
//				phot_out[nphot+j]=x;
//			}
//			nphot+=64;
//		}
////		done.read_nb(finished);
//	}
//}
//
//
////void maxi_writer(hls::stream<singleiqstream_t> postagein[N_MONITOR], hls::stream<photon_t> &photons,
////		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7) {
////#pragma HLS INTERFACE ap_ctrl_none port=return
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out0 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out1 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out2 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out3 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out4 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out5 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out6 offset=slave
////#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out7 offset=slave
////
////#pragma HLS INTERFACE mode=axis depth=13500 port=postagein
////#pragma HLS INTERFACE mode=axis depth=13500 port=photons
////#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=postagein
//////#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//////#pragma HLS INTERFACE mode=s_axilite port=monitor
//////#pragma HLS INTERFACE mode=s_axilite port=alignment
////
////#pragma HLS DATAFLOW
////
//////	hls::stream<bool> done;
//////#pragma HLS stream variable = done depth = 2
////
//////	capblock_t capblocks[N_MONITOR];
//////	capblock_t capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7;
////
////	reader(postagein, photons, capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7);
////	write_maxi(capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7,
////			iq_out0, iq_out1, iq_out2, iq_out3, iq_out4, iq_out5, iq_out6, iq_out7);
////}
//
//
//void maxi_writer(hls::stream<singleiqstream_t> postagein[N_MONITOR], hls::stream<photon_t> &photons,
//		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7,
//		unsigned long *phot_out) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out0 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out1 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out2 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out3 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out4 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out5 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out6 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out7 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=phot_out offset=slave
//
//#pragma HLS INTERFACE mode=axis depth=13500 port=postagein
//#pragma HLS INTERFACE mode=axis depth=13500 port=photons
//#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=postagein
////#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
////#pragma HLS INTERFACE mode=s_axilite port=monitor
////#pragma HLS INTERFACE mode=s_axilite port=alignment
//
//#pragma HLS DATAFLOW
//
////	hls::stream<bool> done;
////#pragma HLS stream variable = done depth = 2
//
//	const int MONDEPTH=2*N_CAPDATA;
//	hls::stream<iq_t> monstreams[N_MONITOR];
//#pragma HLS stream variable=monstreams depth=MONDEPTH
//#pragma HLS ARRAY_PARTITION variable=monstreams complete
//
//	hls::stream<photon_t> photstream;
//#pragma HLS stream variable=photstream depth=MONDEPTH
//
//	reader(postagein, photons, monstreams, photstream);
//	write_maxi(monstreams, photstream, iq_out0, iq_out1, iq_out2, iq_out3, iq_out4, iq_out5, iq_out6, iq_out7,phot_out);
//}
//
//
//
//void photons_maxi(hls::stream<photon_t> &photons, unsigned short n,
//		smallphoton_t *photons_out, photoncount_t *count_out, unsigned char &active_buffer) {
//#pragma HLS INTERFACE mode=s_axilite port=return
//#pragma HLS INTERFACE mode=axis port=photons depth=64 register
//#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=count_out offset=slave
//#pragma HLS INTERFACE mode=s_axilite port=active_buffer
//#pragma HLS INTERFACE mode=s_axilite port=n
//
//// buffer address consists of three consecutive photon buffers each of 2048*(n) length
////format is photons_i....
//
//
//	unsigned short next[2048];
//#pragma HLS ARRAY_PARTITION variable = next complete
//
//	unsigned short _n=6000;
//	_n=n;
//	smallphoton_t *_buf;
//	for (int i=0;i<3;i++) {
//		bool full;
//		full=false;
//		active_buffer=i;
//		_buf = photons_out+2048*_n*i;
//		for (int j=0;j<2048;j++)
//			next[j]=0;
//		while (!full) {
//			photon_t photon;
//			smallphoton_t sphot;
//
//			photon = photons.read();
//			sphot.time=photon.time;
//			sphot.phase=photon.phase;
//
//			_buf[next[photon.id]]=sphot;
//			full = !(next[photon.id]<_n-1);
//			next[photon.id]++;
//		}
//		for (int j=0;j<2048;j++)
//			count_out[2048*i+j]=next[j];
//
//	}
//
//}
//
//
//
//
//void write_photons_df(hls::stream<photon_t> &photons, unsigned short n,
//		smallphoton_t *photons_out, photoncount_t *count_out, unsigned char &active_buffer) {
//#pragma HLS INTERFACE mode=s_axilite port=return
//#pragma HLS INTERFACE mode=axis port=photons depth=64 register
//#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=photons_out offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=64 max_widen_bitwidth=128 port=count_out offset=slave
//#pragma HLS INTERFACE mode=s_axilite port=active_buffer
//#pragma HLS INTERFACE mode=s_axilite port=n
//
//// buffer address consists of three consecutive photon buffers each of 2048*(n) length
////format is photons_i....
//
//
//
//#pragma HLS DATAFLOW
//
////	hls::stream<bool> done;
////#pragma HLS stream variable = done depth = 2
//
//	const int MONDEPTH=2*N_CAPDATA;
//	hls::stream<iq_t> monstreams[N_MONITOR];
//#pragma HLS stream variable=monstreams depth=MONDEPTH
//#pragma HLS ARRAY_PARTITION variable=monstreams complete
//
//	hls::stream<photon_t> photstream;
//#pragma HLS stream variable=photstream depth=MONDEPTH
//
//	photon_reader(photons, photstream);
//	write_maxi(photstream, iq_out0, iq_out1, iq_out2, iq_out3, iq_out4, iq_out5, iq_out6, iq_out7,phot_out);
//}
//
//
//
//void photon_reader(hls::stream<photon_t> &photons,hls::stream<sphoton_t> &photout){
//
//	while(true) {
//#pragma HLS PIPELINE II=1
//
//		photon_t phot;
//		photons.read(phot);
//		photout.write(phot);
//	}
//}
//
//
//void photon_addresser(hls::stream<photon_t> &photons,
//		hls::stream<destined_photon_t> &photout, hls::stream<bool> &buffer_full){
//
//	unsigned short next[2048];
//#pragma HLS ARRAY_PARTITION variable = next complete
//
//	while(true) {
//#pragma HLS PIPELINE II=1
//
//
//		destined_photon_t out;
//		photon = photons.read();
//
//		out.offset=6000*photon.id+next[photon.id];
//		out.photon.phase=photon.phase;
//		out.photon.time=photon.time;
//		full = next[photon.id]==6000-2;
//
//		if (full) {
//			for (int j=0;j<2048;j++)
//				next[j]=0;
//		} else{
//			next[photon.id]++;
//		}
//		buffer_full.write(full);
//		photout.write(out);
//
//	}
//}
