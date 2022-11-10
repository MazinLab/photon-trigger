#include "photon.hpp"

//#define EXPLICIT_ACQUIRE_RELEASE
//#include "hls_streamofblocks.h"

//typedef unsigned int postageblock[N_CAPDATA];
//
//typedef hls::stream_of_blocks<postageblock, 2> capblock_t;
//
//
//void reader(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
//		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
//		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7){//, hls::stream<bool> &done) {
//#pragma HLS ARRAY_PARTITION variable=iq_in complete
////#pragma HLS ARRAY_PARTITION variable=capblocks complete
//
//	capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
////	capblock_t capblock;
//
//
//	unsigned char n[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=n complete
//	while(true) {
//		for (int i=0;i<N_MONITOR;i++) {
//#pragma HLS UNROLL
////			capblock=cb[i];
//			for (int n=0;n<N_CAPDATA;n++) {
//				hls::write_lock<postageblock> b(cb[i]);
//				b[n]=iq_in[i].read().data;
//			}
//		}
//	}
//}

void reader(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
		hls::stream<iq_t> iq[N_MONITOR], hls::stream<photon_t> &photout){//, hls::stream<bool> &done) {
#pragma HLS ARRAY_PARTITION variable=iq_in complete
#pragma HLS ARRAY_PARTITION variable=iq complete

	while(true) {
#pragma HLS PIPELINE II=1

		ap_uint<N_MONITOR> read;
		iq_t x[N_MONITOR];
		#pragma HLS ARRAY_PARTITION variable=x complete
		singleiqstream_t tmp[N_MONITOR];
#pragma HLS ARRAY_PARTITION variable=tmp complete

		read: for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL

			read[i]=iq_in[i].read_nb(tmp[i]);
			if (read[i])
				iq[i].write_nb(tmp[i].data);
		}
//		write: for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//			if (read[i])
//				iq[i].write_nb(tmp[i].data);
//		}

		photon_t phot;
		bool readphot;
		readphot = photons.read_nb(phot);
		if (readphot) photout.write_nb(phot);
	}
}
//
//void readerdf(hls::stream<singleiqstream_t> iq_in[N_MONITOR], hls::stream<photon_t> &photons,
//		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
//		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7){//, hls::stream<bool> &done) {
//#pragma HLS ARRAY_PARTITION variable=iq_in complete
////#pragma HLS ARRAY_PARTITION variable=capblocks complete
//
//	capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
////	capblock_t capblock;
//
//
//	unsigned char n[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable=n complete
//	while(true) {
//		ap_uint<N_MONITOR> read;
//		iq_t x[N_MONITOR];
//		read: for (int i=0;i<N_MONITOR;i++) {
//			#pragma HLS UNROLL
//			singleiqstream_t tmp;
//			read[i]=iq_in[i].read_nb(tmp);
//			x[i]=tmp.data;
//
//			if (read[i]) {
//				if (n[i]==0) {
//					hls::write_lock<postageblock> b(capblocks0);
//					b.write_lock(s)
//				}
//			}
//		}
//
//		read[i]=iq_in[i].read_nb().data;
//		for (int i=0;i<N_MONITOR;i++) {
//#pragma HLS UNROLL
////			capblock=cb[i];
//			for (int n=0;n<N_CAPDATA;n++) {
//				hls::write_lock<postageblock> b(cb[i]);
//				b[n]=iq_in[i].read().data;
//			}
//		}
//	}
//}
//
//
//void write_maxi(
//		capblock_t &capblocks0, capblock_t &capblocks1, capblock_t &capblocks2, capblock_t &capblocks3,
//		capblock_t &capblocks4, capblock_t &capblocks5, capblock_t &capblocks6, capblock_t &capblocks7,
//		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7) {
////#pragma HLS INTERFACE mode=ap_ctrl_none port=return
////#pragma HLS ARRAY_PARTITION variable=capblocks complete
//
////	bool finished=false, skipdone=false;
//
//	unsigned short n[N_MONITOR];
//#pragma HLS ARRAY_PARTITION variable = n complete
//
//	for (int i=0;i<N_MONITOR;i++) n[i]=0;
//
//	distin: while (true) {
////		skipdone=true;
//		for (int i=0;i<N_MONITOR;i++) {
//		#pragma HLS UNROLL
//			int base;
//			iq_t* iq_out;
//			iq_t* iqouts[N_MONITOR]={iq_out0,iq_out1,iq_out2,iq_out3,iq_out4,iq_out5,iq_out6,iq_out7};
//			capblock_t cb[N_MONITOR]={capblocks0,capblocks1,capblocks2,capblocks3,capblocks4,capblocks5,capblocks6,capblocks7};
////			capblock_t &capblock;
////			capblock=cb[i];
//			iq_out=iqouts[i];
//			base=n[i];
//			n[i]+=N_CAPDATA;
//			for (int n=0;n<N_CAPDATA;n++) {
//				hls::read_lock<postageblock> b(cb[i]);
//				iq_out[base+n]=b[n];
//			}
//		}
////		done.read_nb(finished);
//	}
//}


