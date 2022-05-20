/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Modulator.h
 *
 * Code generated for Simulink model 'V2X_TX_Modulator'.
 *
 * Model version                  : 1.159
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Mon Apr 25 21:20:28 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#ifndef RTW_HEADER_V2X_TX_Modulator_h_
#define RTW_HEADER_V2X_TX_Modulator_h_
#include <math.h>
#include <string.h>
#ifndef V2X_TX_Modulator_COMMON_INCLUDES_
#define V2X_TX_Modulator_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* V2X_TX_Modulator_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  creal_T FIRInterpolation_TapDelayBuff[10];/* '<S4>/FIR Interpolation' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: QPSKModulatorBaseband_modmap
   * Referenced by: '<S2>/QPSK Modulator Baseband'
   */
  real_T QPSKModulatorBaseband_modmap[8];

  /* Expression: a.h
   * Referenced by: '<S4>/FIR Interpolation'
   */
  real_T FIRInterpolation_FILTER_COEFF[88];
} ConstP;

/* Real-time Model Data Structure */
struct tag_RTM {
  DW *dwork;
};

/* Constant parameters (default storage) */
extern const ConstP rtConstP_tx_mod;

/* Model entry point functions */
extern void V2X_TX_Modulator_initialize(RT_MODEL *const rtM, boolean_T
  rtU_v2x_tx_bb_out[1976], cint16_T rtY_mod_frame[7904], creal_T rtY_map_out[988],
  creal_T rtY_ps_out[7904]);
extern void V2X_TX_Modulator_step(RT_MODEL *const rtM, boolean_T
  rtU_v2x_tx_bb_out[1976], cint16_T rtY_mod_frame[7904], creal_T rtY_map_out[988],
  creal_T rtY_ps_out[7904]);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S3>/Complex to Real-Imag' : Unused code path elimination
 * Block '<S3>/Complex to Real-Imag1' : Unused code path elimination
 * Block '<S3>/Scope' : Unused code path elimination
 */

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('v2x_modem_tb/V2X_TX_Modulator')    - opens subsystem v2x_modem_tb/V2X_TX_Modulator
 * hilite_system('v2x_modem_tb/V2X_TX_Modulator/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'v2x_modem_tb'
 * '<S1>'   : 'v2x_modem_tb/V2X_TX_Modulator'
 * '<S2>'   : 'v2x_modem_tb/V2X_TX_Modulator/Mapper'
 * '<S3>'   : 'v2x_modem_tb/V2X_TX_Modulator/Pulse Shaping'
 * '<S4>'   : 'v2x_modem_tb/V2X_TX_Modulator/Pulse Shaping/Raised Cosine Transmit Filter'
 */

/*-
 * Requirements for '<Root>': V2X_TX_Modulator
 */
#endif                                 /* RTW_HEADER_V2X_TX_Modulator_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
