/*
 * Author: Ryan Hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: fll.cpp
 *
 * Description: FLL - Frequency Correction 
 * Using bandedge filters as described in
 * fred harris' "Let’s Assume the System Is Synchronized"
 *
 * Figure 18.
 */
#include "fll.h"

void fll(float I, float Q, float *out_i, float *out_q){

    float beu_re, beu_im;
    float bel_re, bel_im;
    float nco_sin, nco_cos;

    // Frequency Adjustment (Phasor) (complex Multiply)
    cmpy_mult(nco_cos, nco_sin, in_i, in_q, &temp_re, &temp_im);// (a+ib)(c+id)

    // Assign outputs
    *out_i = temp_re;
    *out_q = temp_im;

    // Band Edge Filters
    bandedge_upper(temp_re, temp_im, &beu_re, &beu_im);
    bandedge_lower(temp_re, temp_im, &bel_re, &bel_im);

    // Energy Difference
    float err = fll_energy_det(beu_re, beu_im, bel_re, bel_im);

    // Lock Detection
    
    // Loop Filter
    float loop_out = fll_loopf(err);

    // Phase Accumulator & DDS Generation
    fll_nco(loop_out, &nco_sin, &nco_cos)

}

float fll_energy_det(beu_re, beu_im, bel_re, bel_im)
{
    //(a+ib)(c+id)
    float a,b,c,d;

    // (beu+bel) = (a+c)+(b+d)
    // (beu-bel) = (a-c)+(b-d)
    float a = beu_re+beu_re;
    float c = beu_re-bel_re;

    float b = beu_im+beu_im;
    float d = beu_im-bel_im;

    // xy = (ac-bd)+j(ad+bc)
    // re{xy} = (ac-bd)
    float err = (a*c)+(b*d); // + due to conj(y)
    return err;
}

// See figure 18 (Loop Filter)
float fll_loopf(float err)
{
    float kp_err = kp*err;
    float ki_err = ki*err;

    static float accum = 0;

    accum = accum + ki_err;

    return accum+kp_err;
}

void fll_nco(float loop_filt, float *nco_sin, float *nco_cos)
{
    // Accumulator
    static float accum = 0;
    accum = accum + loop_filt;

    if(accum >= CNST_PI)
        accum = accum - CNST_TWO_PI;
    else if(accum < -CNST_PI)
        accum = accum + CNST_TWO_PI;
    
    // NCO
    *nco_sin = hls::sin(accum);
    *nco_cos = hls::cos(accum);
}

// 3-Mult Structure (Karatsuba multiplication)
// (a+ib)(c+id)
void cmpy_mult(float a, float b, float c, float d, float *y_re, float *y_im)
{
    float ac = a*c;
    float bd = b*d;
    float ab_cd = (a+b)*(c+d);

    *y_re = ac-bd;
    *y_im = ab_cd - ac - bd;
}