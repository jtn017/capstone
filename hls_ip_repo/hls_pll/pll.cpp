#include "pll.h"

void pll(DATA_T IN_R, DATA_T IN_I, DATA_T *OUT_R, DATA_T *OUT_I)
{
//	// Pragmas
//	#pragma HLS PIPELINE II=25

	// Statically allocate registers
	static REG_T phs_int = 0.0; // Phase integral register
	static REG_T phs_acc = 0.0; // Phase accumulator register

	// Phase shift
	PHASE_T phase = 2*PI*phs_acc;
	COS_T phase_R = hls::cos(phase);
	COS_T phase_I = hls::sin(phase);
	DATA_T prod_R = (IN_R * phase_R) - (IN_I * phase_I);
	DATA_T prod_I = (IN_R * phase_I) + (IN_I * phase_R);

	// Phase error
	DATA_T phs_det_R = (prod_R > 0) ? 1 : -1;
	DATA_T phs_det_I = (prod_I > 0) ? 1 : -1;
	DATA_T phs_err_R = (phs_det_R *  prod_R) - (phs_det_I * -prod_I);
	DATA_T phs_err_I = (phs_det_R * -prod_I) + (phs_det_I *  prod_R);
	PHASE_T phs_err = hls::atan2(phs_err_I, phs_err_R);

	// Loop filter
	phs_int = KI * phs_int;
	DATA_T phs_prop  = KP * phs_err;
	DATA_T loop_filt = phs_prop + phs_int;

	// Phase accumulator
	phs_acc = phs_acc + loop_filt;

	// Assign output
	*OUT_R = prod_R;
	*OUT_I = prod_I;
}
