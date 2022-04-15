/*
 * Author: Ryan Hiser
 * Project:
 * File: norm.cpp
 *
 * Description:
 *
 */

#include "norm.h"

void normalizer(din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q)
{
#pragma HLS PIPELINE II=25
	ap_fixed<15,2> outsin, outcos; // W-I+2,2
	ap_fixed<16,3> phase = hls::atan2(in_q,in_i);
	hls::sincos<16,3>(phase, &outsin, &outcos);

	*out_i = outcos;
	*out_q = outsin;
}
