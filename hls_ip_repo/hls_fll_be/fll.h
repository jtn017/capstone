/*
 * Author: Ryan Hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: fll.h
 *
 * Description: Header file for LOG Based AGC
 */
#ifndef CMPLX_FIR_H_
#define CMPLX_FIR_H_

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>
#include "cmplx_fir.h"

typedef float cnst_t;

const cnst_t CNST_PI     = 3.1415926535897932384626433832795;
const cnst_t CNST_TWO_PI = 6.283185307179586476925286766559;

void fll(float I, float Q, float *out_i, float *out_q);
float fll_energy_det(beu_re, beu_im, bel_re, bel_im);
float fll_loopf(float err);
void fll_nco(float loop_filt, float *nco_sin, float *nco_cos);
void cmpy_mult(float a, float b, float c, float d, float *y_re, float *y_im);

#endif // CMPLX_FIR_H_
