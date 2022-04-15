/*
 * Author: Ryan Hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: agc.cpp
 *
 * Description: LOG Based AGC using LUT and
 * magnitude approximation.
 *
 */
#include "agc.h"

void agc(ref_t pref, short in_i, short in_q, short *out_i, short *out_q, gain_t *dbg)
{
	static gain_t log2_gain = 1;
	agc_t abs_i, abs_q;
	agc_t abs_max, abs_min;
	agc_t out_i_r, out_q_r;

	// Do exp2(log2_gain)
	slice_t gain_slice = (uint16_t) log2_gain.range(14,3);
	gain_t gain;
	slice_t lut_address = gain_slice + MIDDLE; // Look up table stores [-,+]
	gain = EXP2_LUT[lut_address];

	// Apply Gain
	out_i_r = in_i * gain;
	out_q_r = in_q * gain;

	// Calculate Magintude (via approximation)
	abs_i = myAbs<agc_t>(out_i_r);
	abs_q = myAbs<agc_t>(out_q_r);

	if(abs_i >= abs_q){
		abs_min = abs_i;
		abs_max = abs_q;
	}else{
		abs_min = abs_q;
		abs_max = abs_i;
	}

	mag_t mag_step1 = abs_max+(abs_min>>1);
	mag_t mag_step2 = mag_step1 * 15;
	agc_t mag = mag_step2 >> 4;

	// Calculate LOG2(mag) (use hls function)
	agc_t hlslog = hls::log2(mag);
	agc_t err = pref - hlslog; // using log(a/b) = log(a) - log(b)

	// Accumulator
	//gain_t accum = log2_gain + alpha*err; // could remove extra mult by doing .125 instead of .1
	gain_t accum = log2_gain + alpha>>3; // could remove extra mult by doing .125 instead of .1

	// Limit
	if(accum > 7.9961)
		log2_gain = 7.9961;
	else if(accum < -8)
		log2_gain = -8;
	else
		log2_gain = accum;

	// Assign Outputs
	*out_i = out_i_r;
	*out_q = out_q_r;
	*dbg = log2_gain;

}
