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

typedef ap_int<16> din_t;
typedef ap_int<16> dout_t;
typedef uint16_t slice_t;
typedef ap_fixed<24,13> gain_t;
typedef ap_int<21> mag_t;
typedef ap_fixed<32,16> agc_t;
typedef ap_ufixed<26,16> ref_t;
typedef ap_fixed<16, 1> cnst_t;
typedef ap_ufixed<16, 8> lut_t;


#include "lut.h"

const cnst_t alpha = 0.1;

void agc(ref_t pref, din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, gain_t *dbg);

#endif // AGC_H_
