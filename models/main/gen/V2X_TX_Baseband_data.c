/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Baseband_data.c
 *
 * Code generated for Simulink model 'V2X_TX_Baseband'.
 *
 * Model version                  : 1.134
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun Mar  6 18:32:02 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_TX_Baseband.h"

/* Constant parameters (default storage) */
const ConstP rtConstP = {
  /* Expression: ini.preamble_params.seq
   * Referenced by: '<S4>/preamble'
   */
  { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 0.0,
    1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 1.0,
    0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0,
    1.0, 0.0, 0.0, 0.0 },

  /* Computed Parameter: QPSKModulatorBaseband_modmap
   * Referenced by: '<S3>/QPSK Modulator Baseband'
   */
  { 0.70710678118654757, 0.70710678118654757, -0.70710678118654746,
    0.70710678118654757, -0.70710678118654768, -0.70710678118654746,
    0.70710678118654735, -0.70710678118654768 },

  /* Computed Parameter: RSEncoder_table1
   * Referenced by: '<S6>/RS Encoder'
   */
  { 2, 4, 3, 6, 7, 5, 1 },

  /* Computed Parameter: RSEncoder_table2
   * Referenced by: '<S6>/RS Encoder'
   */
  { 0, 1, 3, 2, 6, 4, 5 },

  /* Computed Parameter: Scrambler_Polynomial
   * Referenced by: '<S5>/Scrambler'
   */
  { 1U, 0U, 0U, 0U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 1U, 0U, 1U, 1U }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
