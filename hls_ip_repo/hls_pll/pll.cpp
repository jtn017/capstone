#include "pll.h"
#include "hls_math.h"

void pll(DTYPE IN_R, DTYPE IN_I, DTYPE *OUT_R, DTYPE *OUT_I)
{
//	// Pragmas
//	#pragma HLS PIPELINE II=25

	// Statically allocate registers
	static DTYPE phs_int = 0.0; // Phase integral register
	static DTYPE phs_acc = 0.0; // Phase accumulator register

	// Phase shift
	DTYPE phase, prod_R, prod_I;
	DTYPE phase_R, phase_I;
	phase = 2*PI*phs_acc;
	phase_R = hls::cos(phase);
	phase_I = hls::sin(phase);
	complex_multiply<DTYPE>(IN_R, IN_I, phase_R, phase_I, &prod_R, &prod_I);

	// Phase error
	DTYPE phs_det_R, phs_det_I;
	DTYPE conj_I;
	DTYPE phs_err_R, phs_err_I, phs_err;
	phs_det_R = sign<DTYPE>(prod_R);
	phs_det_I = sign<DTYPE>(prod_I);
	conj_I = -prod_I;
	complex_multiply<DTYPE>(phs_det_R, phs_det_I, prod_R, conj_I, &phs_err_R, &phs_err_I);
	phs_err = hls::atan2(phs_err_I, phs_err_R);

	// Loop filter
	DTYPE phs_prop, loop_filt;
	phs_prop  = KP * phs_err;
	phs_int   = KI * phs_int;
	loop_filt = phs_prop + phs_int;

	// Phase accumulator
	phs_acc = phs_acc + loop_filt;

	// Assign output
	*OUT_R = prod_R;
	*OUT_I = prod_I;
}
