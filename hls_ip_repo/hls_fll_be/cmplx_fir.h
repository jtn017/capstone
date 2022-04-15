/*
 * Author: Ryan hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: cmplx_fir.h
 *
 * Description: Headder file for LOG Based AGC
 */
#ifndef CMPLX_FIR_H_
#define CMPLX_FIR_H_

#include <ap_int.h>
#include <ap_axi_sdata.h>
#include <hls_stream.h>
#include <hls_math.h>

typedef float fir_in_t;
typedef float fir_out_t;

void bandedge_upper(fir_in_t in_re, fir_in_t in_im, fir_out_t *out_re, fir_out_t *out_im);
void bandedge_lower(fir_in_t in_re, fir_in_t in_im, fir_out_t *out_re, fir_out_t *out_im);

#endif // CMPLX_FIR_H_
