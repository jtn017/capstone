/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Modulator.c
 *
 * Code generated for Simulink model 'V2X_TX_Modulator'.
 *
 * Model version                  : 1.138
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun Mar 27 16:30:36 2022
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
void V2X_TX_Modulator_step(RT_MODEL *const rtM, creal_T rtU_v2x_tx_bb_out[8464],
  cint16_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;
  real_T accumulator_im;
  real_T accumulator_re;
  int32_T i;
  int32_T j;
  int32_T m;

  /* S-Function (sdspupfir2): '<S3>/FIR Interpolation' incorporates:
   *  DataTypeConversion: '<S1>/convert'
   *  Inport: '<Root>/ tx_frame'
   */
  /* Loop over each input channel */
  for (m = 0; m < 8; m++) {
    int32_T coefArrayIdx;
    int32_T oIdx;
    oIdx = m;
    coefArrayIdx = m * 11;

    /* Consume delay line and beginning of input samples */
    for (i = 0; i < 10; i++) {
      real_T accumulator_re_tmp_0;
      accumulator_re = 0.0;
      accumulator_im = 0.0;
      for (j = 0; j < i + 1; j++) {
        int32_T accumulator_re_tmp;
        accumulator_re_tmp = i - j;
        accumulator_re_tmp_0 =
          rtConstP_tx_mod.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
        accumulator_re += rtU_v2x_tx_bb_out[accumulator_re_tmp].re *
          accumulator_re_tmp_0;
        accumulator_im += rtU_v2x_tx_bb_out[accumulator_re_tmp].im *
          accumulator_re_tmp_0;
      }

      for (j = 0; j < 10 - i; j++) {
        accumulator_re_tmp_0 = rtConstP_tx_mod.FIRInterpolation_FILTER_COEFF
          [((coefArrayIdx + i) + j) + 1];
        accumulator_re += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[j].re;
        accumulator_im += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[j].im;
      }

      rtY_ps_out[oIdx].re = accumulator_re;
      rtY_ps_out[oIdx].im = accumulator_im;
      oIdx += 8;
    }

    /* Consume remaining input samples */
    for (i = 0; i < 8454; i++) {
      accumulator_re = 0.0;
      accumulator_im = 0.0;
      for (j = 0; j < 11; j++) {
        accumulator_re += rtU_v2x_tx_bb_out[(i - j) + 10].re *
          rtConstP_tx_mod.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
        accumulator_im += rtU_v2x_tx_bb_out[(i - j) + 10].im *
          rtConstP_tx_mod.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
      }

      rtY_ps_out[oIdx].re = accumulator_re;
      rtY_ps_out[oIdx].im = accumulator_im;
      oIdx += 8;
    }
  }

  /* Update delay line for next frame */
  for (i = 0; i < 10; i++) {
    rtDW->FIRInterpolation_TapDelayBuff[9 - i] = rtU_v2x_tx_bb_out[i + 8454];
  }

  /* End of S-Function (sdspupfir2): '<S3>/FIR Interpolation' */

  /* Outport: '<Root>/mod_frame' */
  for (i = 0; i < 67712; i++) {
    /* DataTypeConversion: '<S1>/convert' incorporates:
     *  S-Function (sdspupfir2): '<S3>/FIR Interpolation'
     */
    accumulator_re = floor(rtY_ps_out[i].re * 1024.0);
    accumulator_im = floor(rtY_ps_out[i].im * 1024.0);
    if (accumulator_re < 2048.0) {
      if (accumulator_re >= -2048.0) {
        rtY_mod_frame[i].re = (int16_T)accumulator_re;
      } else {
        rtY_mod_frame[i].re = -2048;
      }
    } else {
      rtY_mod_frame[i].re = 2047;
    }

    if (accumulator_im < 2048.0) {
      if (accumulator_im >= -2048.0) {
        rtY_mod_frame[i].im = (int16_T)accumulator_im;
      } else {
        rtY_mod_frame[i].im = -2048;
      }
    } else {
      rtY_mod_frame[i].im = 2047;
    }
  }

  /* End of Outport: '<Root>/mod_frame' */
}

/* Model initialize function */
void V2X_TX_Modulator_initialize(RT_MODEL *const rtM, creal_T rtU_v2x_tx_bb_out
  [8464], cint16_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(&rtU_v2x_tx_bb_out[0], 0, 8464U * sizeof(creal_T));

  /* external outputs */
  (void)memset(&rtY_mod_frame[0], 0, 67712U * sizeof(cint16_T));
  (void)memset(&rtY_ps_out[0], 0, 67712U * sizeof(creal_T));

  /* InitializeConditions for S-Function (sdspupfir2): '<S3>/FIR Interpolation' */
  memset(&rtDW->FIRInterpolation_TapDelayBuff[0], 0, 10U * sizeof(creal_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
