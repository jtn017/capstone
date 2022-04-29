#include "xcorr.h"

const corr_t THRESH = 20000;

void xcorr(din_t in_i, din_t in_q, dout_t *out_i, dout_t *out_q, corr_t *corr, bool *max_vld,cmpy_t *to_atan_i,cmpy_t *to_atan_q)
{
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
    static avg_t avg_i = 0;
    static avg_t avg_q = 0;

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

    //
    // --- Magnitude Approximation Calculation ---
    //
    acc_t abs_re = hls::abs(accum_re);
    acc_t abs_im = hls::abs(accum_im);
    corr_t magnitude = abs_re + abs_im;
    static int cnt;

    if(magnitude > THRESH){ // lets find the max
    	if(magnitude > max)
    		max = magnitude;
    	flag = 1;
    }else{
    	flag = 0;
    }

    if(flag == 1){ // posedge
    	cnt = 0;
        avg_i = 0;
        avg_q = 0;
    }else{
		cnt++;
    }

    if(cnt > 800*8)
    	max = 0;

    //
    // Z2 = mean(conj(rx_signal(N1+(0:N-1))) .* rx_signal(N2+(0:N-1)));
    //
    din_t temp_i = shift_re_reg[(N-PN_LEN)*SPS-1];
    din_t temp_q = shift_im_reg[(N-PN_LEN)*SPS-1];
    din_t ot_i = shift_re_reg[N*SPS-1];
    din_t ot_q = shift_im_reg[N*SPS-1];

    cmpy_t c1,c2;
    myCmpy( (cmpy_t)ot_i, (cmpy_t)-ot_q, (cmpy_t)temp_i, (cmpy_t)temp_q, &c1, &c2);
    avg_i += c1;
    avg_q += c2;

    //
    // --- Assign Outputs ---
    //
	if(cnt == N*SPS-1){ // need to understand
		#ifdef BIT_ACCURATE
			*to_atan_i = avg_i >> 8;
			*to_atan_q = avg_q >> 8;
		#else
			*to_atan_i = avg_i/256;
			*to_atan_q = avg_q/256;
		#endif
	}

	*max_vld = flag;
    *out_i = ot_i;
    *out_q = ot_q;
	*corr = magnitude;
}


void myCmpy(cmpy_t a_re, cmpy_t b_im,cmpy_t c_re, cmpy_t d_im,cmpy_t *o_re,cmpy_t *o_im)
{
	// Use 3 multiplies
	cmpy_t ac = a_re*c_re;
	cmpy_t bd = b_im*d_im;
	*o_re = ac-bd;
	*o_im = (a_re+b_im)*(c_re+d_im)-ac-bd;
}
