#ifndef PLL_H_
#define PLL_H_

#include <ap_int.h>
#include <hls_math.h>

typedef ap_fixed<16,2> COS_T;
typedef ap_fixed<16,3> PHASE_T;
typedef ap_fixed<32,16> DATA_T;
typedef ap_fixed<32,16> REG_T;

const DATA_T KI = 0.000620528942;
const DATA_T KP = 0.034917020688;
const DATA_T PI = 3.141592653589;

void pll(DATA_T IN_R, DATA_T IN_I, DATA_T *OUT_R, DATA_T *OUT_I);

#endif
