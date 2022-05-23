#include <ap_int.h>

void rx_interface
(
unsigned int in0,
unsigned int in1,
unsigned int in2,
ap_uint<32> *ctrl,
ap_uint<32> *threshold,
ap_uint<26> *pow_ref
)
{
#pragma HLS INTERFACE s_axilite port=in0 bundle=input
#pragma HLS INTERFACE s_axilite port=in1 bundle=input
#pragma HLS INTERFACE s_axilite port=in2 bundle=input

//#pragma HLS INTERFACE ap_none port=ctrl
//#pragma HLS INTERFACE ap_none port=threshold
//#pragma HLS INTERFACE ap_none port=pow_ref

	*ctrl = in0;
	*threshold = in1;
	*pow_ref = in2;
}
