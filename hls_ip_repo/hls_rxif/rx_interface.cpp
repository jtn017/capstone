#include <ap_int.h>

void rx_interface
(
unsigned int in0,
unsigned int in1,
unsigned int in2,
unsigned int in3,
unsigned int in4,
ap_uint<32> *ctrl,
ap_uint<32> *threshold,
ap_uint<26> *pow_ref,
ap_uint<32> *store_dly,
ap_uint<3>  *time_sel
)
{
#pragma HLS INTERFACE s_axilite port=in0 bundle=busa
#pragma HLS INTERFACE s_axilite port=in1 bundle=busa
#pragma HLS INTERFACE s_axilite port=in2 bundle=busa
#pragma HLS INTERFACE s_axilite port=in3 bundle=busa
#pragma HLS INTERFACE s_axilite port=in4 bundle=busa


#pragma HLS INTERFACE ap_none port=ctrl
#pragma HLS INTERFACE ap_none port=threshold
#pragma HLS INTERFACE ap_none port=pow_ref
#pragma HLS INTERFACE ap_none port=store_dly
#pragma HLS INTERFACE ap_none port=time_sel

	*ctrl = in0;
	*threshold = in1;
	*pow_ref = in2;
	*store_dly = in3;
	*time_sel = in4;
}
