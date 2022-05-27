/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_RX_Baseband.h
 *
 * Code generated for Simulink model 'V2X_RX_Baseband'.
 *
 * Model version                  : 1.160
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Fri May 27 13:06:37 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#ifndef RTW_HEADER_V2X_RX_Baseband_h_
#define RTW_HEADER_V2X_RX_Baseband_h_
#include <string.h>
#ifndef V2X_RX_Baseband_COMMON_INCLUDES_
#define V2X_RX_Baseband_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 /* V2X_RX_Baseband_COMMON_INCLUDES_ */

/* Model Code Variants */

/* Macros for accessing real-time model data structure */

/* Forward declaration for rtModel */
typedef struct tag_RTM RT_MODEL;

/* Block signals and states (default storage) for system '<Root>' */
typedef struct {
  int32_T RSDecoder_GammaZ[5];         /* '<S6>/RS Decoder' */
  int32_T RSDecoder_GammaZTemp[2];     /* '<S6>/RS Decoder' */
  int32_T RSDecoder_Dz[6];             /* '<S6>/RS Decoder' */
  int32_T RSDecoder_CCode[7];          /* '<S6>/RS Decoder' */
  int32_T RSDecoder_PsiZDeriv[4];      /* '<S6>/RS Decoder' */
  int32_T RSDecoder_d[5];              /* '<S6>/RS Decoder' */
  int32_T RSDecoder_tmpQuotient[5];    /* '<S6>/RS Decoder' */
  int32_T RSDecoder_input[616];        /* '<S6>/RS Decoder' */
  int32_T RSDecoder_outMsg[264];       /* '<S6>/RS Decoder' */
  int32_T RSDecoder_outCNumErr[88];    /* '<S6>/RS Decoder' */
  boolean_T RSDecoder_erasures[616];   /* '<S6>/RS Decoder' */
} DW;

/* Constant parameters (default storage) */
typedef struct {
  /* Computed Parameter: RSDecoder_table1
   * Referenced by: '<S6>/RS Decoder'
   */
  int32_T RSDecoder_table1[7];

  /* Computed Parameter: RSDecoder_table2
   * Referenced by: '<S6>/RS Decoder'
   */
  int32_T RSDecoder_table2[7];

  /* Computed Parameter: Descrambler_Polynomial
   * Referenced by: '<S3>/Descrambler'
   */
  uint8_T Descrambler_Polynomial[17];
} ConstP;

/* Real-time Model Data Structure */
struct tag_RTM {
  DW *dwork;
};

/* Constant parameters (default storage) */
extern const ConstP rtConstP;

/* Model entry point functions */
extern void V2X_RX_Baseband_initialize(RT_MODEL *const rtM, boolean_T
  rtU_v2x_rx_bb_in[1976], uint8_T rtY_data_frame[98], boolean_T rtY_dec_in[1848],
  boolean_T rtY_descr_in[784], boolean_T rtY_bits_out[784]);
extern void V2X_RX_Baseband_step(RT_MODEL *const rtM, boolean_T
  rtU_v2x_rx_bb_in[1976], uint8_T rtY_data_frame[98], boolean_T rtY_dec_in[1848],
  boolean_T rtY_descr_in[784], boolean_T rtY_bits_out[784]);

/*-
 * These blocks were eliminated from the model due to optimizations:
 *
 * Block '<S7>/Data Type Duplicate' : Unused code path elimination
 * Block '<S3>/Data Type Conversion' : Eliminate redundant data type conversion
 * Block '<S5>/Reshape' : Reshape block reduction
 * Block '<S5>/Reshape1' : Reshape block reduction
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
 * hilite_system('v2x_modem_tb/V2X_RX_Baseband')    - opens subsystem v2x_modem_tb/V2X_RX_Baseband
 * hilite_system('v2x_modem_tb/V2X_RX_Baseband/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'v2x_modem_tb'
 * '<S1>'   : 'v2x_modem_tb/V2X_RX_Baseband'
 * '<S2>'   : 'v2x_modem_tb/V2X_RX_Baseband/Decoder'
 * '<S3>'   : 'v2x_modem_tb/V2X_RX_Baseband/Descrambler'
 * '<S4>'   : 'v2x_modem_tb/V2X_RX_Baseband/Preamble Extractor'
 * '<S5>'   : 'v2x_modem_tb/V2X_RX_Baseband/bits_to_bytes'
 * '<S6>'   : 'v2x_modem_tb/V2X_RX_Baseband/Decoder/Binary-Output RS Decoder'
 * '<S7>'   : 'v2x_modem_tb/V2X_RX_Baseband/Decoder/Binary-Output RS Decoder/Data Type Conversion Inherited'
 * '<S8>'   : 'v2x_modem_tb/V2X_RX_Baseband/bits_to_bytes/bit_concat_per_col'
 * '<S9>'   : 'v2x_modem_tb/V2X_RX_Baseband/bits_to_bytes/bit_concat_per_col/Bit Concat'
 * '<S10>'  : 'v2x_modem_tb/V2X_RX_Baseband/bits_to_bytes/bit_concat_per_col/Bit Concat/bc4'
 * '<S11>'  : 'v2x_modem_tb/V2X_RX_Baseband/bits_to_bytes/bit_concat_per_col/Bit Concat/bc4/bit_concat_unary'
 */

/*-
 * Requirements for '<Root>': V2X_RX_Baseband
 */
#endif                                 /* RTW_HEADER_V2X_RX_Baseband_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
