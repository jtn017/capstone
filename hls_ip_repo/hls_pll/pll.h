#ifndef PLL_H_
#define PLL_H_

typedef float DTYPE;

const DTYPE KI = 0.000621;
const DTYPE KP = 0.034917;
const DTYPE PI = 3.141592653589;

void pll(DTYPE IN_R, DTYPE IN_I, DTYPE *OUT_R, DTYPE *OUT_I);

#endif
