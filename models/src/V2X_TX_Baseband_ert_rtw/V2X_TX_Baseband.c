/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Baseband.c
 *
 * Code generated for Simulink model 'V2X_TX_Baseband'.
 *
 * Model version                  : 1.186
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun May 29 15:41:53 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_TX_Baseband.h"
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

/*===========*
 * Constants *
 *===========*/
#define RT_PI                          3.14159265358979323846
#define RT_PIF                         3.1415927F
#define RT_LN_10                       2.30258509299404568402
#define RT_LN_10F                      2.3025851F
#define RT_LOG10E                      0.43429448190325182765
#define RT_LOG10EF                     0.43429449F
#define RT_E                           2.7182818284590452354
#define RT_EF                          2.7182817F

/*
 * UNUSED_PARAMETER(x)
 *   Used to specify that a function parameter (argument) is required but not
 *   accessed by the function body.
 */
#ifndef UNUSED_PARAMETER
#if defined(__LCC__)
#define UNUSED_PARAMETER(x)                                      /* do nothing */
#else

/*
 * This is the semi-ANSI standard way of indicating that an
 * unused function parameter is required.
 */
#define UNUSED_PARAMETER(x)            (void) (x)
#endif
#endif

/* Model step function */
void V2X_TX_Baseband_step(RT_MODEL *const rtM, uint8_T rtU_v2x_tx_bb_in[98],
  boolean_T rtY_tx_frame[1976], boolean_T rtY_bits_in[784], boolean_T
  rtY_scramb_out[784], boolean_T rtY_enc_out[1848])
{
  int32_T shiftReg[16];
  int32_T B_n_idx_0;
  int32_T colIdx;
  int32_T i;
  int32_T tmp;
  uint32_T rtb_RSEncoder[616];
  int8_T rtb_IntegertoBitConverter[1848];
  int8_T rtb_BittoIntegerConverter[264];
  int8_T MessagePAD[3];
  boolean_T rtb_MatrixConcatenate[792];
  for (i = 0; i < 98; i++) {
    /* DataTypeConversion: '<S5>/convert7' incorporates:
     *  DataTypeConversion: '<S15>/Extract Desired Bits'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i] = ((int32_T)((uint32_T)rtU_v2x_tx_bb_in[i] >> 7) != 0);

    /* DataTypeConversion: '<S5>/convert6' incorporates:
     *  DataTypeConversion: '<S14>/Extract Desired Bits'
     *  DataTypeConversion: '<S5>/convert7'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 98] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 6 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert5' incorporates:
     *  DataTypeConversion: '<S13>/Extract Desired Bits'
     *  DataTypeConversion: '<S5>/convert7'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 196] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 5 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert4' incorporates:
     *  DataTypeConversion: '<S12>/Extract Desired Bits'
     *  DataTypeConversion: '<S5>/convert7'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 294] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 4 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert3' incorporates:
     *  DataTypeConversion: '<S11>/Extract Desired Bits'
     *  DataTypeConversion: '<S5>/convert7'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 392] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 3 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert2' incorporates:
     *  DataTypeConversion: '<S10>/Extract Desired Bits'
     *  DataTypeConversion: '<S5>/convert7'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 490] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 2 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert1' incorporates:
     *  DataTypeConversion: '<S5>/convert7'
     *  DataTypeConversion: '<S9>/Extract Desired Bits'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 588] = (((uint32_T)rtU_v2x_tx_bb_in[i] >> 1 & 1U) != 0U);

    /* DataTypeConversion: '<S5>/convert' incorporates:
     *  DataTypeConversion: '<S5>/convert7'
     *  DataTypeConversion: '<S8>/Extract Desired Bits'
     *  Inport: '<Root>/data_frame'
     */
    rtY_scramb_out[i + 686] = ((rtU_v2x_tx_bb_in[i] & 1U) != 0U);
  }

  /* Math: '<S5>/Transpose' incorporates:
   *  DataTypeConversion: '<S5>/convert7'
   *  S-Function (scomscram2): '<S4>/Scrambler'
   */
  for (tmp = 0; tmp < 8; tmp++) {
    for (i = 0; i < 98; i++) {
      rtY_bits_in[tmp + (i << 3)] = rtY_scramb_out[98 * tmp + i];
    }
  }

  /* S-Function (scomscram2): '<S4>/Scrambler' incorporates:
   *  DataTypeConversion: '<S5>/convert7'
   *  Math: '<S5>/Transpose'
   */
  memset(&shiftReg[0], 0, sizeof(int32_T) << 4U);
  for (B_n_idx_0 = 0; B_n_idx_0 < 784; B_n_idx_0++) {
    tmp = rtY_bits_in[B_n_idx_0];
    for (i = 0; i < 16; i++) {
      tmp += (uint8_T)(rtConstP.Scrambler_Polynomial[i + 1] * shiftReg[i]);
    }

    tmp %= 2;
    rtY_scramb_out[B_n_idx_0] = (tmp != 0);
    for (i = 14; i >= 0; i--) {
      shiftReg[i + 1] = shiftReg[i];
    }

    shiftReg[0U] = tmp;

    /* SignalConversion generated from: '<S2>/Matrix Concatenate' incorporates:
     *  DataTypeConversion: '<S5>/convert7'
     *  Math: '<S5>/Transpose'
     *  S-Function (scomscram2): '<S4>/Scrambler'
     */
    rtb_MatrixConcatenate[B_n_idx_0] = rtY_scramb_out[B_n_idx_0];
  }

  /* End of S-Function (scomscram2): '<S4>/Scrambler' */

  /* Constant: '<S2>/append_zeros' */
  for (i = 0; i < 8; i++) {
    rtb_MatrixConcatenate[i + 784] = false;
  }

  /* End of Constant: '<S2>/append_zeros' */

  /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' incorporates:
   *  Concatenate: '<S2>/Matrix Concatenate'
   */
  /* Bit to Integer Conversion */
  tmp = 0;
  for (i = 0; i < 264; i++) {
    /* Input bit order is MSB first */
    B_n_idx_0 = (int32_T)(((uint32_T)rtb_MatrixConcatenate[tmp] << 1U |
      rtb_MatrixConcatenate[tmp + 1]) << 1U | rtb_MatrixConcatenate[tmp + 2]);
    tmp += 3;
    rtb_BittoIntegerConverter[i] = (int8_T)B_n_idx_0;
  }

  /* End of S-Function (scominttobit): '<S6>/Bit to Integer Converter' */

  /* S-Function (scombchrsencoder): '<S6>/RS Encoder' incorporates:
   *  S-Function (scominttobit): '<S6>/Bit to Integer Converter'
   */
  for (i = 0; i < 88; i++) {
    int32_T B_n_idx_1;
    int32_T B_n_idx_2;
    int32_T B_n_idx_3;
    int8_T MessagePAD_tmp;
    int8_T MessagePAD_tmp_0;
    int8_T MessagePAD_tmp_1;

    /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' */
    MessagePAD_tmp = rtb_BittoIntegerConverter[i * 3];
    MessagePAD[0] = MessagePAD_tmp;

    /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' */
    MessagePAD_tmp_0 = rtb_BittoIntegerConverter[i * 3 + 1];
    MessagePAD[1] = MessagePAD_tmp_0;

    /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' */
    MessagePAD_tmp_1 = rtb_BittoIntegerConverter[i * 3 + 2];
    MessagePAD[2] = MessagePAD_tmp_1;
    B_n_idx_0 = 0;
    B_n_idx_1 = 0;
    B_n_idx_2 = 0;
    B_n_idx_3 = 0;
    for (colIdx = 0; colIdx < 3; colIdx++) {
      int32_T firstParity_idx_1;
      int8_T MessagePAD_p;
      MessagePAD_p = MessagePAD[colIdx];
      firstParity_idx_1 = B_n_idx_0;
      tmp = MessagePAD_p ^ B_n_idx_0;
      if (tmp == 0) {
        tmp = 0;
      } else {
        tmp = (rtConstP.RSEncoder_table2[tmp - 1] + 3) % 7;
        if (tmp == 0) {
          tmp = 7;
        }

        tmp = rtConstP.RSEncoder_table1[tmp - 1];
      }

      B_n_idx_0 = B_n_idx_1 ^ tmp;
      tmp = MessagePAD_p ^ firstParity_idx_1;
      if (tmp == 0) {
        tmp = 0;
      } else {
        tmp = rtConstP.RSEncoder_table2[tmp - 1] % 7;
        if (tmp == 0) {
          tmp = 7;
        }

        tmp = rtConstP.RSEncoder_table1[tmp - 1];
      }

      B_n_idx_1 = B_n_idx_2 ^ tmp;
      tmp = MessagePAD_p ^ firstParity_idx_1;
      if (tmp == 0) {
        tmp = 0;
      } else {
        tmp = (rtConstP.RSEncoder_table2[tmp - 1] + 1) % 7;
        if (tmp == 0) {
          tmp = 7;
        }

        tmp = rtConstP.RSEncoder_table1[tmp - 1];
      }

      B_n_idx_2 = B_n_idx_3 ^ tmp;
      tmp = MessagePAD_p ^ firstParity_idx_1;
      if (tmp == 0) {
        B_n_idx_3 = 0;
      } else {
        tmp = (rtConstP.RSEncoder_table2[tmp - 1] + 3) % 7;
        if (tmp == 0) {
          tmp = 7;
        }

        B_n_idx_3 = rtConstP.RSEncoder_table1[tmp - 1];
      }
    }

    rtb_RSEncoder[i * 7] = (uint32_T)MessagePAD_tmp;
    rtb_RSEncoder[i * 7 + 1] = (uint32_T)MessagePAD_tmp_0;
    rtb_RSEncoder[i * 7 + 2] = (uint32_T)MessagePAD_tmp_1;
    rtb_RSEncoder[i * 7 + 3] = (uint32_T)B_n_idx_0;
    rtb_RSEncoder[i * 7 + 4] = (uint32_T)B_n_idx_1;
    rtb_RSEncoder[i * 7 + 5] = (uint32_T)B_n_idx_2;
    rtb_RSEncoder[i * 7 + 6] = (uint32_T)B_n_idx_3;
  }

  /* End of S-Function (scombchrsencoder): '<S6>/RS Encoder' */

  /* S-Function (scominttobit): '<S6>/Integer to Bit Converter' incorporates:
   *  S-Function (scombchrsencoder): '<S6>/RS Encoder'
   */
  /* Integer to Bit Conversion */
  for (i = 0; i < 616; i++) {
    uint32_T u;
    tmp = (i + 1) * 3;
    u = rtb_RSEncoder[i];
    rtb_IntegertoBitConverter[tmp - 1] = (int8_T)(u & 1U);
    u >>= 1U;
    rtb_IntegertoBitConverter[tmp - 2] = (int8_T)(u & 1U);
    rtb_IntegertoBitConverter[tmp - 3] = (int8_T)(u >> 1U & 1U);
  }

  /* End of S-Function (scominttobit): '<S6>/Integer to Bit Converter' */
  for (i = 0; i < 1848; i++) {
    /* DataTypeConversion: '<S7>/Conversion' incorporates:
     *  S-Function (scominttobit): '<S6>/Integer to Bit Converter'
     */
    rtY_enc_out[i] = ((uint8_T)rtb_IntegertoBitConverter[i] != 0);

    /* SignalConversion generated from: '<S3>/Matrix Concatenate' incorporates:
     *  DataTypeConversion: '<S7>/Conversion'
     *  Outport: '<Root>/tx_frame'
     */
    rtY_tx_frame[i + 128] = rtY_enc_out[i];
  }

  /* Constant: '<S3>/preamble' incorporates:
   *  Outport: '<Root>/tx_frame'
   */
  memcpy(&rtY_tx_frame[0], &rtConstP.preamble_Value[0], sizeof(boolean_T) << 7U);
  UNUSED_PARAMETER(rtM);
}

/* Model initialize function */
void V2X_TX_Baseband_initialize(RT_MODEL *const rtM, uint8_T rtU_v2x_tx_bb_in[98],
  boolean_T rtY_tx_frame[1976], boolean_T rtY_bits_in[784], boolean_T
  rtY_scramb_out[784], boolean_T rtY_enc_out[1848])
{
  /* Registration code */

  /* external inputs */
  (void)memset(&rtU_v2x_tx_bb_in[0], 0, 98U * sizeof(uint8_T));

  /* external outputs */
  (void)memset(&rtY_tx_frame[0], 0, 1976U * sizeof(boolean_T));
  (void)memset(&rtY_bits_in[0], 0, 784U * sizeof(boolean_T));
  (void)memset(&rtY_scramb_out[0], 0, 784U * sizeof(boolean_T));
  (void)memset(&rtY_enc_out[0], 0, 1848U * sizeof(boolean_T));
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
