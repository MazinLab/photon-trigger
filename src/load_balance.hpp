/*
 * Copyright 2019 Xilinx, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef XF_UTILS_HW_STREAM_N1_LB
#define XF_UTILS_HW_STREAM_N1_LB

inline ap_uint<4> countOnes(ap_uint<4> y) {
#pragma HLS inline

    ap_uint<4> x0 = y;
    ap_uint<4> x1 = (x0 & 0x5) + ((x0 & 0xa) >> 1);
    ap_uint<4> x2 = (x1 & 0x3) + ((x1 & 0xc) >> 2);
    return x2;
}

/**
 * @brief read the data from _NStrm streams, skip the empty streams
 * collect  _NStrm data(_WInStrm bits) from input streams and output
 * one(_NStrm*_NStrm bits)
 * @tparam _WInStrm input stream width.
 * @tparam _NStrm number of input streams.
 *
 * @param istrms input data streams.
 * @param e_istrms end flag streams for input data.
 * @param left_n the number of available data in last buf_lcm_strm.
 * @param buf_lcm_strm output stream.
 * @param e_buf_lcm_strm end flag stream.
 */
template <int _WInStrm, int _NStrm>
void stream_n_to_one_read_lb(hls::stream<ap_uint<_WInStrm> > istrms[N_PHASE],
                             hls::stream<ap_uint<_WInStrm * _NStrm> >& buf_lcm_strm) {
//#pragma HLS INTERFACE mode=ap_ctrl_none port=return
    const int buf_width = _WInStrm * _NStrm;
    const int num_in = _NStrm;
    const int up_nstrm = UpBound<_NStrm>::value;
    ap_uint<2 * buf_width> buff_a = 0;
    ap_uint<buf_width> buff_b = 0;
    const ap_uint<_NStrm> ends = -1;
    ap_uint<_NStrm> val = 0;
    ap_uint<_NStrm> bak_val = 0;
    int base = 0;
    ap_uint<_NStrm * _WInStrm> tmpb[_NStrm];
#pragma HLS ARRAY_PARTITION variable = tmpb complete
    ap_uint<_WInStrm> ttm[_NStrm];
#pragma HLS ARRAY_PARTITION variable = ttm complete

#if !defined(__SYNTHESIS__) && XF_UTIL_STRM_1NRR_DEBUG == 1
    std::cout << std::dec << std::endl;
    std::cout << "_NStrm =" << _NStrm << std::endl;
    std::cout << "_WInStrm =" << _WInStrm << std::endl;
    std::cout << "Win*_NStrm =" << buf_width << std::endl;
    std::cout << "num_in =" << num_in << std::endl;
    std::cout << "UpBound<_NStrm> =" << up_nstrm << std::endl;
#endif


//    while (true) {
//#pragma HLS pipeline II = 1
    for(int group=0;group<512;group++) {
#pragma HLS pipeline II = 1 rewind
        for (int i = 0; i < _NStrm; ++i) {
#pragma HLS unroll
            bool vl;
            vl=!istrms[i].empty();

            val[i] = vl; // flag of available data

            // neither  empty nor finished stream, read it, or default zero
            ttm[i] = vl ? istrms[i].read() : (ap_uint<_WInStrm>)0;

#if !defined(__SYNTHESIS__) && XF_UTIL_STRM_1NRR_DEBUG == 1
            std::cout << std::dec << "i= " << i << " ttm[i]= " << ttm[i] << std::endl;
            std::cout << std::dec << "i= " << i << " val[i]= " << val[i] << std::endl;
#endif
        } // for

        // move the available data to "the right position"
        // if a stream is empty or finished, there is no point in moving its data
        // since its data is 0
        tmpb[0] = ttm[0];
        for (int i = 1; i < _NStrm; ++i) {
#pragma HLS unroll
            ap_uint<up_nstrm> v = val.range(i - 1, 0);
            int ones = countOnes(v); // it's similar to round robin  if ones always is i.
            int p = ones;            // index of tmpb[i].range(), where istrm[i] is stored if it
                                     // is not empty
            ap_uint<_NStrm* _WInStrm> d = ttm[i];
            tmpb[i] = d << (p * _WInStrm);
#if !defined(__SYNTHESIS__) && XF_UTIL_STRM_1NRR_DEBUG == 1
            std::cout << std::hex << "v= " << v << " d= " << d << " tmpb[i]=" << tmpb[i] << std::endl;
            std::cout << std::dec << "i= " << i << " p= " << p << " p*_WInStrm=" << p * _WInStrm << std::endl;
#endif
        } // for

        buff_b = 0;
        for (int i = 0; i < _NStrm; ++i) {
#pragma HLS pipeline II = 1
            // merge data,
            buff_b |= tmpb[i];
        }

        int un = countOnes(val); // how many new data are collected to buffer at this time

        // accumulate data
        if (un > 0) {
            buff_a.range((base + un) * _WInStrm - 1, base * _WInStrm) = buff_b.range(un * _WInStrm - 1, 0);
        }

        // output one data
        if (base + un >= num_in) {
            //  the size of buff_a is big enough
            base = base + un - num_in;
            buf_lcm_strm.write(buff_a.range(buf_width - 1, 0));
            // move the remaining data to the start position
            buff_a = buff_a >> buf_width;
        } else {
            // accumulated data
            base += un;
        } // if-else

    } // while

}

