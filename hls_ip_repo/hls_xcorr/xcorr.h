/*
 * Author: 
 * Project: Capstone V2X Motorcycle HUD
 * File: xcorr.h
 *
 * Description: Header file for LOG Based AGC
 */
#ifndef XCORR_H_
#define XCORR_H_

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>

#define BIT_ACCURATE

// Data Types
#ifdef BIT_ACCURATE
	typedef ap_int<16> din_t;
	typedef ap_int<16> dout_t;
	typedef ap_uint<16+6> corr_t;
	typedef ap_int<16+6> acc_t; // 64 Additions should grow by 6 bits
	typedef ap_fixed<32,24> cmpy_t;
	typedef ap_fixed<32,16> avg_t;
#else
	typedef short din_t;
	typedef short dout_t;
	typedef float corr_t;
	typedef float acc_t; // 64 Additions should grow by 6 bits
	typedef float cmpy_t;
	typedef float avg_t;
#endif

// Constants
const unsigned int PN_LEN = 32;
const unsigned int N = PN_LEN*2; // length of preamble sequence
const unsigned int SPS = 8; // samples per symbol

const bool pa[N] = {0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1};

// Function Definitions
void myCmpy(cmpy_t a_re, cmpy_t b_im,cmpy_t c_re, cmpy_t d_im,cmpy_t *o_re,cmpy_t *o_im);
void xcorr(din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, corr_t *corr, bool *max_vld,cmpy_t *to_atan_i,cmpy_t *to_atan_q);

#endif // XCORR_H_
