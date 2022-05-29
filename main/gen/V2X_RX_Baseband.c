/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_RX_Baseband.c
 *
 * Code generated for Simulink model 'V2X_RX_Baseband'.
 *
 * Model version                  : 1.186
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sun May 29 15:42:47 2022
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Intel->x86-64 (Linux 64)
 * Code generation objectives:
 *    1. Execution efficiency
 *    2. Traceability
 * Validation result: Not run
 */

#include "V2X_RX_Baseband.h"

/* Model step function */
void V2X_RX_Baseband_step(RT_MODEL *const rtM, boolean_T rtU_v2x_rx_bb_in[1976],
  uint8_T rtY_data_frame[98], boolean_T rtY_dec_in[1848], boolean_T
  rtY_descr_in[784], boolean_T rtY_bits_out[784])
{
  DW *rtDW = rtM->dwork;
  int32_T shiftReg[16];
  int32_T RSDecoder_OmegaZ[10];
  int32_T RSDecoder_PsiZ[5];
  int32_T RSDecoder_PsiZStar[5];
  int32_T RSDecoder_TempVec2t1[5];
  int32_T RSDecoder_Errloc[4];
  int32_T RSDecoder_Syndrome[4];
  int32_T Temp4;
  int32_T i;
  int32_T intVal;
  int32_T inv;
  int32_T j;
  int32_T rtb_uDMaximum;
  int32_T temp;
  uint32_T rtb_RSDecoder[264];
  uint32_T tmp;
  uint32_T tmp_0;
  uint32_T tmp_1;
  uint32_T u;
  int8_T rtb_IntegertoBitConverter[792];
  int8_T rtb_BittoIntegerConverter[616];
  int8_T y[8];
  uint8_T uDMaximum_Valdata;
  boolean_T rtb_off1[1976];
  boolean_T rtb_off2[1976];
  boolean_T rtb_off3[1976];
  boolean_T rtb_Conversion[792];
  boolean_T guard1 = false;
  boolean_T guard2 = false;
  boolean_T loopflag;

  /* MATLAB Function: '<S4>/rotator' incorporates:
   *  Inport: '<Root>/rx_frame'
   */
  memcpy(&rtb_off1[0], &rtU_v2x_rx_bb_in[0], 1976U * sizeof(boolean_T));
  for (i = 0; i < 988; i++) {
    temp = i << 1;
    loopflag = rtb_off1[temp];
    if ((!loopflag) && (!rtb_off1[temp + 1])) {
      rtb_off1[temp] = false;
      rtb_off1[temp + 1] = true;
    } else {
      guard1 = false;
      guard2 = false;
      if (!loopflag) {
        rtb_uDMaximum = (i << 1) + 1;
        if (rtb_off1[rtb_uDMaximum]) {
          rtb_off1[temp] = true;
          rtb_off1[rtb_uDMaximum] = true;
        } else {
          guard2 = true;
        }
      } else {
        guard2 = true;
      }

      if (guard2) {
        if (rtb_off1[temp]) {
          rtb_uDMaximum = (i << 1) + 1;
          if (rtb_off1[rtb_uDMaximum]) {
            rtb_off1[temp] = true;
            rtb_off1[rtb_uDMaximum] = false;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }

      if (guard1) {
        rtb_off1[temp] = false;
        rtb_off1[(i << 1) + 1] = false;
      }
    }
  }

  for (i = 0; i < 1976; i++) {
    rtb_off2[i] = !rtU_v2x_rx_bb_in[i];
    rtb_off3[i] = rtU_v2x_rx_bb_in[i];
  }

  for (i = 0; i < 988; i++) {
    temp = i << 1;
    loopflag = rtb_off3[temp];
    if ((!loopflag) && (!rtb_off3[temp + 1])) {
      rtb_off3[temp] = true;
      rtb_off3[temp + 1] = false;
    } else {
      guard1 = false;
      guard2 = false;
      if (!loopflag) {
        rtb_uDMaximum = (i << 1) + 1;
        if (rtb_off3[rtb_uDMaximum]) {
          rtb_off3[temp] = false;
          rtb_off3[rtb_uDMaximum] = false;
        } else {
          guard2 = true;
        }
      } else {
        guard2 = true;
      }

      if (guard2) {
        if (rtb_off3[temp]) {
          rtb_uDMaximum = (i << 1) + 1;
          if (rtb_off3[rtb_uDMaximum]) {
            rtb_off3[temp] = false;
            rtb_off3[rtb_uDMaximum] = true;
          } else {
            guard1 = true;
          }
        } else {
          guard1 = true;
        }
      }

      if (guard1) {
        rtb_off3[temp] = true;
        rtb_off3[(i << 1) + 1] = true;
      }
    }
  }

  /* Sum: '<S4>/Sum' */
  u = 0U;

  /* Sum: '<S4>/Sum1' */
  tmp = 0U;

  /* Sum: '<S4>/Sum2' */
  tmp_0 = 0U;

  /* Sum: '<S4>/Sum3' */
  tmp_1 = 0U;
  for (i = 0; i < 128; i++) {
    /* Sum: '<S4>/Sum' incorporates:
     *  Constant: '<S4>/Constant'
     *  Inport: '<Root>/rx_frame'
     *  Logic: '<S4>/AND'
     *  MATLAB Function: '<S4>/rotator'
     */
    u += ((rtConstP_rx_base.Constant_Value[i] != 0.0) == (int32_T)rtU_v2x_rx_bb_in[i]);

    /* Sum: '<S4>/Sum1' incorporates:
     *  Constant: '<S4>/Constant'
     *  Logic: '<S4>/AND1'
     */
    tmp += ((rtConstP_rx_base.Constant_Value[i] != 0.0) == (int32_T)rtb_off1[i]);

    /* Sum: '<S4>/Sum2' incorporates:
     *  Constant: '<S4>/Constant'
     *  Logic: '<S4>/AND2'
     */
    tmp_0 += ((rtConstP_rx_base.Constant_Value[i] != 0.0) == (int32_T)rtb_off2[i]);

    /* Sum: '<S4>/Sum3' incorporates:
     *  Constant: '<S4>/Constant'
     *  Logic: '<S4>/AND3'
     */
    tmp_1 += ((rtConstP_rx_base.Constant_Value[i] != 0.0) == (int32_T)rtb_off3[i]);
  }

  /* S-Function (svipstatminmax): '<S4>/2-D Maximum' incorporates:
   *  Sum: '<S4>/Sum'
   *  Sum: '<S4>/Sum1'
   *  Sum: '<S4>/Sum2'
   *  Sum: '<S4>/Sum3'
   */
  uDMaximum_Valdata = (uint8_T)u;
  rtb_uDMaximum = 1;
  if ((uint8_T)tmp > (uint8_T)u) {
    uDMaximum_Valdata = (uint8_T)tmp;
    rtb_uDMaximum = 2;
  }

  if ((uint8_T)tmp_0 > uDMaximum_Valdata) {
    uDMaximum_Valdata = (uint8_T)tmp_0;
    rtb_uDMaximum = 3;
  }

  if ((uint8_T)tmp_1 > uDMaximum_Valdata) {
    rtb_uDMaximum = 4;
  }

  /* End of S-Function (svipstatminmax): '<S4>/2-D Maximum' */

  /* MultiPortSwitch: '<S4>/Multiport Switch' incorporates:
   *  Inport: '<Root>/rx_frame'
   *  MATLAB Function: '<S4>/rotator'
   */
  switch ((uint32_T)rtb_uDMaximum) {
   case 1:
    memcpy(&rtb_off3[0], &rtU_v2x_rx_bb_in[0], 1976U * sizeof(boolean_T));
    break;

   case 2:
    memcpy(&rtb_off3[0], &rtb_off1[0], 1976U * sizeof(boolean_T));
    break;

   case 3:
    memcpy(&rtb_off3[0], &rtb_off2[0], 1976U * sizeof(boolean_T));
    break;
  }

  /* End of MultiPortSwitch: '<S4>/Multiport Switch' */

  /* Outport: '<Root>/dec_in' */
  memcpy(&rtY_dec_in[0], &rtb_off3[128], 1848U * sizeof(boolean_T));

  /* S-Function (scominttobit): '<S6>/Bit to Integer Converter' */
  /* Bit to Integer Conversion */
  rtb_uDMaximum = 128;
  for (i = 0; i < 616; i++) {
    /* Input bit order is MSB first */
    intVal = (int32_T)(((uint32_T)rtb_off3[rtb_uDMaximum] << 1U |
                        rtb_off3[rtb_uDMaximum + 1]) << 1U |
                       rtb_off3[rtb_uDMaximum + 2]);
    rtb_uDMaximum += 3;
    rtb_BittoIntegerConverter[i] = (int8_T)intVal;
  }

  /* End of S-Function (scominttobit): '<S6>/Bit to Integer Converter' */

  /* S-Function (scomberlekamp): '<S6>/RS Decoder' incorporates:
   *  S-Function (scominttobit): '<S6>/Bit to Integer Converter'
   */
  /* start of COMM_DoBerlekamp()  */
  /* The function uses the algorithm  described */
  /* in Clark and Cain, Error-Correction Coding for Digital Communications, 1981 */
  /* PART I - ERROR LOCATOR POLYNOMIAL COMPUTATION */
  /* Compute syndrome series : length = 2*t */
  rtDW->RSDecoder_GammaZ[0U] = 1;
  rtDW->RSDecoder_GammaZ[1] = 0;
  rtDW->RSDecoder_GammaZ[2] = 0;
  rtDW->RSDecoder_GammaZ[3] = 0;
  rtDW->RSDecoder_GammaZ[4] = 0;
  rtDW->RSDecoder_GammaZTemp[0U] = 1;
  for (rtb_uDMaximum = 0; rtb_uDMaximum < 88; rtb_uDMaximum++) {
    int32_T Temp3;
    int32_T noErrorStatus;
    noErrorStatus = 1;

    /*  start of assignInputs(): Populate the codeword and erasure vectors with the proper data  */
    /* Assign message symbols. */
    /* If there are erasures, insert zeros in the erased positions. */
    i = rtb_uDMaximum * 7;
    rtDW->RSDecoder_CCode[0] = rtb_BittoIntegerConverter[i];
    rtDW->RSDecoder_CCode[1] = rtb_BittoIntegerConverter[i + 1];
    rtDW->RSDecoder_CCode[2] = rtb_BittoIntegerConverter[i + 2];

    /* Assign parity symbols, again accounting for erasures */
    /* no puncturing */
    /*  end of assignInputs()  */
    /* Initialize Gamma(Z) = 1 : ASCENDING ORDER.  length = 2t+1 */
    rtDW->RSDecoder_CCode[3] = rtb_BittoIntegerConverter[i + 3];
    rtDW->RSDecoder_CCode[4] = rtb_BittoIntegerConverter[i + 4];
    rtDW->RSDecoder_CCode[5] = rtb_BittoIntegerConverter[i + 5];
    rtDW->RSDecoder_CCode[6] = rtb_BittoIntegerConverter[i + 6];

    /* Calculate the erasure polynomial GammaZ.GammaZ is the set of coefficients */
    /* of the erasure polynomial in ASCENDING order, because the syndrome is */
    /* calculated in ascending order as well. */
    /*  start of calculateGammaZ() -- Calculate the erasure locator polynomial */
    /*  end of calculateGammaZ()  */
    /* Calculate the syndrome by evaluating the codeword at successive */
    /* powers of alpha.  The syndrome is in ASCENDING order. */
    for (i = 0; i < 4; i++) {
      Temp3 = 0;
      for (j = 6; j >= 0; j--) {
        /*  start of gf_pow():gf_pow raises x^yd  */
        temp = (i + 1) * j * rtConstP_rx_base.RSDecoder_table2[1] % 7;
        if (temp == 0) {
          temp = 7;
        }

        intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];

        /*  end of gf_pow()  */
        /* CCode[nfull-1-j] is the current input code symbol. */
        /* Multiply it by alpha, then get the sum so far. */
        /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
        temp = rtDW->RSDecoder_CCode[6 - j];
        if ((temp == 0) || (intVal == 0)) {
          intVal = 0;
        } else {
          temp = (rtConstP_rx_base.RSDecoder_table2[temp - 1] +
                  rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
          if (temp == 0) {
            temp = 7;
          }

          intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
        }

        /* end of gf_mul() */
        Temp3 ^= intVal;
      }

      if ((noErrorStatus != 0) && (Temp3 != 0)) {
        noErrorStatus = 0;
      }

      RSDecoder_Syndrome[i] = Temp3;
    }

    /* Stop if all syndromes == 0 (i.e. input word is already a valid BCH/RS codeword) */
    if (noErrorStatus == 1) {
      /*  start of assignOutputs():Populate output vectors with proper data */
      /* Corrected message.  If there is a decoding failure, return the input message. */
      rtb_RSDecoder[rtb_uDMaximum * 3] = (uint32_T)rtDW->RSDecoder_CCode[0];
      rtb_RSDecoder[rtb_uDMaximum * 3 + 1] = (uint32_T)rtDW->RSDecoder_CCode[1];
      rtb_RSDecoder[rtb_uDMaximum * 3 + 2] = (uint32_T)rtDW->RSDecoder_CCode[2];

      /* Optional output for # of errors corrected */
      /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
      /* If there is a decoding failure, return the input parity. */
      /*  end of assignOutputs()  */
    } else {
      int32_T RSDecoder_OmegaZActual_idx_1;

      /* Calculate the error/erasure locator polynomial PsiZ */
      /*  start of calculatePsiZ(): Calculate the error/erasure locator polynomial PsiZ */
      /* Use the diagram in Fig. 5-8 of Clark and Cain to implement the algorithm. */
      /* Box 1 -- Initializations */
      /* kCC is the location of the oldest symbol in the LFSR at the point */
      /* where the register fails. */
      noErrorStatus = -1;

      /* discrep is the convolution of PsiZ and the syndrome */
      /* L is the length of the linear feedback shift register (LFSR) */
      RSDecoder_OmegaZActual_idx_1 = 0;

      /* Connection polynomial = 1.  ASCENDING order.  deg(PsiZ) = 2*t. */
      /* To account for erasures, PsiZ = LambdaZ * GammaZ, where LambdaZ is the */
      /* error locator polynomial, and GammaZ is the erasure locator polynomial. */
      /* NOTE:  PsiZ is the coefficients of the connection polynomial in order of */
      /*        ASCENDING powers rather than the conventional descending order. */
      /*        This is such that after the set of iterations, the inverse of */
      /*        roots of PsiZ in descending order can be obtained directly by */
      /*        finding the roots of PsiZ in ascending order.  PsiZ is */
      /*        initialized as GammaZ in case there are erasures.
       */
      /* Initialize Psi(Z) = Gamma(Z) : ASCENDING ORDER.  length = 2t+1 */
      /* Initialize correction polynomial D(z) = z*GammaZ : ASCENDING ORDER. */
      /*   length = 2t+2  */
      rtDW->RSDecoder_Dz[0U] = 0;
      for (i = 0; i < 5; i++) {
        RSDecoder_PsiZ[i] = rtDW->RSDecoder_GammaZ[i];
        RSDecoder_PsiZStar[i] = 0;
        rtDW->RSDecoder_Dz[i + 1] = rtDW->RSDecoder_GammaZ[i];
      }

      /* 2*t-numErasPuncs iterations (Diamond 3) */
      for (j = 0; j < 4; j++) {
        /* Box 2 -- Calculate the discrepancy, which is the sum over i of */
        /*          PsiZ(i)*Syndrome(n-i) with i going from 0 to L  */
        Temp3 = 0;
        for (i = 0; i < RSDecoder_OmegaZActual_idx_1 + 1; i++) {
          temp = j - i;
          if (temp >= 0) {
            /* Check that syndrome position is valid */
            /* Multiply the current Psi coefficient by the */
            /* (nCC-L)'th syndrome value.  Then update sum. */
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            temp = RSDecoder_Syndrome[temp];
            if ((RSDecoder_PsiZ[i] == 0) || (temp == 0)) {
              intVal = 0;
            } else {
              temp = (rtConstP_rx_base.RSDecoder_table2[temp - 1] +
                      rtConstP_rx_base.RSDecoder_table2[RSDecoder_PsiZ[i] - 1]) % 7;
              if (temp == 0) {
                temp = 7;
              }

              intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            Temp3 ^= intVal;
          }
        }

        /* Diamond 1 -- Continue if the discrepancy is not equal to zero */
        if (Temp3 != 0) {
          /* Box 3 -- Connection polynomial */
          /*          PsiZ(n) = PsiZ(n-1) - discrep(n)*Dz */
          for (i = 0; i < 5; i++) {
            RSDecoder_TempVec2t1[i] = Temp3;
          }

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[0] == 0) || (rtDW->RSDecoder_Dz[0] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_TempVec2t1[0] - 1] +
                    rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_Dz[0] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[0] = RSDecoder_PsiZ[0] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[1] == 0) || (rtDW->RSDecoder_Dz[1] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_TempVec2t1[1] - 1] +
                    rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_Dz[1] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[1] = RSDecoder_PsiZ[1] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[2] == 0) || (rtDW->RSDecoder_Dz[2] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_TempVec2t1[2] - 1] +
                    rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_Dz[2] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[2] = RSDecoder_PsiZ[2] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[3] == 0) || (rtDW->RSDecoder_Dz[3] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_TempVec2t1[3] - 1] +
                    rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_Dz[3] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[3] = RSDecoder_PsiZ[3] ^ intVal;

          /* Diamond 2  */
          Temp3 = j - noErrorStatus;
          if (RSDecoder_OmegaZActual_idx_1 < Temp3) {
            /* Boxes 4-7 -- Correction polynomial */
            /*              Dz = PsiZ(n-1) / discrep(n) */
            noErrorStatus = j - RSDecoder_OmegaZActual_idx_1;
            for (i = 0; i < 5; i++) {
              intVal = RSDecoder_PsiZ[i];
              temp = RSDecoder_TempVec2t1[i];

              /*  start of gf_div():gf_div divides the scalars x/b */
              if (temp == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[temp - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((temp == 0) || (intVal == 0)) {
                rtDW->RSDecoder_Dz[i] = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[temp - 1] +
                        rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                rtDW->RSDecoder_Dz[i] = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
            }

            RSDecoder_OmegaZActual_idx_1 = Temp3;
          }

          /* Box 8 -- Reset the error/erasure locator polynomial */
          for (i = 0; i < 5; i++) {
            RSDecoder_PsiZ[i] = RSDecoder_PsiZStar[i];
          }
        }

        /* Box 9 -- Correction polynomial */
        /*           Dz = z * Dz */
        rtDW->RSDecoder_Dz[4] = rtDW->RSDecoder_Dz[3];
        rtDW->RSDecoder_Dz[3] = rtDW->RSDecoder_Dz[2];
        rtDW->RSDecoder_Dz[2] = rtDW->RSDecoder_Dz[1];
        rtDW->RSDecoder_Dz[1] = rtDW->RSDecoder_Dz[0];
        rtDW->RSDecoder_Dz[0U] = 0;
      }

      /*  end of calculatePsiZ  */
      /* FIND ERROR LOCATIONS */
      /* At this point, error/erasure locator polynomial has been found,  which is PsiZ */
      /* Find degree of Psi(Z) */
      Temp3 = 0;
      loopflag = true;
      for (i = 4; i >= 0; i--) {
        if ((RSDecoder_PsiZ[i] > 0) && loopflag) {
          Temp3 = i;
          loopflag = false;
        }
      }

      /* Degree of Psi(Z) must be equal to L and larger than 0 */
      /* (i.e. cannot be a constant) */
      if ((Temp3 != RSDecoder_OmegaZActual_idx_1) || (Temp3 < 1)) {
        /*  start of assignOutputs():Populate output vectors with proper data */
        /* Corrected message.  If there is a decoding failure, return the input message. */
        rtb_RSDecoder[rtb_uDMaximum * 3] = (uint32_T)
          rtb_BittoIntegerConverter[rtb_uDMaximum * 7];
        rtb_RSDecoder[rtb_uDMaximum * 3 + 1] = (uint32_T)
          rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 1];
        rtb_RSDecoder[rtb_uDMaximum * 3 + 2] = (uint32_T)
          rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 2];

        /* Optional output for # of errors corrected */
        /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
        /* If there is a decoding failure, return the input parity. */
        /*  end of assignOutputs()  */
      } else {
        int32_T RSDecoder_OmegaZActual_idx_2;
        int32_T RSDecoder_OmegaZActual_idx_3;

        /* Initialize contents at pointer Errloc */
        RSDecoder_Errloc[0] = 0;
        RSDecoder_Errloc[1] = 0;
        RSDecoder_Errloc[2] = 0;
        RSDecoder_Errloc[3] = 0;

        /*  start of gf_roots()  */
        /* This function takes in vector X, computes the roots and stores them in roots */
        /* it returns the number of roots */
        RSDecoder_OmegaZActual_idx_1 = 5;
        noErrorStatus = 0;
        for (i = 0; i < 5; i++) {
          RSDecoder_PsiZStar[i] = RSDecoder_PsiZ[i];
        }

        i = 0;
        while (i < 8) {
          RSDecoder_OmegaZActual_idx_2 = 0;
          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            rtDW->RSDecoder_d[j] = i;
          }

          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            /*  start of gf_pow():gf_pow raises x^yd  */
            intVal = rtDW->RSDecoder_d[j];
            if (intVal == 0) {
              rtDW->RSDecoder_d[j] = (j == 0);
            } else {
              temp = rtConstP_rx_base.RSDecoder_table2[intVal - 1] * j % 7;
              if (temp == 0) {
                temp = 7;
              }

              rtDW->RSDecoder_d[j] = rtConstP_rx_base.RSDecoder_table1[temp - 1];
            }

            /*  end of gf_pow()  */
          }

          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((rtDW->RSDecoder_d[j] == 0) || (RSDecoder_PsiZStar[j] == 0)) {
              intVal = 0;
            } else {
              temp = (rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_d[j] - 1] +
                      rtConstP_rx_base.RSDecoder_table2[RSDecoder_PsiZStar[j] - 1]) % 7;
              if (temp == 0) {
                temp = 7;
              }

              intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            RSDecoder_OmegaZActual_idx_2 ^= intVal;
          }

          if (RSDecoder_OmegaZActual_idx_2 == 0) {
            RSDecoder_Errloc[noErrorStatus] = i;
            noErrorStatus++;

            /*  start of gf_div():gf_div divides the scalars x/b */
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            intVal = rtConstP_rx_base.RSDecoder_table1[6];

            /* end of gf_mul() */
            /*  end of gf_div() */
            /*  start of gf_div():gf_div divides the scalars x/b */
            if (i == 0) {
              temp = 1;
            } else {
              temp = rtConstP_rx_base.RSDecoder_table1[6 - rtConstP_rx_base.RSDecoder_table2[i -
                1]];
            }

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if (temp == 0) {
              RSDecoder_OmegaZActual_idx_2 = 0;
            } else {
              temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
              if (temp == 0) {
                temp = 7;
              }

              RSDecoder_OmegaZActual_idx_2 = rtConstP_rx_base.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            /*  end of gf_div() */
            /*  start of gf_deconv --- de-convolves A from B  */
            RSDecoder_OmegaZActual_idx_3 = 0;
            for (Temp4 = 0; Temp4 < RSDecoder_OmegaZActual_idx_1; Temp4++) {
              rtDW->RSDecoder_tmpQuotient[Temp4] = 0;
            }

            if (0 < RSDecoder_OmegaZActual_idx_1 - 1) {
              if (rtConstP_rx_base.RSDecoder_table1[6] == 0) {
                inv = 1;
              } else {
                inv = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[rtConstP_rx_base.RSDecoder_table1[6] - 1]];
              }
            }

            for (Temp4 = 0; Temp4 < RSDecoder_OmegaZActual_idx_1 - 1; Temp4++) {
              int32_T z;

              /*  start of gf_div():gf_div divides the scalars x/b */
              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((inv == 0) || (RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3]
                                 == 0)) {
                z = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[inv - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3]
                        - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                z = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              for (j = 1; j < RSDecoder_OmegaZActual_idx_1; j++) {
                rtDW->RSDecoder_tmpQuotient[j - 1] = rtDW->
                  RSDecoder_tmpQuotient[j];
              }

              rtDW->RSDecoder_tmpQuotient[RSDecoder_OmegaZActual_idx_1 - 1] = z;

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((z == 0) || (intVal == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[z - 1] +
                        rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3] ^= temp;

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((z == 0) || (RSDecoder_OmegaZActual_idx_2 == 0)) {
                z = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[z - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_OmegaZActual_idx_2 -
                        1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                z = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3 + 1] ^= z;
              RSDecoder_OmegaZActual_idx_3++;
            }

            for (Temp4 = 0; Temp4 < RSDecoder_OmegaZActual_idx_1 - 1; Temp4++) {
              RSDecoder_PsiZStar[Temp4] = rtDW->RSDecoder_tmpQuotient[Temp4 + 1];
            }

            /*  end of gf_deconv  */
            RSDecoder_OmegaZActual_idx_1--;
            i--;
          }

          i++;
        }

        for (j = 0; j < noErrorStatus; j++) {
          /*  start of gf_div():gf_div divides the scalars x/b */
          if (RSDecoder_Errloc[j] == 0) {
            temp = 1;
          } else {
            temp = rtConstP_rx_base.RSDecoder_table1[6 -
              rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
          }

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if (temp == 0) {
            RSDecoder_Errloc[j] = 0;
          } else {
            temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
            if (temp == 0) {
              temp = 7;
            }

            RSDecoder_Errloc[j] = rtConstP_rx_base.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          /*  end of gf_div() */
        }

        /*  end of gf_roots() */
        /* Decoding failure if one of the following conditions is met: */
        /* (1) Psi(Z) has no roots in this field */
        /* (2) Number of roots not equal to degree of PsiZ */
        if (noErrorStatus != Temp3) {
          /*  start of assignOutputs():Populate output vectors with proper data */
          /* Corrected message.  If there is a decoding failure, return the input message. */
          rtb_RSDecoder[rtb_uDMaximum * 3] = (uint32_T)
            rtb_BittoIntegerConverter[rtb_uDMaximum * 7];
          rtb_RSDecoder[rtb_uDMaximum * 3 + 1] = (uint32_T)
            rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 1];
          rtb_RSDecoder[rtb_uDMaximum * 3 + 2] = (uint32_T)
            rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 2];

          /* Optional output for # of errors corrected */
          /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
          /* If there is a decoding failure, return the input parity. */
          /*  end of assignOutputs()  */
        } else {
          /* Test if the error locations are unique
           */
          loopflag = true;
          i = 0;
          while ((i < noErrorStatus - 1) && loopflag) {
            j = i + 1;
            while ((j < noErrorStatus) && loopflag) {
              loopflag = (RSDecoder_Errloc[i] != RSDecoder_Errloc[j]);
              j++;
            }

            i++;
          }

          if (!loopflag) {
            /*  start of assignOutputs():Populate output vectors with proper data */
            /* Corrected message.  If there is a decoding failure, return the input message. */
            rtb_RSDecoder[rtb_uDMaximum * 3] = (uint32_T)
              rtb_BittoIntegerConverter[rtb_uDMaximum * 7];
            rtb_RSDecoder[rtb_uDMaximum * 3 + 1] = (uint32_T)
              rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 1];
            rtb_RSDecoder[rtb_uDMaximum * 3 + 2] = (uint32_T)
              rtb_BittoIntegerConverter[rtb_uDMaximum * 7 + 2];

            /* Optional output for # of errors corrected */
            /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
            /* If there is a decoding failure, return the input parity. */
            /*  end of assignOutputs()  */
          } else {
            /* PART II - FIND ERROR MAGNITUDES AT EACH OF THE ERROR/ERASURE LOCATIONS, */
            /* AND CORRECT THEM */
            /*  start of correctErrors():Calculate the error magnitude in the current error position,  */
            /*                           and correct the errors. */
            for (i = 0; i < 10; i++) {
              RSDecoder_OmegaZ[i] = 0;
            }

            /*  start of gf_conv():gf_conv convolves A with B and stores the result in retValue  */
            for (i = 0; i < 8; i++) {
              RSDecoder_OmegaZ[i] = 0;
            }

            for (i = 0; i < 5; i++) {
              intVal = RSDecoder_PsiZ[i];
              if ((intVal == 0) || (RSDecoder_Syndrome[0] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_Syndrome[0] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[i] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[1] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_Syndrome[1] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[i + 1] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[2] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_Syndrome[2] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[i + 2] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[3] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] +
                        rtConstP_rx_base.RSDecoder_table2[RSDecoder_Syndrome[3] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[i + 3] ^= temp;
            }

            /*  end of gf_conv()  */
            /* Disregard terms of x^(2t) and higher in Omega(Z) */
            /* because we have no knowledge of such terms in S(Z). */
            /* That is, retain terms up to x^(2t-1) */
            i = RSDecoder_OmegaZ[0];
            RSDecoder_OmegaZActual_idx_1 = RSDecoder_OmegaZ[1];
            RSDecoder_OmegaZActual_idx_2 = RSDecoder_OmegaZ[2];
            RSDecoder_OmegaZActual_idx_3 = RSDecoder_OmegaZ[3];

            /* Compute derivative of PsiZ */
            rtDW->RSDecoder_PsiZDeriv[0] = RSDecoder_PsiZ[1];
            rtDW->RSDecoder_PsiZDeriv[2] = RSDecoder_PsiZ[3];
            rtDW->RSDecoder_PsiZDeriv[1] = 0;
            rtDW->RSDecoder_PsiZDeriv[3] = 0;

            /* Find error magnitude at each error location.  Use the expression found on pg. 222 of Wicker. */
            for (j = 0; j < noErrorStatus; j++) {
              /* Dot product for numerator term */
              Temp3 = 0;

              /* Dot product for denominator */
              Temp4 = 0;

              /*  start of gf_pow():gf_pow raises x^yd  */
              if (RSDecoder_Errloc[j] == 0) {
                intVal = 1;
              } else {
                intVal = rtConstP_rx_base.RSDecoder_table1[6];
              }

              /*  end of gf_pow()  */
              if (i > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  Temp3 = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[i - 1] +
                          rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  Temp3 = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              if (RSDecoder_Errloc[j] == 0) {
                intVal = 1;
              } else {
                intVal = rtConstP_rx_base.RSDecoder_table1[6];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[0] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[0] == 0) || (intVal == 0)) {
                  Temp4 = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[0]
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  Temp4 = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP_rx_base.RSDecoder_table2[intVal - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_1 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_OmegaZActual_idx_1
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP_rx_base.RSDecoder_table2[intVal - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[1] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[1] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[1]
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] << 1) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_2 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_OmegaZActual_idx_2
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = (rtConstP_rx_base.RSDecoder_table2[intVal - 1] << 1) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[2] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[2] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[2]
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP_rx_base.RSDecoder_table2[intVal - 1] * 3 % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_3 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[RSDecoder_OmegaZActual_idx_3
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP_rx_base.RSDecoder_table2[intVal - 1] * 3 % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[3] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[3] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP_rx_base.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[3]
                          - 1] + rtConstP_rx_base.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /* Re-use space in Temp1 */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (Temp4 == 0) {
                temp = 1;
              } else {
                temp = rtConstP_rx_base.RSDecoder_table1[6 -
                  rtConstP_rx_base.RSDecoder_table2[Temp4 - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((temp == 0) || (Temp3 == 0)) {
                intVal = 0;
              } else {
                temp = (rtConstP_rx_base.RSDecoder_table2[temp - 1] +
                        rtConstP_rx_base.RSDecoder_table2[Temp3 - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP_rx_base.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              /* Find exponent representations of Errloc ==> get actual error locations */
              /* Correct the current error */
              temp = 6 - rtConstP_rx_base.RSDecoder_table2[RSDecoder_Errloc[j] - 1];
              rtDW->RSDecoder_CCode[temp] ^= intVal;
            }

            /*  end of correctErrors()  */
            /* Assign outputs.  Reduce cnumerr by the number of punctures and erasures. */
            /*  start of assignOutputs():Populate output vectors with proper data */
            /* Corrected message.  If there is a decoding failure, return the input message. */
            rtb_RSDecoder[rtb_uDMaximum * 3] = (uint32_T)rtDW->RSDecoder_CCode[0];
            rtb_RSDecoder[rtb_uDMaximum * 3 + 1] = (uint32_T)
              rtDW->RSDecoder_CCode[1];
            rtb_RSDecoder[rtb_uDMaximum * 3 + 2] = (uint32_T)
              rtDW->RSDecoder_CCode[2];

            /* Optional output for # of errors corrected */
            /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
            /* If there is a decoding failure, return the input parity. */
            /*  end of assignOutputs()  */
          }
        }
      }
    }
  }

  /* End of S-Function (scomberlekamp): '<S6>/RS Decoder' */

  /* S-Function (scominttobit): '<S6>/Integer to Bit Converter' incorporates:
   *  S-Function (scomberlekamp): '<S6>/RS Decoder'
   */
  /*  end of COMM_DoBerlekamp()  */
  /* Integer to Bit Conversion */
  for (i = 0; i < 264; i++) {
    rtb_uDMaximum = (i + 1) * 3;
    u = rtb_RSDecoder[i];
    rtb_IntegertoBitConverter[rtb_uDMaximum - 1] = (int8_T)(u & 1U);
    u >>= 1U;
    rtb_IntegertoBitConverter[rtb_uDMaximum - 2] = (int8_T)(u & 1U);
    rtb_IntegertoBitConverter[rtb_uDMaximum - 3] = (int8_T)(u >> 1U & 1U);
  }

  /* End of S-Function (scominttobit): '<S6>/Integer to Bit Converter' */

  /* DataTypeConversion: '<S7>/Conversion' incorporates:
   *  S-Function (scominttobit): '<S6>/Integer to Bit Converter'
   */
  for (i = 0; i < 792; i++) {
    rtb_Conversion[i] = ((uint8_T)rtb_IntegertoBitConverter[i] != 0);
  }

  /* End of DataTypeConversion: '<S7>/Conversion' */

  /* Outport: '<Root>/descr_in' */
  memcpy(&rtY_descr_in[0], &rtb_Conversion[0], 784U * sizeof(boolean_T));

  /* S-Function (scomscram2): '<S3>/Descrambler' */
  memset(&shiftReg[0], 0, sizeof(int32_T) << 4U);
  for (j = 0; j < 784; j++) {
    inv = rtb_Conversion[j];
    rtb_uDMaximum = inv;
    for (i = 0; i < 16; i++) {
      rtb_uDMaximum -= (uint8_T)(rtConstP_rx_base.Descrambler_Polynomial[i + 1] *
        shiftReg[i]);
    }

    while (rtb_uDMaximum < 0) {
      rtb_uDMaximum += 2;
    }

    rtY_bits_out[j] = (rtb_uDMaximum % 2 != 0);
    for (i = 14; i >= 0; i--) {
      shiftReg[i + 1] = shiftReg[i];
    }

    shiftReg[0U] = inv;
  }

  /* Outputs for Iterator SubSystem: '<S5>/bit_concat_per_col' incorporates:
   *  ForEach: '<S9>/For Each'
   */
  for (i = 0; i < 98; i++) {
    /* Outputs for Atomic SubSystem: '<S10>/bc4' */
    /* MATLAB Function: '<S11>/bit_concat_unary' incorporates:
     *  ForEachSliceSelector generated from: '<S9>/bits'
     *  S-Function (scomscram2): '<S3>/Descrambler'
     */
    for (inv = 0; inv < 8; inv++) {
      y[inv] = (int8_T)rtY_bits_out[(i << 3) + inv];
    }

    /* ForEachSliceAssignment generated from: '<S9>/bytes' incorporates:
     *  MATLAB Function: '<S11>/bit_concat_unary'
     */
    rtY_data_frame[i] = (uint8_T)((uint8_T)y[6] << 1 | (uint8_T)y[7] | (uint8_T)
      y[5] << 2 | (uint8_T)y[4] << 3 | (uint8_T)y[3] << 4 | (uint8_T)y[2] << 5 |
      (uint8_T)y[1] << 6 | (uint8_T)y[0] << 7);

    /* End of Outputs for SubSystem: '<S10>/bc4' */
  }

  /* End of Outputs for SubSystem: '<S5>/bit_concat_per_col' */
}

/* Model initialize function */
void V2X_RX_Baseband_initialize(RT_MODEL *const rtM, boolean_T rtU_v2x_rx_bb_in
  [1976], uint8_T rtY_data_frame[98], boolean_T rtY_dec_in[1848], boolean_T
  rtY_descr_in[784], boolean_T rtY_bits_out[784])
{
  DW *rtDW = rtM->dwork;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(&rtU_v2x_rx_bb_in[0], 0, 1976U * sizeof(boolean_T));

  /* external outputs */
  (void)memset(&rtY_data_frame[0], 0, 98U * sizeof(uint8_T));
  (void)memset(&rtY_dec_in[0], 0, 1848U * sizeof(boolean_T));
  (void)memset(&rtY_descr_in[0], 0, 784U * sizeof(boolean_T));
  (void)memset(&rtY_bits_out[0], 0, 784U * sizeof(boolean_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