void write_maxi(hls::stream<iq_t> monstreams[N_MONITOR], hls::stream<photon_t> &photstream,
		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7,
		unsigned long *phot_out) {
//#pragma HLS INTERFACE mode=ap_ctrl_none port=return
#pragma HLS ARRAY_PARTITION variable=monstreams complete
//#pragma HLS DEPENDENCE class=pointer dependent=false direction=RAW type=inter variable=phot_out
//#pragma HLS DEPENDENCE class=pointer dependent=false direction=WAR type=inter variable=phot_out
//#pragma HLS DEPENDENCE class=pointer dependent=false direction=RAW type=intra variable=phot_out
//#pragma HLS DEPENDENCE class=pointer dependent=false direction=WAR type=intra variable=phot_out

//	bool finished=false, skipdone=false;
	unsigned short n[N_MONITOR];
	unsigned int nphot;
#pragma HLS ARRAY_PARTITION variable = n complete

	for (int i=0;i<N_MONITOR;i++) n[i]=0;

	writemain: while (true) {
		mon: for (int i=0;i<N_MONITOR;i++) {
		#pragma HLS UNROLL
			int base;
			iq_t* iqouts[N_MONITOR]={iq_out0,iq_out1,iq_out2,iq_out3,iq_out4,iq_out5,iq_out6,iq_out7};
//#pragma HLS ARRAY_PARTITION variable = iqouts complete
			base=n[i];
			if (!monstreams[i].empty()) { //data will be coming, let all else buffer
				burstpostage: for (int j=0;j<N_CAPDATA;j++) {
				#pragma HLS PIPELINE II=1
					iqouts[i][base+j]=monstreams[i].read();
				}
				n[i]+=N_CAPDATA;
			}
		}
		if (!photstream.empty()) { //data will be coming, let all else buffer
			burstphot: for (int j=0;j<64;j++) {
			#pragma HLS PIPELINE II=1
				photon_t phot;
				unsigned long x;
				phot=photstream.read();
				x=((unsigned long) phot.id) | ((unsigned long) phot.phase<<16)| ((unsigned long) phot.time<<32);
				phot_out[nphot+j]=x;
			}
			nphot+=64;
		}
//		done.read_nb(finished);
	}
}


//void maxi_writer(hls::stream<singleiqstream_t> postagein[N_MONITOR], hls::stream<photon_t> &photons,
//		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7) {
//#pragma HLS INTERFACE ap_ctrl_none port=return
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out0 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out1 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out2 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out3 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out4 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out5 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out6 offset=slave
//#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out7 offset=slave
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
////	capblock_t capblocks[N_MONITOR];
////	capblock_t capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7;
//
//	reader(postagein, photons, capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7);
//	write_maxi(capblocks0, capblocks1, capblocks2, capblocks3, capblocks4, capblocks5, capblocks6, capblocks7,
//			iq_out0, iq_out1, iq_out2, iq_out3, iq_out4, iq_out5, iq_out6, iq_out7);
//}


void maxi_writer(hls::stream<singleiqstream_t> postagein[N_MONITOR], hls::stream<photon_t> &photons,
		iq_t *iq_out0, iq_t *iq_out1, iq_t *iq_out2, iq_t *iq_out3, iq_t *iq_out4, iq_t *iq_out5, iq_t *iq_out6, iq_t *iq_out7,
		unsigned long *phot_out) {
#pragma HLS INTERFACE ap_ctrl_none port=return
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out0 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out1 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out2 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out3 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out4 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out5 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out6 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=iq_out7 offset=slave
#pragma HLS INTERFACE mode=m_axi depth=13500 max_widen_bitwidth=128 port=phot_out offset=slave

#pragma HLS INTERFACE mode=axis depth=13500 port=postagein
#pragma HLS INTERFACE mode=axis depth=13500 port=photons
#pragma HLS ARRAY_PARTITION dim=1 type=complete variable=postagein
//#pragma HLS INTERFACE mode=axis port=instream register_mode=off depth=13500 //register
//#pragma HLS INTERFACE mode=s_axilite port=monitor
//#pragma HLS INTERFACE mode=s_axilite port=alignment

#pragma HLS DATAFLOW

//	hls::stream<bool> done;
//#pragma HLS stream variable = done depth = 2

	const int MONDEPTH=2*N_CAPDATA;
	hls::stream<iq_t> monstreams[N_MONITOR];
#pragma HLS stream variable=monstreams depth=MONDEPTH
#pragma HLS ARRAY_PARTITION variable=monstreams complete

	hls::stream<photon_t> photstream;
#pragma HLS stream variable=photstream depth=MONDEPTH

	reader(postagein, photons, monstreams, photstream);
	write_maxi(monstreams, photstream, iq_out0, iq_out1, iq_out2, iq_out3, iq_out4, iq_out5, iq_out6, iq_out7,phot_out);
}




