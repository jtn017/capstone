//-----------------------------------------------------------
// Project: UCSD_MAS - V2X Motorcycle HUD
// Authors: Ryan Hiser, Jacob Nguyen, Jorge Pacheco
// file: tec.h
// description: PFB Timing Error Correction
//-----------------------------------------------------------

#ifndef TEC_H_
#define TEC_H_

#include <stdint.h>
#include <ap_int.h>
#include "ap_shift_reg.h"
#include "hls_math.h"

#define SPS 8
#define LOG2_SPS 3


//#define BIT_ACCURATE

#ifdef BIT_ACCURATE
	typedef ap_fixed<1,18> coef_t;
	typedef ap_int<16> data_t;
	typedef ap_int<32> dout_t;
#else
	typedef float coef_t;
	typedef float data_t;
	typedef float dout_t;
#endif

const float ki = 0.002731550096641;
const float kp = 0.614814780101614;


void mf_pfb(data_t real, data_t imag, unsigned int bank_sel, dout_t *out_real, dout_t *out_imag);
void dmf_pfb(data_t real, data_t imag, unsigned int bank_sel, dout_t *out_real, dout_t *out_imag);
void tec_loopf(dout_t y_re, dout_t y_im, dout_t dy_re, dout_t dy_im,unsigned int *fbsel);

void tec(data_t real, data_t imag, dout_t *out_real, dout_t *out_imag, bool *out_vld, unsigned int *bank);

#endif // TEC_H_
