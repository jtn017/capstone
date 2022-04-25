#ifndef PLL_H_
#define PLL_H_

#include "ap_fixed.h"

//typedef ap_fixed<15,2> COS_TYPE;
//typedef ap_fixed<16,3> ATAN_TYPE;
typedef float DTYPE;

const DTYPE KI = 0.000620528942;
const DTYPE KP = 0.034917020688;
const DTYPE PI = 3.141592653589;

template <typename T>
void complex_multiply(T a, T b, T c, T d, T *o_re, T *o_im)
{
    // Use 3 multiplies
    T ac = a * c;
    T bd = b * d;
    *o_re = ac - bd;
    *o_im = (a + b) * (c + d) - ac - bd;
}

template <typename S>
S sign(S input)
{
	return (input > 0) ? 1 : -1;
}

void pll(DTYPE IN_R, DTYPE IN_I, DTYPE *OUT_R, DTYPE *OUT_I);

#endif
