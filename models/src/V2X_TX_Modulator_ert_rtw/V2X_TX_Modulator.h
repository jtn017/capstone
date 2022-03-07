/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Modulator.h
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

#ifndef RTW_HEADER_V2X_TX_Modulator_h_
#define RTW_HEADER_V2X_TX_Modulator_h_
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
  creal_T FIRInterpolation_TapDelayBuff[10];/* '<S3>/FIR Interpolation' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: a.h
   * Referenced by: '<S3>/FIR Interpolation'
   */
  real_T FIRInterpolation_FILTER_COEFF[88];
} ConstP;

/* Real-time Model Data Structure */
struct tag_RTM {
  DW *dwork;
};

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void V2X_TX_Modulator_initialize(RT_MODEL *const rtM, creal_T
  rtU_tx_frame[8464], creal_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712]);
extern void V2X_TX_Modulator_step(RT_MODEL *const rtM, creal_T rtU_tx_frame[8464],
  creal_T rtY_mod_frame[67712], creal_T rtY_ps_out[67712]);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S2>/Complex to Real-Imag' : Unused code path elimination
 * Block '<S2>/Complex to Real-Imag1' : Unused code path elimination
 * Block '<S2>/Scope' : Unused code path elimination
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
 * '<S2>'   : 'v2x_modem_tb/V2X_TX_Modulator/Pulse Shaping'
 * '<S3>'   : 'v2x_modem_tb/V2X_TX_Modulator/Pulse Shaping/Raised Cosine Transmit Filter'
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
