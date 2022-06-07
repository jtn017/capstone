#include "pll.h"


#ifdef BETTER
	void pll(bool init_v, DATA_T init_i, DATA_T init_q, DATA_T IN_R, DATA_T IN_I, DATA_T *OUT_R, DATA_T *OUT_I)
	{
	#pragma HLS INTERFACE ap_vld port=IN_I
	#pragma HLS INTERFACE ap_vld port=IN_R
	#pragma HLS PIPELINE II=400

		// Statically allocate registers
		// Phase integral register
		static REG_T phs_int = 0;
	#pragma HLS RESET variable=phs_int
		// Phase accumulator register
		static REG_T phs_acc = 0;
	#pragma HLS RESET variable=phs_acc

		PHASE_T phase;
		COS_T phase_R;
		COS_T phase_I;
		PHASE_T prod_R;
		PHASE_T prod_I;
		PHASE_T phs_det_R;
		PHASE_T phs_det_I;
		ERR_T phs_err_R;
		ERR_T phs_err_I;

		if(init_v){
			phs_err_R = init_i;
			phs_err_I = init_q;
		}else{

			// Phase shift
			phase = TWO_PI*phs_acc;
			phase_R = hls::cos(phase);
			phase_I = hls::sin(phase);
			prod_R = (IN_R * phase_R) - (IN_I * phase_I);
			prod_I = (IN_R * phase_I) + (IN_I * phase_R);

			// Assign output
			*OUT_R = prod_R;
			*OUT_I = prod_I;

			// Phase error
			phs_det_R = (prod_R > 0) ? 1 : -1;
			phs_det_I = (prod_I > 0) ? 1 : -1;
			phs_err_R = (phs_det_R *  prod_R) - (phs_det_I * -prod_I);
			phs_err_I = (phs_det_R * -prod_I) + (phs_det_I *  prod_R);

		}

		PHASE_T phs_err_atan = hls::atan2(phs_err_I, phs_err_R);

		PHASE_T phs_err = phs_err_atan * DIV_2PI;

		// Loop filter
		phs_int = phs_int + KI * phs_err;
		DATA_T phs_prop  = KP * phs_err;
		DATA_T loop_filt = phs_prop + phs_int;

		// Phase accumulator
		phs_acc = phs_acc + loop_filt;

	}
#else
	void pll(bool init_v, DATA_T init_i, DATA_T init_q, DATA_T IN_R, DATA_T IN_I, DATA_T *OUT_R, DATA_T *OUT_I)
	{
	#pragma HLS INTERFACE ap_vld port=IN_I
	#pragma HLS INTERFACE ap_vld port=IN_R
	#pragma HLS PIPELINE II=400

		// Statically allocate registers
		// Phase integral register
		static REG_T phs_int = 0;
	#pragma HLS RESET variable=phs_int
		// Phase accumulator register
		static REG_T phs_acc = 0;
	#pragma HLS RESET variable=phs_acc
		static REG_T cntr = 0;
	#pragma HLS RESET variable=cntr

		PHASE_T phase;
		COS_T phase_R;
		COS_T phase_I;
		PHASE_T prod_R;
		PHASE_T prod_I;
		PHASE_T sgn_R;
		PHASE_T sgn_I;

		PHASE_T phs_err;

		if(cntr == 64)
			phs_acc = 0;

		// Phase shift
		phase = -phs_acc;
		phase_R = hls::cos(phase);
		phase_I = hls::sin(phase);
		prod_R = (IN_R * phase_R) - (IN_I * phase_I);
		prod_I = (IN_R * phase_I) + (IN_I * phase_R);

		// Assign output
		*OUT_R = prod_R;
		*OUT_I = prod_I;

		// Phase error
		sgn_R = (prod_R > 0) ? 1 : -1;
		sgn_I = (prod_I > 0) ? 1 : -1;

		//if(cntr >= 63)
			phs_err = sgn_R*prod_I-sgn_I*prod_R;
		//else // if bpsk preamble
		//	phs_err = prod_R*prod_I;

		// Loop filter
		phs_int = phs_int + KI * phs_err;
		DATA_T phs_prop  = KP * phs_err;
		DATA_T loop_filt = phs_prop + phs_int;

		// Phase accumulator
		phs_acc = phs_acc + loop_filt;
		cntr = cntr + 1;

	}
#endif
