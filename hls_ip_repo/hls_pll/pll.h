#ifndef PLL_H_
#define PLL_H_

#include <ap_int.h>
#include <hls_math.h>

typedef ap_fixed<26-3+2,2> COS_T;
typedef ap_fixed<26,3> PHASE_T;
typedef ap_fixed<16,2> DATA_T;
typedef ap_fixed<22,8> REG_T;
typedef ap_fixed<26,3> ERR_T;


//#define BETTER

#ifdef BETTER
	const DATA_T KI = 0.1781;//0.000620528942;
	const DATA_T KP = 0.5011;//0.034917020688;
#else
	const DATA_T KI = 0.000620528942;
	const DATA_T KP = 0.034917020688;
#endif

const ap_ufixed<16,3> TWO_PI = 6.283185307179586476925286766559;
const ap_ufixed<16,1> DIV_2PI = 0.159154943091895;

void pll(bool init_v, DATA_T init_i, DATA_T init_q, DATA_T IN_R, DATA_T IN_I, DATA_T *OUT_R, DATA_T *OUT_I);

#endif
