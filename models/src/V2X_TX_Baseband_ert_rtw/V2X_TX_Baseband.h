/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Baseband.h
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

#ifndef RTW_HEADER_V2X_TX_Baseband_h_
#define RTW_HEADER_V2X_TX_Baseband_h_
#include <string.h>
#ifndef V2X_TX_Baseband_COMMON_INCLUDES_
#define V2X_TX_Baseband_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* V2X_TX_Baseband_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  uint32_T RSEncoder[5600];            /* '<S6>/RS Encoder' */
  int8_T IntegertoBitConverter[16800]; /* '<S6>/Integer to Bit Converter' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Expression: ini.preamble_params.seq
   * Referenced by: '<S4>/preamble'
   */
  real_T preamble_Value[64];

  /* Computed Parameter: QPSKModulatorBaseband_modmap
   * Referenced by: '<S3>/QPSK Modulator Baseband'
   */
  real_T QPSKModulatorBaseband_modmap[8];

  /* Computed Parameter: RSEncoder_table1
   * Referenced by: '<S6>/RS Encoder'
   */
  int32_T RSEncoder_table1[7];

  /* Computed Parameter: RSEncoder_table2
   * Referenced by: '<S6>/RS Encoder'
   */
  int32_T RSEncoder_table2[7];

  /* Computed Parameter: Scrambler_Polynomial
   * Referenced by: '<S5>/Scrambler'
   */
  uint8_T Scrambler_Polynomial[17];
} ConstP;

/* Real-time Model Data Structure */
struct tag_RTM {
  DW *dwork;
};

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void V2X_TX_Baseband_initialize(RT_MODEL *const rtM, boolean_T
  rtU_data_frame[7200], creal_T rtY_tx_frame[8464], boolean_T rtY_tx_in[7200],
  boolean_T rtY_scrambler_out[7200], boolean_T rtY_encoder_out[16800], creal_T
  rtY_mapper_out[8400], creal_T rtY_preamble_out[8464]);
extern void V2X_TX_Baseband_step(RT_MODEL *const rtM, boolean_T rtU_data_frame
  [7200], creal_T rtY_tx_frame[8464], boolean_T rtY_tx_in[7200], boolean_T
  rtY_scrambler_out[7200], boolean_T rtY_encoder_out[16800], creal_T
  rtY_mapper_out[8400], creal_T rtY_preamble_out[8464]);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S7>/Data Type Duplicate' : Unused code path elimination
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
 * hilite_system('v2x_modem_tb/V2X_TX_Baseband')    - opens subsystem v2x_modem_tb/V2X_TX_Baseband
 * hilite_system('v2x_modem_tb/V2X_TX_Baseband/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'v2x_modem_tb'
 * '<S1>'   : 'v2x_modem_tb/V2X_TX_Baseband'
 * '<S2>'   : 'v2x_modem_tb/V2X_TX_Baseband/Encoder'
 * '<S3>'   : 'v2x_modem_tb/V2X_TX_Baseband/Mapper'
 * '<S4>'   : 'v2x_modem_tb/V2X_TX_Baseband/Preamble Prepender'
 * '<S5>'   : 'v2x_modem_tb/V2X_TX_Baseband/Scrambler'
 * '<S6>'   : 'v2x_modem_tb/V2X_TX_Baseband/Encoder/Binary-Input RS Encoder'
 * '<S7>'   : 'v2x_modem_tb/V2X_TX_Baseband/Encoder/Binary-Input RS Encoder/Data Type Conversion Inherited'
 */

/*-
 * Requirements for '<Root>': V2X_TX_Baseband
 */
#endif                                 /* RTW_HEADER_V2X_TX_Baseband_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
