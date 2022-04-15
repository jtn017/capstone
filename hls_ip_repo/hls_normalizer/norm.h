/*
 * Author: Ryan hiser
 * Project:
 * File: norm.h
 *
 * Description:
 */
#ifndef NORM_H_
#define NORM_H_

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>

typedef ap_fixed<16,1> din_t;
typedef ap_fixed<16,2> dout_t;

void normalizer(din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q);

#endif // NORM_H_
