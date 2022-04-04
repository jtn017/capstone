/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Modulator.c
 *
 * Code generated for Simulink model 'V2X_TX_Modulator'.
 *
 * Model version                  : 1.156
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun Apr  3 20:26:38 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_TX_Modulator.h"
#ifndef UCHAR_MAX
#include <limits.h>
#endif

#if ( UCHAR_MAX != (0xFFU) ) || ( SCHAR_MAX != (0x7F) )
#error Code was generated for compiler with different sized uchar/char. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( USHRT_MAX != (0xFFFFU) ) || ( SHRT_MAX != (0x7FFF) )
#error Code was generated for compiler with different sized ushort/short. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

#if ( UINT_MAX != (0xFFFFFFFFU) ) || ( INT_MAX != (0x7FFFFFFF) )
#error Code was generated for compiler with different sized uint/int. \
Consider adjusting Test hardware word size settings on the \
Hardware Implementation pane to match your compiler word sizes as \
defined in limits.h of the compiler. Alternatively, you can \
select the Test hardware is the same as production hardware option and \
select the Enable portable word sizes option on the Code Generation > \
Verification pane for ERT based targets, which will disable the \
preprocessor word size checks.
#endif

/* Skipping ulong/long check: insufficient preprocessor integer range. */

/* Model step function */
void V2X_TX_Modulator_step(RT_MODEL *const rtM, boolean_T rtU_v2x_tx_bb_out
  [16928], cint16_T rtY_mod_frame[67712], creal_T rtY_map_out[8464], creal_T
  rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;
  real_T accumulator_im;
  real_T accumulator_re;
  int32_T i;
  int32_T inIdx;
  int32_T inIdx_0;
  int32_T outIdx;
  int32_T symbolIndex;

  /* S-Function (scomapskmod4): '<S2>/QPSK Modulator Baseband' incorporates:
   *  Inport: '<Root>/ tx_frame'
   */
  inIdx = 0;
  outIdx = 0;
  for (i = 0; i < 8464; i++) {
    inIdx_0 = inIdx + 1;
    symbolIndex = rtU_v2x_tx_bb_out[inIdx];
    inIdx += 2;
    symbolIndex = (symbolIndex << 1) + rtU_v2x_tx_bb_out[inIdx_0];
    inIdx_0 = (symbolIndex >> 1 ^ symbolIndex) << 1;
    rtY_map_out[outIdx].re = rtConstP.QPSKModulatorBaseband_modmap[inIdx_0];
    rtY_map_out[outIdx].im = rtConstP.QPSKModulatorBaseband_modmap[inIdx_0 + 1];
    outIdx++;
  }

  /* S-Function (sdspupfir2): '<S4>/FIR Interpolation' incorporates:
   *  DataTypeConversion: '<S1>/convert'
   *  S-Function (scomapskmod4): '<S2>/QPSK Modulator Baseband'
   */
  /* Loop over each input channel */
  for (inIdx = 0; inIdx < 8; inIdx++) {
    outIdx = inIdx;
    inIdx_0 = inIdx * 11;

    /* Consume delay line and beginning of input samples */
    for (i = 0; i < 10; i++) {
      real_T accumulator_re_tmp_0;
      accumulator_re = 0.0;
      accumulator_im = 0.0;
      for (symbolIndex = 0; symbolIndex < i + 1; symbolIndex++) {
        int32_T accumulator_re_tmp;
        accumulator_re_tmp = i - symbolIndex;
        accumulator_re_tmp_0 = rtConstP.FIRInterpolation_FILTER_COEFF[inIdx_0 +
          symbolIndex];
        accumulator_re += rtY_map_out[accumulator_re_tmp].re *
          accumulator_re_tmp_0;
        accumulator_im += rtY_map_out[accumulator_re_tmp].im *
          accumulator_re_tmp_0;
      }

      for (symbolIndex = 0; symbolIndex < 10 - i; symbolIndex++) {
        accumulator_re_tmp_0 = rtConstP.FIRInterpolation_FILTER_COEFF[((inIdx_0
          + i) + symbolIndex) + 1];
        accumulator_re += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[symbolIndex].re;
        accumulator_im += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[symbolIndex].im;
      }

      rtY_ps_out[outIdx].re = accumulator_re;
      rtY_ps_out[outIdx].im = accumulator_im;
      outIdx += 8;
    }

    /* Consume remaining input samples */
    for (i = 0; i < 8454; i++) {
      accumulator_re = 0.0;
      accumulator_im = 0.0;
      for (symbolIndex = 0; symbolIndex < 11; symbolIndex++) {
        accumulator_re += rtY_map_out[(i - symbolIndex) + 10].re *
          rtConstP.FIRInterpolation_FILTER_COEFF[inIdx_0 + symbolIndex];
        accumulator_im += rtY_map_out[(i - symbolIndex) + 10].im *
          rtConstP.FIRInterpolation_FILTER_COEFF[inIdx_0 + symbolIndex];
      }

      rtY_ps_out[outIdx].re = accumulator_re;
      rtY_ps_out[outIdx].im = accumulator_im;
      outIdx += 8;
    }
  }

  /* Update delay line for next frame */
  for (i = 0; i < 10; i++) {
    rtDW->FIRInterpolation_TapDelayBuff[9 - i] = rtY_map_out[i + 8454];
  }

  /* End of S-Function (sdspupfir2): '<S4>/FIR Interpolation' */

  /* Outport: '<Root>/mod_frame' */
  for (i = 0; i < 67712; i++) {
    /* DataTypeConversion: '<S1>/convert' incorporates:
     *  S-Function (sdspupfir2): '<S4>/FIR Interpolation'
     */
    accumulator_re = floor(rtY_ps_out[i].re * 16384.0);
    accumulator_im = floor(rtY_ps_out[i].im * 16384.0);
    if (accumulator_re < 32768.0) {
      if (accumulator_re >= -32768.0) {
        rtY_mod_frame[i].re = (int16_T)accumulator_re;
      } else {
        rtY_mod_frame[i].re = MIN_int16_T;
      }
    } else {
      rtY_mod_frame[i].re = MAX_int16_T;
    }

    if (accumulator_im < 32768.0) {
      if (accumulator_im >= -32768.0) {
        rtY_mod_frame[i].im = (int16_T)accumulator_im;
      } else {
        rtY_mod_frame[i].im = MIN_int16_T;
      }
    } else {
      rtY_mod_frame[i].im = MAX_int16_T;
    }
  }

  /* End of Outport: '<Root>/mod_frame' */
}

/* Model initialize function */
void V2X_TX_Modulator_initialize(RT_MODEL *const rtM, boolean_T
  rtU_v2x_tx_bb_out[16928], cint16_T rtY_mod_frame[67712], creal_T rtY_map_out
  [8464], creal_T rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(&rtU_v2x_tx_bb_out[0], 0, 16928U * sizeof(boolean_T));

  /* external outputs */
  (void)memset(&rtY_mod_frame[0], 0, 67712U * sizeof(cint16_T));
  (void)memset(&rtY_map_out[0], 0, 8464U * sizeof(creal_T));
  (void)memset(&rtY_ps_out[0], 0, 67712U * sizeof(creal_T));

  /* InitializeConditions for S-Function (sdspupfir2): '<S4>/FIR Interpolation' */
  memset(&rtDW->FIRInterpolation_TapDelayBuff[0], 0, 10U * sizeof(creal_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
