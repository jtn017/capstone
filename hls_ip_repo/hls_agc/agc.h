/*
 * Author: Ryan hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: agc.h
 *
 * Description: Headder file for LOG Based AGC
 */
#ifndef AGC_H_
#define AGC_H_

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>

typedef short din_t;
typedef short dout_t;
typedef uint16_t slice_t;
typedef ap_fixed<24,13> gain_t;
typedef ap_fixed<32,16> ref_t;
typedef ap_fixed<32,16> agc_t;
typedef ap_fixed<16, 1> cnst_t;
typedef ap_ufixed<16, 8> lut_t;
typedef ap_int<24> mag_t;

#include "lut.h"

template <typename T>
T myAbs(T x)
{
	return (x >= 0)? x : (T) (-x);
}

const cnst_t alpha = 0.1;

void agc(ref_t pref, din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, gain_t *dbg);

#endif // AGC_H_
