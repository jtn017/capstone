#include "pll.h"
#include "hls_math.h"

void pll(DTYPE IN_R, DTYPE IN_I, DTYPE *OUT_R, DTYPE *OUT_I)
{
	// Statically allocate registers
	static DTYPE phs_int = 0.0; // Phase integral register
	static DTYPE phs_acc = 0.0; // Phase accumulator register

	// Internal variables
	DTYPE prod_R, prod_I;
	DTYPE phs_det_R, phs_det_I, phs_err_R, phs_err_I, phs_err;
	DTYPE phs_prop, loop_filt;

	// Phase shift (TODO: sinf/cosf eventually)
	prod_R = (IN_R * hls::cos(2*PI*phs_acc)) - (IN_I * hls::sin(2*PI*phs_acc));
	prod_I = (IN_R * hls::sin(2*PI*phs_acc)) + (IN_I * hls::cos(2*PI*phs_acc));

	// Phase error
	phs_det_R = (prod_R > 0) ? 1 : -1;
	phs_det_I = (prod_I > 0) ? 1 : -1;
	phs_err_R = (phs_det_R * prod_R)        - (phs_det_I * (prod_I * -1));
	phs_err_I = (phs_det_R * (prod_I * -1)) + (phs_det_I * prod_R);
	phs_err = hls::atan2(phs_err_I, phs_err_R)/(2*PI);

	// Loop filter
	phs_prop  = KP * phs_err;
	phs_int   = KI * phs_int;
	loop_filt = phs_prop + phs_int;

	// Phase accumulator
	phs_acc = phs_acc + loop_filt;

	// Assign output
	*OUT_R = prod_R;
	*OUT_I = prod_I;
}
