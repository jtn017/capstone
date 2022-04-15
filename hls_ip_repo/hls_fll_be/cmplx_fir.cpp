/*
 * Author: Ryan Hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: cmplx_fir.cpp
 *
 * Description: Implements Band Edge Filters using
 * 3 multiplier structure.
 */
#include "cmplx_fir.h"

void bandedge_upper(fir_in_t in_re, fir_in_t in_im, fir_out_t *out_re, fir_out_t *out_im){
    static fir_in_t tap_delay_re[NUM_TAPS];
    static fir_in_t tap_delay_im[NUM_TAPS];
    static fir_in_t tap_delay_comb[NUM_TAPS];

    static dint_t m_re, m_q, m_comb;
    float fb1_acc, fb2_acc, fb3_acc;

    cmplx_fir_loop: for(i = NUM_TAPS-1; i >= 0; i--)
    {
        // Filter Bank 1
        if(i == 0)
        {
            m_re = in_re;
            tap_delay_re[0] = m_re;
        }else{
            m_re = tap_delay_re[i-1];
            tap_delay_re[i] = tap_delay_re[i-1];
        }
        fb1_acc += m_re * (be_upper_coefs_re[i]-be_upper_coefs_im[i]);
        
        // Filter Bank 2
        if(i == 0)
        {
            m_comb = in_re - in_im;
            tap_delay_comb[0] = m_comb;
        }else{
            m_comb = tap_delay_comb[i-1];
            tap_delay_comb[i] = tap_delay_comb[i-1];
        }
        fb3_acc += m_comb * -be_upper_coefs_im[i];

        // Filter Bank 3
        if(i == 0)
        {
            m_im = in_im;
            tap_delay_im[0] = m_im;
        }else{
            m_re = tap_delay_im[i-1];
            tap_delay_im[i] = tap_delay_im[i-1];
        }
        fb3_acc += m_re * (be_upper_coefs_re[i]+be_upper_coefs_im[i]);

        // (XiYi-XqYq)+j(XiYq+XqYi)
        acc_re = fb1_acc + fb2_acc;
        acc_im = fb2_acc + fb3_acc;
    }

    *out_re = acc_re;
    *out_im = acc_im;
}

void bandedge_lower(){
    static fir_in_t tap_delay_re[NUM_TAPS];
    static fir_in_t tap_delay_im[NUM_TAPS];
    static fir_in_t tap_delay_comb[NUM_TAPS];

    static dint_t m_re, m_q, m_comb;
    float fb1_acc, fb2_acc, fb3_acc;

    cmplx_fir_loop: for(i = NUM_TAPS-1; i >= 0; i--)
    {
        // Filter Bank 1
        if(i == 0)
        {
            m_re = in_re;
            tap_delay_re[0] = m_re;
        }else{
            m_re = tap_delay_re[i-1];
            tap_delay_re[i] = tap_delay_re[i-1];
        }
        fb1_acc += m_re * (be_lower_coefs_re[i]-be_lower_coefs_im[i]);
        
        // Filter Bank 2
        if(i == 0)
        {
            m_comb = in_re - in_im;
            tap_delay_comb[0] = m_comb;
        }else{
            m_comb = tap_delay_comb[i-1];
            tap_delay_comb[i] = tap_delay_comb[i-1];
        }
        fb3_acc += m_comb * -be_lower_coefs_im[i];

        // Filter Bank 3
        if(i == 0)
        {
            m_im = in_im;
            tap_delay_im[0] = m_im;
        }else{
            m_re = tap_delay_im[i-1];
            tap_delay_im[i] = tap_delay_im[i-1];
        }
        fb3_acc += m_re * (be_lower_coefs_re[i]+be_lower_coefs_im[i]);

        // (XiYi-XqYq)+j(XiYq+XqYi)
        acc_re = fb1_acc + fb2_acc;
        acc_im = fb2_acc + fb3_acc;
    }

    *out_re = acc_re;
    *out_im = acc_im;
}
