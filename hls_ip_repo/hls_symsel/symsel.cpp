#include "symsel.h"

void symsel(corr_t thresh, din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, corr_t *corr, bool *max_vld)
{
#pragma HLS INTERFACE mode=ap_vld port=thresh
#pragma HLS INTERFACE mode=ap_vld port=in_i
#pragma HLS INTERFACE mode=ap_vld port=in_q
#pragma HLS INTERFACE mode=ap_vld port=out_i
#pragma HLS INTERFACE mode=ap_vld port=out_q
#pragma HLS INTERFACE mode=ap_none port=max_vld

	// Write your code here
	static din_t shift_re_reg[N*SPS];
    static din_t shift_im_reg[N*SPS];
    static din_t shift_ph_calc_i[N*SPS];
    static din_t shift_ph_calc_q[N*SPS];
    static corr_t max = 0;

#pragma HLS RESET variable=max
#pragma HLS PIPELINE II=2
#pragma HLS ARRAY_PARTITION variable=shift_re_reg dim=1 complete
#pragma HLS ARRAY_PARTITION variable=shift_im_reg dim=1 complete

	acc_t accum_re, accum_im;

	int i, j;
    int pa_idx;
    static bool flag = 0;

    //
    // --- Matched Filter ---
    //

    // Shift Register
	Shift_LOOP: for(i = (N*SPS)-1; i > 0; i--)
	{
#pragma HLS UNROLL
		shift_re_reg[i] = shift_re_reg[i-1];
        shift_im_reg[i] = shift_im_reg[i-1];
	}

	shift_re_reg[0] = in_i;
    shift_im_reg[0] = in_q;

    // Adder Tree
    pa_idx = N-1;
    accum_re = 0;
    accum_im = 0;

	MAC_LOOP: for(j = (N*SPS)-1; j > 0; j-=SPS)
	{
		accum_re += pa[pa_idx] ? (ap_int<17>)shift_re_reg[j] : (ap_int<17>)-shift_re_reg[j];
        accum_im += pa[pa_idx] ? (ap_int<17>)shift_im_reg[j] : (ap_int<17>)-shift_im_reg[j];
        pa_idx--; // 1 for every symbol
	}

	accum_re += pa[0] ? (ap_int<17>)shift_re_reg[0] : (ap_int<17>)-shift_re_reg[0];
    accum_im += pa[0] ? (ap_int<17>)shift_im_reg[0] : (ap_int<17>)-shift_im_reg[0];

    din_t ot_i = shift_re_reg[N*SPS-1];
    din_t ot_q = shift_im_reg[N*SPS-1];

    //
    // --- Magnitude Approximation Calculation ---
    //
    acc_t abs_re = hls::abs(accum_re);
    acc_t abs_im = hls::abs(accum_im);
    acc_t abs_max, abs_min;
	if(abs_re >= abs_im){
		abs_min = abs_re;
		abs_max = abs_im;
	}else{
		abs_min = abs_im;
		abs_max = abs_re;
	}

	corr_t mag_step1 = abs_max+(abs_min>>1);
	corr_t mag_step2 = mag_step1 * 15;
    corr_t magnitude = mag_step2 >> 4;

    static int cnt;

    if(magnitude > thresh){ // lets find the max
    	if(magnitude > max){
    		max = magnitude;
    		flag = 1;
    	}else{
    		flag = 0;
    	}
    }else{
    	flag = 0;
    }

	*max_vld = flag;
    *out_i = ot_i;
    *out_q = ot_q;
	*corr = magnitude;
}
