/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_TX_Baseband.c
 *
 * Code generated for Simulink model 'V2X_TX_Baseband'.
 *
 * Model version                  : 1.138
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun Mar 27 16:30:25 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_TX_Baseband.h"

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
void V2X_TX_Baseband_step(RT_MODEL *const rtM, boolean_T rtU_v2x_tx_bb_in[7200],
  creal_T rtY_tx_frame[8464], boolean_T rtY_tx_in[7200], boolean_T
  rtY_scrambler_out[7200], boolean_T rtY_encoder_out[16800], creal_T
  rtY_mapper_out[8400], creal_T rtY_preamble_out[8464])
{
  DW *rtDW = rtM->dwork;
  int32_T shiftReg[16];
  int32_T B_n_idx_0;
  int32_T B_n_idx_1;
  int32_T B_n_idx_2;
  int32_T colIdx;
  int32_T i;
  int32_T tmp;
  int8_T rtb_BittoIntegerConverter[2400];
  int8_T MessagePAD[3];

  /* S-Function (scomscram2): '<S5>/Scrambler' incorporates:
   *  Inport: '<Root>/data_frame'
   */
  memset(&shiftReg[0], 0, sizeof(int32_T) << 4U);
  for (B_n_idx_0 = 0; B_n_idx_0 < 7200; B_n_idx_0++) {
    tmp = rtU_v2x_tx_bb_in[B_n_idx_0];
    for (i = 0; i < 16; i++) {
      tmp += (uint8_T)(rtConstP.Scrambler_Polynomial[i + 1] * shiftReg[i]);
    }

    tmp %= 2;
    rtY_scrambler_out[B_n_idx_0] = (tmp != 0);
    for (i = 14; i >= 0; i--) {
      shiftReg[i + 1] = shiftReg[i];
    }

    shiftReg[0U] = tmp;
  }

  /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' incorporates:
   *  S-Function (scomscram2): '<S5>/Scrambler'
   */
  /* Bit to Integer Conversion */
  tmp = 0;
  for (i = 0; i < 2400; i++) {
    /* Input bit order is MSB first */
    B_n_idx_0 = (int32_T)(((uint32_T)rtY_scrambler_out[tmp] << 1U |
      rtY_scrambler_out[tmp + 1]) << 1U | rtY_scrambler_out[tmp + 2]);
    tmp += 3;
    rtb_BittoIntegerConverter[i] = (int8_T)B_n_idx_0;
  }

  /* End of S-Function (scominttobit): '<S6>/Bit to Integer Converter' */

  /* S-Function (scombchrsencoder): '<S6>/RS Encoder' incorporates:
   *  S-Function (scominttobit): '<S6>/Bit to Integer Converter'
   */
  for (i = 0; i < 800; i++) {
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

    rtDW->RSEncoder[i * 7] = (uint32_T)MessagePAD_tmp;
    rtDW->RSEncoder[i * 7 + 1] = (uint32_T)MessagePAD_tmp_0;
    rtDW->RSEncoder[i * 7 + 2] = (uint32_T)MessagePAD_tmp_1;
    rtDW->RSEncoder[i * 7 + 3] = (uint32_T)B_n_idx_0;
    rtDW->RSEncoder[i * 7 + 4] = (uint32_T)B_n_idx_1;
    rtDW->RSEncoder[i * 7 + 5] = (uint32_T)B_n_idx_2;
    rtDW->RSEncoder[i * 7 + 6] = (uint32_T)B_n_idx_3;
  }

  /* End of S-Function (scombchrsencoder): '<S6>/RS Encoder' */

  /* S-Function (scominttobit): '<S6>/Integer to Bit Converter' incorporates:
   *  S-Function (scombchrsencoder): '<S6>/RS Encoder'
   */
  /* Integer to Bit Conversion */
  for (i = 0; i < 5600; i++) {
    uint32_T u;
    tmp = (i + 1) * 3;
    u = rtDW->RSEncoder[i];
    rtDW->IntegertoBitConverter[tmp - 1] = (int8_T)(u & 1U);
    u >>= 1U;
    rtDW->IntegertoBitConverter[tmp - 2] = (int8_T)(u & 1U);
    rtDW->IntegertoBitConverter[tmp - 3] = (int8_T)(u >> 1U & 1U);
  }

  /* End of S-Function (scominttobit): '<S6>/Integer to Bit Converter' */

  /* DataTypeConversion: '<S7>/Conversion' incorporates:
   *  S-Function (scominttobit): '<S6>/Integer to Bit Converter'
   */
  for (i = 0; i < 16800; i++) {
    rtY_encoder_out[i] = ((uint8_T)rtDW->IntegertoBitConverter[i] != 0);
  }

  /* S-Function (scomapskmod4): '<S3>/QPSK Modulator Baseband' incorporates:
   *  Concatenate: '<S4>/Matrix Concatenate'
   *  DataTypeConversion: '<S7>/Conversion'
   */
  tmp = 0;
  B_n_idx_0 = 0;
  for (i = 0; i < 8400; i++) {
    B_n_idx_1 = tmp + 1;
    B_n_idx_2 = rtY_encoder_out[tmp];
    tmp += 2;
    B_n_idx_2 = (B_n_idx_2 << 1) + rtY_encoder_out[B_n_idx_1];
    B_n_idx_1 = (B_n_idx_2 >> 1 ^ B_n_idx_2) << 1;
    rtY_mapper_out[B_n_idx_0].re =
      rtConstP.QPSKModulatorBaseband_modmap[B_n_idx_1];
    rtY_mapper_out[B_n_idx_0].im =
      rtConstP.QPSKModulatorBaseband_modmap[B_n_idx_1 + 1];
    B_n_idx_0++;
  }

  /* End of S-Function (scomapskmod4): '<S3>/QPSK Modulator Baseband' */

  /* Concatenate: '<S4>/Matrix Concatenate' incorporates:
   *  Constant: '<S4>/preamble'
   *  S-Function (scomapskmod4): '<S3>/QPSK Modulator Baseband'
   */
  for (i = 0; i < 64; i++) {
    rtY_tx_frame[i].re = rtConstP.preamble_Value[i];
    rtY_tx_frame[i].im = 0.0;
  }

  memcpy(&rtY_tx_frame[64], &rtY_mapper_out[0], 8400U * sizeof(creal_T));

  /* Outport: '<Root>/preamble_out' incorporates:
   *  Concatenate: '<S4>/Matrix Concatenate'
   */
  memcpy(&rtY_preamble_out[0], &rtY_tx_frame[0], 8464U * sizeof(creal_T));

  /* Outport: '<Root>/tx_in' incorporates:
   *  Inport: '<Root>/data_frame'
   */
  memcpy(&rtY_tx_in[0], &rtU_v2x_tx_bb_in[0], 7200U * sizeof(boolean_T));
}

/* Model initialize function */
void V2X_TX_Baseband_initialize(RT_MODEL *const rtM, boolean_T rtU_v2x_tx_bb_in
  [7200], creal_T rtY_tx_frame[8464], boolean_T rtY_tx_in[7200], boolean_T
  rtY_scrambler_out[7200], boolean_T rtY_encoder_out[16800], creal_T
  rtY_mapper_out[8400], creal_T rtY_preamble_out[8464])
{
  /* Registration code */

  /* external inputs */
  (void)memset(&rtU_v2x_tx_bb_in[0], 0, 7200U * sizeof(boolean_T));

  /* external outputs */
  (void)memset(&rtY_tx_in[0], 0, 7200U * sizeof(boolean_T));
  (void)memset(&rtY_scrambler_out[0], 0, 7200U * sizeof(boolean_T));
  (void)memset(&rtY_encoder_out[0], 0, 16800U * sizeof(boolean_T));
  (void)memset(&rtY_tx_frame[0], 0, 8464U * sizeof(creal_T));
  (void)memset(&rtY_mapper_out[0], 0, 8400U * sizeof(creal_T));
  (void)memset(&rtY_preamble_out[0], 0, 8464U * sizeof(creal_T));
  UNUSED_PARAMETER(rtM);
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