/**
 * @brief buffer data to solve different input and output width.
 *
 * input  _WInStrm * _NStrm bit --> output lcm(_WInStrm*_NStrm, WOutStrm) bits
 * @tparam _WInStrm input stream width.
 * @tparam _WOutStrm output stream width.
 * @tparam _NStrm number of input streams.
 *
 * @param buf_n_strm input data stream.
 * @param buf_lcm_strm output stream.
 */
template <int _WInStrm, int _WOutStrm, int _NStrm>
void stream_n_to_one_collect_lb(hls::stream<ap_uint<_WInStrm * _NStrm> >& buf_n_strm,
                                hls::stream<ap_uint<LCM<_WInStrm * _NStrm, _WOutStrm>::value> >& buf_lcm_strm) {
    const int buf_width = LCM<_WInStrm * _NStrm, _WOutStrm>::value;
    const int num_in = buf_width / _WInStrm;
    const int count_in = num_in / _NStrm;
    ap_uint<buf_width> inner_buf;
    int p = 0;
    int pos = 0;
//    while (true) {
//#pragma HLS pipeline II = 1
	for(int group=0;group<512;group++) {
#pragma HLS pipeline II = 1 rewind
        int low = pos;
        pos += _NStrm * _WInStrm;
        inner_buf.range(pos - 1, low) = buf_n_strm.read();
        if (p + 1 == count_in) {
            buf_lcm_strm.write(inner_buf);
            p = 0;
            pos = 0;
        } else
            p++;
    } // while

}

/**
 * @brief output data sequentially from input stream with big width.
 *
 * output buf_width=lcm(_WInStrm*_NStrm, WOutStrm) bits in buf_width/_WOutStrm
 * cycles
 *
 * @tparam _WInStrm input stream width.
 * @tparam _WOutStrm output stream width.
 * @tparam _NStrm number of input streams.
 *
 * @param buf_lcm_strm input data stream.
 * @param ostrm output stream.
 */
//void stream_n_to_one_distribute_lb(hls::stream<ap_uint<LCM<_WInStrm * _NStrm, _WOutStrm>::value> >& buf_lcm_strm,
void stream_n_to_one_distribute_lb(hls::stream<ap_uint<WInStrm * _NStrm >& buf_lcm_strm,
                                   hls::stream<ap_uint<_WOutStrm> >& ostrm) {
    const int buf_width = LCM<_WInStrm * _NStrm, _WOutStrm>::value;
    const int num_out = buf_width / _WOutStrm;

    ap_uint<buf_width> inner_buf;
    int low = 0;
    int up = 0;
    int c = num_out;
    unsigned int up_pos = -1;
//    while (true) {
//#pragma HLS pipeline II = 1
	for(int group=0;group<512*_NStrm;group++) {
#pragma HLS pipeline II = 1 rewind
        if (c == num_out) {
            inner_buf = buf_lcm_strm.read();
            c = 0;
        } // if

        if ((c + 1) * _WOutStrm <= up_pos) {
            ostrm.write(inner_buf.range((c + 1) * _WOutStrm - 1, c * _WOutStrm));
        } // if
        c++;
    } // while
}

/**
 * @brief stream distribute, skip to read the empty input streams.
 *
 * @tparam _WInStrm input stream width.
 * @tparam _WOutStrm output stream width.
 * @tparam _NStrm number of input streams.
 *
 * @param istrms input data streams.
 * @param ostrm output data stream.
 */
template <int _WInStrm, int _WOutStrm, int _NStrm>
void stream_n_to_one_load_balance(hls::stream<ap_uint<_WInStrm> > istrms[_NStrm],
                                  hls::stream<ap_uint<_WOutStrm> >& ostrm) {

    const int buf_width = LCM<_WInStrm * _NStrm, _WOutStrm>::value;
    hls::stream<ap_uint<_WInStrm * _NStrm> > buf_n_strm;
#pragma HLS stream variable = buf_n_strm depth = 8

    hls::stream<ap_uint<buf_width> > buf_lcm_strm;
#pragma HLS stream variable = buf_lcm_strm depth = 8

#pragma HLS dataflow

    /*  read data   : read data from input streams, output _NStrm * _WInStrm bits
     *  collect data: buffer  buf_width=lcm(_WInStrm*_NStrm, _WOutStrm) bits and
     * output them.
     *  distribute  : output buf_width/_WOutStrm  data when read buf_width bits
     * once
     *
     * least common mutiple(lcm) is used for solving the difference between  the
     * input width and output width
     *
     * */
    stream_n_to_one_read_lb<_WInStrm, _NStrm>(istrms, left_n, buf_n_strm);
    stream_n_to_one_collect_lb<_WInStrm, _WOutStrm, _NStrm>(buf_n_strm, buf_lcm_strme_buf_lcm_strm);
    stream_n_to_one_distribute_lb<_WInStrm, _WOutStrm, _NStrm>(buf_lcm_strm, ostrm);
}


/**
 * @brief stream distribute, skip to read the empty input streams.
 *
 * @tparam _TIn the type of streams.
 * @tparam _NStrm number of input streams.
 *
 * @param istrms input data streams.
 * @param ostrm output data stream.
 */
template <typename _TIn, int _NStrm>
void stream_n_to_one_load_balance_type(hls::stream<_TIn> istrms[_NStrm],
                                       hls::stream<_TIn>& ostrm) {

    int id = 0;
    while (true) {
#pragma HLS pipeline II = 1
		bool em = istrms[id].empty();
		// read data if not end and not empty
		if (!em) {
			_TIn data = istrms[id].read();
			ostrm.write(data);
		}
		id = (id + 1) == _NStrm ? 0 : (id + 1);
    }
}



