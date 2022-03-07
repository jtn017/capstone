/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Modulator.c
 *
 * Code generated for Simulink model 'V2X_TX_Modulator'.
 *
 * Model version                  : 1.134
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun Mar  6 18:32:06 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_TX_Modulator.h"

/* Model step function */
void V2X_TX_Modulator_step(RT_MODEL *const rtM, creal_T rtU_tx_frame[8464],
  creal_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;
  int32_T i;
  int32_T j;
  int32_T m;

  /* S-Function (sdspupfir2): '<S3>/FIR Interpolation' incorporates:
   *  Inport: '<Root>/ tx_frame'
   *  Outport: '<Root>/ps_out'
   */
  /* Loop over each input channel */
  for (m = 0; m < 8; m++) {
    real_T accumulator_im;
    real_T accumulator_re;
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
          rtConstP.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
        accumulator_re += rtU_tx_frame[accumulator_re_tmp].re *
          accumulator_re_tmp_0;
        accumulator_im += rtU_tx_frame[accumulator_re_tmp].im *
          accumulator_re_tmp_0;
      }

      for (j = 0; j < 10 - i; j++) {
        accumulator_re_tmp_0 = rtConstP.FIRInterpolation_FILTER_COEFF
          [((coefArrayIdx + i) + j) + 1];
        accumulator_re += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[j].re;
        accumulator_im += accumulator_re_tmp_0 *
          rtDW->FIRInterpolation_TapDelayBuff[j].im;
      }

      rtY_mod_frame[oIdx].re = accumulator_re;
      rtY_mod_frame[oIdx].im = accumulator_im;
      oIdx += 8;
    }

    /* Consume remaining input samples */
    for (i = 0; i < 8454; i++) {
      accumulator_re = 0.0;
      accumulator_im = 0.0;
      for (j = 0; j < 11; j++) {
        accumulator_re += rtU_tx_frame[(i - j) + 10].re *
          rtConstP.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
        accumulator_im += rtU_tx_frame[(i - j) + 10].im *
          rtConstP.FIRInterpolation_FILTER_COEFF[coefArrayIdx + j];
      }

      rtY_mod_frame[oIdx].re = accumulator_re;
      rtY_mod_frame[oIdx].im = accumulator_im;
      oIdx += 8;
    }
  }

  /* Update delay line for next frame */
  for (i = 0; i < 10; i++) {
    rtDW->FIRInterpolation_TapDelayBuff[9 - i] = rtU_tx_frame[i + 8454];
  }

  /* End of S-Function (sdspupfir2): '<S3>/FIR Interpolation' */

  /* Outport: '<Root>/ps_out' incorporates:
   *  S-Function (sdspupfir2): '<S3>/FIR Interpolation'
   */
  memcpy(&rtY_ps_out[0], &rtY_mod_frame[0], 67712U * sizeof(creal_T));
}

/* Model initialize function */
void V2X_TX_Modulator_initialize(RT_MODEL *const rtM, creal_T rtU_tx_frame[8464],
  creal_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712])
{
  DW *rtDW = rtM->dwork;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(&rtU_tx_frame[0], 0, 8464U * sizeof(creal_T));

  /* external outputs */
  (void)memset(&rtY_mod_frame[0], 0, 67712U * sizeof(creal_T));
  (void)memset(&rtY_ps_out[0], 0, 67712U * sizeof(creal_T));

  /* InitializeConditions for S-Function (sdspupfir2): '<S3>/FIR Interpolation' */
  memset(&rtDW->FIRInterpolation_TapDelayBuff[0], 0, 10U * sizeof(creal_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
