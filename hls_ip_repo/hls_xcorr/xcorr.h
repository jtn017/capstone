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
	typedef short din_t;
	typedef short dout_t;
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

//const bool pa[N] = {1,1,1,1,1,1,0,1,0,1,0,1,1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,0,1,0,0,0};
const bool pa[N] = {0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1,0,0,0,1,0,0,1,0,1,1,0,1,1,1,0,1,1,0,0,1,1,0,1,0,1,0,1,1,1,1,1,1};

// Templates
template <typename T>
T myAbs(T x)
{
	return (x >= 0)? x : (T) (-x);
}

template <typename T>
void myCmpy(T a_re, T b_im,T c_re, T d_im,T *o_re,T *o_im)
{
	// Use 3 multiplies
	T ac = a_re	*c_re;
	T bd = b_im	*d_im;
	*o_re = ac-bd;
	*o_im = (a_re+b_im)*(c_re+d_im)-ac-bd;
}

// Function Definitions
void xcorr(din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, corr_t *corr, bool *max_vld,cmpy_t *to_atan_i,cmpy_t *to_atan_q);

#endif // XCORR_H_
