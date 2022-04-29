//-----------------------------------------------------------
// Project: UCSD_MAS - V2X Motorcycle HUD
// Authors: Ryan Hiser, Jacob Nguyen, Jorge Pacheco
// file: tec.cpp
// description: PFB Timing Error Correction
//-----------------------------------------------------------
#include "tec.h"
#include "coefs.h"

//
// Timing Error Correction (TEC)
//
void tec(data_t real, data_t imag, dout_t *out_real, dout_t *out_imag, bool *out_vld, unsigned int *bank)
{
#pragma HLS INTERFACE mode=ap_vld port=real
#pragma HLS INTERFACE mode=ap_vld port=imag

	static unsigned int scnt = 0;
	static unsigned int fbsel = 1;
	static dout_t y_t_i, y_t_q,dy_t_i, dy_t_q;

	dout_t mf_real, dmf_real;
	dout_t mf_imag, dmf_imag;
	bool mf_vld, dmf_vld;

	// Filter Banks
	mf_pfb (real, imag, fbsel-1, &mf_real,  &mf_imag);
	dmf_pfb(real, imag, fbsel-1, &dmf_real, &dmf_imag);
	
	if(scnt == 0){
		y_t_i = mf_real;
		y_t_q = mf_imag;
		dy_t_i = dmf_real;
		dy_t_q = dmf_imag;
		scnt++;
	}else if(scnt == SPS-1){
		// TEC LOOP ERROR Calculation
		tec_loopf(y_t_i, y_t_q, dy_t_i, dy_t_q, &fbsel);
		scnt=0;
	}else{
		scnt++;
	}

	// Loop Filter
	*out_real = mf_real;
	*out_imag = mf_imag;
	*out_vld = (scnt == 0);
	*bank = fbsel;
}

//
// Timing Matched Filter Poly-Phase Filter Bank
//
void mf_pfb(data_t real, data_t imag, unsigned int bank_sel, dout_t *out_real, dout_t *out_imag)
{

	// Static Variables
    static data_t sr_i[NUMCOEFS]; // Input Data Shift Registers
    static data_t sr_q[NUMCOEFS]; // Input Data Shift Registers
    static dout_t mult_i[NUMCOEFS];   // register bank for multiply result (I)
    static dout_t mult_q[NUMCOEFS];   // register bank for multiply result (Q)

#pragma HLS ARRAY_PARTITION variable=sr_i complete dim=0
#pragma HLS ARRAY_PARTITION variable=sr_q complete dim=0

    // Non-Static Variables
	dout_t summer_i = 0;
	dout_t summer_q = 0;

    // Sample Shift Register Loading and Shifting
	shift_loop: for(int i = NUMCOEFS-1; i >= 0; i--) {
#pragma HLS UNROLL factor=4
		//Data Shift Register
		if(i == 0){
			sr_i[0] = real; // shift in new data
			sr_q[0] = imag; // shift in new data
		}
		else{
			sr_i[i] = sr_i[i-1]; // shift in repeats of data
			sr_q[i] = sr_q[i-1]; // shift in repeats of data
		}
	}

	macloop: for(int k = NUMCOEFS-1; k >= 0; k--) {
#pragma HLS UNROLL factor=4
		mult_i[k]= (sr_i[k] * mf_rom[bank_sel][k]);
		mult_q[k]= (sr_q[k] * mf_rom[bank_sel][k]);
		summer_i += mult_i[k];
		summer_q += mult_q[k];
	}

	// Output Samples
	*out_real = summer_i;
	*out_imag = summer_q;
}


//
// Timing Derivative Matched Filter Poly-Phase Filter Bank
//
void dmf_pfb(data_t real, data_t imag,unsigned int bank_sel, dout_t *out_real, dout_t *out_imag)
{

	// Static Variables
    static data_t sr_i[NUMCOEFS];
    // Input Data Shift Registers
    static data_t sr_q[NUMCOEFS]; // Input Data Shift Registers
    static dout_t mult_i[NUMCOEFS];   // register bank for multiply result (I)
    static dout_t mult_q[NUMCOEFS];   // register bank for multiply result (Q)

#pragma HLS ARRAY_PARTITION variable=sr_i complete dim=0
#pragma HLS ARRAY_PARTITION variable=sr_q complete dim=0

    // Non-Static Variables
	dout_t summer_i = 0;
	dout_t summer_q = 0;

    // Sample Shift Register Loading and Shifting
	shift_loop: for(int i = NUMCOEFS-1; i >= 0; i--) {
#pragma HLS UNROLL factor=4
		//Data Shift Register
		if(i == 0){
			sr_i[0] = real; // shift in new data
			sr_q[0] = imag; // shift in new data
		}
		else{
			sr_i[i] = sr_i[i-1]; // shift in repeats of data
			sr_q[i] = sr_q[i-1]; // shift in repeats of data
		}
	}

	macloop: for(int k = NUMCOEFS-1; k >= 0; k--) {
#pragma HLS UNROLL factor=4
		mult_i[k]= (sr_i[k] * dmf_rom[bank_sel][k]);
		mult_q[k]= (sr_q[k] * dmf_rom[bank_sel][k]);
		summer_i += mult_i[k];
		summer_q += mult_q[k];
	}


	// Output Samples
	*out_real = summer_i;
	*out_imag = summer_q;
}

//
// Timing Loop Filter (fixed precision)
//
#ifdef BIT_ACCURATE
	void tec_loopf(dout_t y_re, dout_t y_im, dout_t dy_re, dout_t dy_im,unsigned int *fbsel)
	{
		static ap_fixed<32,8> integrator = 0;
		static ap_fixed<32,8> accum = 1;

		//float det_t = y_re*dy_re; // Option 0
		ap_fixed<32,8> det = ((y_re*dy_re)+(y_im*dy_im))>>2; // Option 1 do a shift for divide by two
		ap_fixed<32,8> d_error = det;

		// Multiply by Constants
		ap_fixed<32,8> p_mult = kp * d_error;
		ap_fixed<32,8> i_mult = ki * d_error;

		// Summations
		ap_fixed<32,8> int_reg = integrator + i_mult;
		ap_fixed<32,8> fltr = int_reg + p_mult; // loop filter output

		// timing Accumulator
		accum = accum +  fltr;
		integrator = int_reg; // store for next iteration

		// Accumulator Overflow Check
		if(accum > BANKS+1){
			accum = accum - BANKS;
		}else if (accum < 1){
			accum = accum + BANKS;
		}


		// Update outputs
		*fbsel = (unsigned int) hls::floor(accum);
	}

//
// Timing Loop Filter (FLOAT)
//
#else
	void tec_loopf(dout_t y_re, dout_t y_im, dout_t dy_re, dout_t dy_im,unsigned int *fbsel)
	{
#pragma HLS PIPELINE II=50
		static float integrator = 0;
		static float accum = 1;

		//float det_t = y_re*dy_re; // Option 0
		float det = ((y_re*dy_re)+(y_im*dy_im))*0.5; // Option 1 do a shift for divide by two
		float d_error = det;

		// Multiply by Constants
		float p_mult = kp * d_error;
		float i_mult = ki * d_error;

		// Summations
		float int_reg = integrator + i_mult;
		float fltr = int_reg + p_mult; // loop filter output

		// timing Accumulator
		accum = accum +  fltr;
		integrator = int_reg; // store for next iteration

		// Accumulator Overflow Check
		if(accum > BANKS+1){
			accum = accum - BANKS;
		}else if (accum < 1){
			accum = accum + BANKS;
		}

		// Update outputs
		*fbsel = (unsigned int) hls::floorf(accum);
	}
#endif
