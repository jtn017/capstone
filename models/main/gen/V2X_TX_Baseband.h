/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Baseband.h
 *
 * Code generated for Simulink model 'V2X_TX_Baseband'.
 *
 * Model version                  : 1.160
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri May 27 13:06:16 2022
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

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: RSEncoder_table1
   * Referenced by: '<S6>/RS Encoder'
   */
  int32_T RSEncoder_table1[7];

  /* Computed Parameter: RSEncoder_table2
   * Referenced by: '<S6>/RS Encoder'
   */
  int32_T RSEncoder_table2[7];

  /* Computed Parameter: preamble_Value
   * Referenced by: '<S3>/preamble'
   */
  boolean_T preamble_Value[128];

  /* Computed Parameter: Scrambler_Polynomial
   * Referenced by: '<S4>/Scrambler'
   */
  uint8_T Scrambler_Polynomial[17];
} ConstP;

/* Real-time Model Data Structure */
struct tag_RTM {
  char_T rt_unused;
};

/* Constant parameters (default storage) */
extern const ConstP rtConstP_tx_base;

/* Model entry point functions */
extern void V2X_TX_Baseband_initialize(RT_MODEL *const rtM, uint8_T
  rtU_v2x_tx_bb_in[98], boolean_T rtY_tx_frame[1976], boolean_T rtY_bits_in[784],
  boolean_T rtY_scramb_out[784], boolean_T rtY_enc_out[1848]);
extern void V2X_TX_Baseband_step(RT_MODEL *const rtM, uint8_T rtU_v2x_tx_bb_in
  [98], boolean_T rtY_tx_frame[1976], boolean_T rtY_bits_in[784], boolean_T
  rtY_scramb_out[784], boolean_T rtY_enc_out[1848]);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S7>/Data Type Duplicate' : Unused code path elimination
 * Block '<S8>/DTProp1' : Unused code path elimination
 * Block '<S8>/DTProp2' : Unused code path elimination
 * Block '<S9>/DTProp1' : Unused code path elimination
 * Block '<S9>/DTProp2' : Unused code path elimination
 * Block '<S10>/DTProp1' : Unused code path elimination
 * Block '<S10>/DTProp2' : Unused code path elimination
 * Block '<S11>/DTProp1' : Unused code path elimination
 * Block '<S11>/DTProp2' : Unused code path elimination
 * Block '<S12>/DTProp1' : Unused code path elimination
 * Block '<S12>/DTProp2' : Unused code path elimination
 * Block '<S13>/DTProp1' : Unused code path elimination
 * Block '<S13>/DTProp2' : Unused code path elimination
 * Block '<S14>/DTProp1' : Unused code path elimination
 * Block '<S14>/DTProp2' : Unused code path elimination
 * Block '<S15>/DTProp1' : Unused code path elimination
 * Block '<S15>/DTProp2' : Unused code path elimination
 * Block '<S8>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S9>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S10>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S11>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S12>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S13>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S14>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S15>/Modify Scaling Only' : Eliminate redundant data type conversion
 * Block '<S5>/Reshape' : Reshape block reduction
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
 * '<S3>'   : 'v2x_modem_tb/V2X_TX_Baseband/Preamble Prepender'
 * '<S4>'   : 'v2x_modem_tb/V2X_TX_Baseband/Scrambler'
 * '<S5>'   : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits'
 * '<S6>'   : 'v2x_modem_tb/V2X_TX_Baseband/Encoder/Binary-Input RS Encoder'
 * '<S7>'   : 'v2x_modem_tb/V2X_TX_Baseband/Encoder/Binary-Input RS Encoder/Data Type Conversion Inherited'
 * '<S8>'   : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits'
 * '<S9>'   : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits1'
 * '<S10>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits2'
 * '<S11>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits3'
 * '<S12>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits4'
 * '<S13>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits5'
 * '<S14>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits6'
 * '<S15>'  : 'v2x_modem_tb/V2X_TX_Baseband/bytes_to_bits/Extract Bits7'
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
