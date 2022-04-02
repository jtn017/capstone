/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 *
 * File: V2X_RX_Baseband.c
 *
 * Code generated for Simulink model 'V2X_RX_Baseband'.
 *
 * Model version                  : 1.150
 * Simulink Coder version         : 9.6 (R2021b) 14-May-2021
 * C/C++ source code generated on : Sat Apr  2 13:40:57 2022
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
void V2X_RX_Baseband_step(RT_MODEL *const rtM, boolean_T rtU_v2x_rx_demod_out
  [16928], boolean_T rtY_data_frame[7200], boolean_T rtY_dec_in[16800],
  boolean_T rtY_descr_in[7200])
{
  DW *rtDW = rtM->dwork;
  int32_T shiftReg[16];
  int32_T RSDecoder_OmegaZ[10];
  int32_T RSDecoder_PsiZ[5];
  int32_T RSDecoder_PsiZStar[5];
  int32_T RSDecoder_TempVec2t1[5];
  int32_T RSDecoder_Errloc[4];
  int32_T RSDecoder_Syndrome[4];
  int32_T RSDecoder_OmegaZActual_idx_0;
  int32_T Temp4;
  int32_T bitIdx;
  int32_T intVal;
  int32_T inv;
  int32_T j;

  /* S-Function (scominttobit): '<S5>/Bit to Integer Converter' incorporates:
   *  Inport: '<Root>/rx_frame'
   */
  /* Bit to Integer Conversion */
  bitIdx = 128;
  for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 5600;
       RSDecoder_OmegaZActual_idx_0++) {
    /* Input bit order is MSB first */
    intVal = (int32_T)(((uint32_T)rtU_v2x_rx_demod_out[bitIdx] << 1U |
                        rtU_v2x_rx_demod_out[bitIdx + 1]) << 1U |
                       rtU_v2x_rx_demod_out[bitIdx + 2]);
    bitIdx += 3;
    rtDW->BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0] = (int8_T)intVal;
  }

  /* End of S-Function (scominttobit): '<S5>/Bit to Integer Converter' */

  /* S-Function (scomberlekamp): '<S5>/RS Decoder' incorporates:
   *  S-Function (scominttobit): '<S5>/Bit to Integer Converter'
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
  for (bitIdx = 0; bitIdx < 800; bitIdx++) {
    int32_T Temp3;
    int32_T noErrorStatus;
    int32_T temp;
    noErrorStatus = 1;

    /*  start of assignInputs(): Populate the codeword and erasure vectors with the proper data  */
    /* Assign message symbols. */
    /* If there are erasures, insert zeros in the erased positions. */
    RSDecoder_OmegaZActual_idx_0 = bitIdx * 7;
    rtDW->RSDecoder_CCode[0] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0];
    rtDW->RSDecoder_CCode[1] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 1];
    rtDW->RSDecoder_CCode[2] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 2];

    /* Assign parity symbols, again accounting for erasures */
    /* no puncturing */
    /*  end of assignInputs()  */
    /* Initialize Gamma(Z) = 1 : ASCENDING ORDER.  length = 2t+1 */
    rtDW->RSDecoder_CCode[3] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 3];
    rtDW->RSDecoder_CCode[4] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 4];
    rtDW->RSDecoder_CCode[5] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 5];
    rtDW->RSDecoder_CCode[6] = rtDW->
      BittoIntegerConverter[RSDecoder_OmegaZActual_idx_0 + 6];

    /* Calculate the erasure polynomial GammaZ.GammaZ is the set of coefficients */
    /* of the erasure polynomial in ASCENDING order, because the syndrome is */
    /* calculated in ascending order as well. */
    /*  start of calculateGammaZ() -- Calculate the erasure locator polynomial */
    /*  end of calculateGammaZ()  */
    /* Calculate the syndrome by evaluating the codeword at successive */
    /* powers of alpha.  The syndrome is in ASCENDING order. */
    for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 4;
         RSDecoder_OmegaZActual_idx_0++) {
      Temp3 = 0;
      for (j = 6; j >= 0; j--) {
        /*  start of gf_pow():gf_pow raises x^yd  */
        temp = (RSDecoder_OmegaZActual_idx_0 + 1) * j *
          rtConstP.RSDecoder_table2[1] % 7;
        if (temp == 0) {
          temp = 7;
        }

        intVal = rtConstP.RSDecoder_table1[temp - 1];

        /*  end of gf_pow()  */
        /* CCode[nfull-1-j] is the current input code symbol. */
        /* Multiply it by alpha, then get the sum so far. */
        /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
        temp = rtDW->RSDecoder_CCode[6 - j];
        if ((temp == 0) || (intVal == 0)) {
          intVal = 0;
        } else {
          temp = (rtConstP.RSDecoder_table2[temp - 1] +
                  rtConstP.RSDecoder_table2[intVal - 1]) % 7;
          if (temp == 0) {
            temp = 7;
          }

          intVal = rtConstP.RSDecoder_table1[temp - 1];
        }

        /* end of gf_mul() */
        Temp3 ^= intVal;
      }

      if ((noErrorStatus != 0) && (Temp3 != 0)) {
        noErrorStatus = 0;
      }

      RSDecoder_Syndrome[RSDecoder_OmegaZActual_idx_0] = Temp3;
    }

    /* Stop if all syndromes == 0 (i.e. input word is already a valid BCH/RS codeword) */
    if (noErrorStatus == 1) {
      /*  start of assignOutputs():Populate output vectors with proper data */
      /* Corrected message.  If there is a decoding failure, return the input message. */
      rtDW->RSDecoder[bitIdx * 3] = (uint32_T)rtDW->RSDecoder_CCode[0];
      rtDW->RSDecoder[bitIdx * 3 + 1] = (uint32_T)rtDW->RSDecoder_CCode[1];
      rtDW->RSDecoder[bitIdx * 3 + 2] = (uint32_T)rtDW->RSDecoder_CCode[2];

      /* Optional output for # of errors corrected */
      /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
      /* If there is a decoding failure, return the input parity. */
      /*  end of assignOutputs()  */
    } else {
      int32_T RSDecoder_OmegaZActual_idx_1;
      boolean_T loopflag;

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
      for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 5;
           RSDecoder_OmegaZActual_idx_0++) {
        RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0] = rtDW->
          RSDecoder_GammaZ[RSDecoder_OmegaZActual_idx_0];
        RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_0] = 0;
        rtDW->RSDecoder_Dz[RSDecoder_OmegaZActual_idx_0 + 1] =
          rtDW->RSDecoder_GammaZ[RSDecoder_OmegaZActual_idx_0];
      }

      /* 2*t-numErasPuncs iterations (Diamond 3) */
      for (j = 0; j < 4; j++) {
        /* Box 2 -- Calculate the discrepancy, which is the sum over i of */
        /*          PsiZ(i)*Syndrome(n-i) with i going from 0 to L  */
        Temp3 = 0;
        for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
             RSDecoder_OmegaZActual_idx_1 + 1; RSDecoder_OmegaZActual_idx_0++) {
          temp = j - RSDecoder_OmegaZActual_idx_0;
          if (temp >= 0) {
            /* Check that syndrome position is valid */
            /* Multiply the current Psi coefficient by the */
            /* (nCC-L)'th syndrome value.  Then update sum. */
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            temp = RSDecoder_Syndrome[temp];
            if ((RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0] == 0) || (temp ==
                 0)) {
              intVal = 0;
            } else {
              temp = (rtConstP.RSDecoder_table2[temp - 1] +
                      rtConstP.RSDecoder_table2[RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0]
                      - 1]) % 7;
              if (temp == 0) {
                temp = 7;
              }

              intVal = rtConstP.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            Temp3 ^= intVal;
          }
        }

        /* Diamond 1 -- Continue if the discrepancy is not equal to zero */
        if (Temp3 != 0) {
          /* Box 3 -- Connection polynomial */
          /*          PsiZ(n) = PsiZ(n-1) - discrep(n)*Dz */
          for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
               5; RSDecoder_OmegaZActual_idx_0++) {
            RSDecoder_TempVec2t1[RSDecoder_OmegaZActual_idx_0] = Temp3;
          }

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[0] == 0) || (rtDW->RSDecoder_Dz[0] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP.RSDecoder_table2[RSDecoder_TempVec2t1[0] - 1] +
                    rtConstP.RSDecoder_table2[rtDW->RSDecoder_Dz[0] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[0] = RSDecoder_PsiZ[0] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[1] == 0) || (rtDW->RSDecoder_Dz[1] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP.RSDecoder_table2[RSDecoder_TempVec2t1[1] - 1] +
                    rtConstP.RSDecoder_table2[rtDW->RSDecoder_Dz[1] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[1] = RSDecoder_PsiZ[1] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[2] == 0) || (rtDW->RSDecoder_Dz[2] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP.RSDecoder_table2[RSDecoder_TempVec2t1[2] - 1] +
                    rtConstP.RSDecoder_table2[rtDW->RSDecoder_Dz[2] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[2] = RSDecoder_PsiZ[2] ^ intVal;

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if ((RSDecoder_TempVec2t1[3] == 0) || (rtDW->RSDecoder_Dz[3] == 0)) {
            intVal = 0;
          } else {
            temp = (rtConstP.RSDecoder_table2[RSDecoder_TempVec2t1[3] - 1] +
                    rtConstP.RSDecoder_table2[rtDW->RSDecoder_Dz[3] - 1]) % 7;
            if (temp == 0) {
              temp = 7;
            }

            intVal = rtConstP.RSDecoder_table1[temp - 1];
          }

          /* end of gf_mul() */
          RSDecoder_PsiZStar[3] = RSDecoder_PsiZ[3] ^ intVal;

          /* Diamond 2  */
          Temp3 = j - noErrorStatus;
          if (RSDecoder_OmegaZActual_idx_1 < Temp3) {
            /* Boxes 4-7 -- Correction polynomial */
            /*              Dz = PsiZ(n-1) / discrep(n) */
            noErrorStatus = j - RSDecoder_OmegaZActual_idx_1;
            for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
                 5; RSDecoder_OmegaZActual_idx_0++) {
              intVal = RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0];
              temp = RSDecoder_TempVec2t1[RSDecoder_OmegaZActual_idx_0];

              /*  start of gf_div():gf_div divides the scalars x/b */
              if (temp == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[temp - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((temp == 0) || (intVal == 0)) {
                rtDW->RSDecoder_Dz[RSDecoder_OmegaZActual_idx_0] = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[temp - 1] +
                        rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                rtDW->RSDecoder_Dz[RSDecoder_OmegaZActual_idx_0] =
                  rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
            }

            RSDecoder_OmegaZActual_idx_1 = Temp3;
          }

          /* Box 8 -- Reset the error/erasure locator polynomial */
          for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
               5; RSDecoder_OmegaZActual_idx_0++) {
            RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0] =
              RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_0];
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
      for (RSDecoder_OmegaZActual_idx_0 = 4; RSDecoder_OmegaZActual_idx_0 >= 0;
           RSDecoder_OmegaZActual_idx_0--) {
        if ((RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0] > 0) && loopflag) {
          Temp3 = RSDecoder_OmegaZActual_idx_0;
          loopflag = false;
        }
      }

      /* Degree of Psi(Z) must be equal to L and larger than 0 */
      /* (i.e. cannot be a constant) */
      if ((Temp3 != RSDecoder_OmegaZActual_idx_1) || (Temp3 < 1)) {
        /*  start of assignOutputs():Populate output vectors with proper data */
        /* Corrected message.  If there is a decoding failure, return the input message. */
        rtDW->RSDecoder[bitIdx * 3] = (uint32_T)rtDW->
          BittoIntegerConverter[bitIdx * 7];
        rtDW->RSDecoder[bitIdx * 3 + 1] = (uint32_T)rtDW->
          BittoIntegerConverter[bitIdx * 7 + 1];
        rtDW->RSDecoder[bitIdx * 3 + 2] = (uint32_T)rtDW->
          BittoIntegerConverter[bitIdx * 7 + 2];

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
        for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 5;
             RSDecoder_OmegaZActual_idx_0++) {
          RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_0] =
            RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0];
        }

        RSDecoder_OmegaZActual_idx_0 = 0;
        while (RSDecoder_OmegaZActual_idx_0 < 8) {
          RSDecoder_OmegaZActual_idx_2 = 0;
          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            rtDW->RSDecoder_d[j] = RSDecoder_OmegaZActual_idx_0;
          }

          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            /*  start of gf_pow():gf_pow raises x^yd  */
            intVal = rtDW->RSDecoder_d[j];
            if (intVal == 0) {
              rtDW->RSDecoder_d[j] = (j == 0);
            } else {
              temp = rtConstP.RSDecoder_table2[intVal - 1] * j % 7;
              if (temp == 0) {
                temp = 7;
              }

              rtDW->RSDecoder_d[j] = rtConstP.RSDecoder_table1[temp - 1];
            }

            /*  end of gf_pow()  */
          }

          for (j = 0; j < RSDecoder_OmegaZActual_idx_1; j++) {
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if ((rtDW->RSDecoder_d[j] == 0) || (RSDecoder_PsiZStar[j] == 0)) {
              intVal = 0;
            } else {
              temp = (rtConstP.RSDecoder_table2[rtDW->RSDecoder_d[j] - 1] +
                      rtConstP.RSDecoder_table2[RSDecoder_PsiZStar[j] - 1]) % 7;
              if (temp == 0) {
                temp = 7;
              }

              intVal = rtConstP.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            RSDecoder_OmegaZActual_idx_2 ^= intVal;
          }

          if (RSDecoder_OmegaZActual_idx_2 == 0) {
            RSDecoder_Errloc[noErrorStatus] = RSDecoder_OmegaZActual_idx_0;
            noErrorStatus++;

            /*  start of gf_div():gf_div divides the scalars x/b */
            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            intVal = rtConstP.RSDecoder_table1[6];

            /* end of gf_mul() */
            /*  end of gf_div() */
            /*  start of gf_div():gf_div divides the scalars x/b */
            if (RSDecoder_OmegaZActual_idx_0 == 0) {
              temp = 1;
            } else {
              temp = rtConstP.RSDecoder_table1[6 -
                rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_0 - 1]];
            }

            /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
            if (temp == 0) {
              RSDecoder_OmegaZActual_idx_2 = 0;
            } else {
              temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
              if (temp == 0) {
                temp = 7;
              }

              RSDecoder_OmegaZActual_idx_2 = rtConstP.RSDecoder_table1[temp - 1];
            }

            /* end of gf_mul() */
            /*  end of gf_div() */
            /*  start of gf_deconv --- de-convolves A from B  */
            RSDecoder_OmegaZActual_idx_3 = 0;
            for (Temp4 = 0; Temp4 < RSDecoder_OmegaZActual_idx_1; Temp4++) {
              rtDW->RSDecoder_tmpQuotient[Temp4] = 0;
            }

            if (0 < RSDecoder_OmegaZActual_idx_1 - 1) {
              if (rtConstP.RSDecoder_table1[6] == 0) {
                inv = 1;
              } else {
                inv = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[rtConstP.RSDecoder_table1[6] - 1]];
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
                temp = (rtConstP.RSDecoder_table2[inv - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3]
                        - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                z = rtConstP.RSDecoder_table1[temp - 1];
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
                temp = (rtConstP.RSDecoder_table2[z - 1] +
                        rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              RSDecoder_PsiZStar[RSDecoder_OmegaZActual_idx_3] ^= temp;

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((z == 0) || (RSDecoder_OmegaZActual_idx_2 == 0)) {
                z = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[z - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_2 -
                        1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                z = rtConstP.RSDecoder_table1[temp - 1];
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
            RSDecoder_OmegaZActual_idx_0--;
          }

          RSDecoder_OmegaZActual_idx_0++;
        }

        for (j = 0; j < noErrorStatus; j++) {
          /*  start of gf_div():gf_div divides the scalars x/b */
          if (RSDecoder_Errloc[j] == 0) {
            temp = 1;
          } else {
            temp = rtConstP.RSDecoder_table1[6 -
              rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
          }

          /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
          if (temp == 0) {
            RSDecoder_Errloc[j] = 0;
          } else {
            temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
            if (temp == 0) {
              temp = 7;
            }

            RSDecoder_Errloc[j] = rtConstP.RSDecoder_table1[temp - 1];
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
          rtDW->RSDecoder[bitIdx * 3] = (uint32_T)rtDW->
            BittoIntegerConverter[bitIdx * 7];
          rtDW->RSDecoder[bitIdx * 3 + 1] = (uint32_T)
            rtDW->BittoIntegerConverter[bitIdx * 7 + 1];
          rtDW->RSDecoder[bitIdx * 3 + 2] = (uint32_T)
            rtDW->BittoIntegerConverter[bitIdx * 7 + 2];

          /* Optional output for # of errors corrected */
          /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
          /* If there is a decoding failure, return the input parity. */
          /*  end of assignOutputs()  */
        } else {
          /* Test if the error locations are unique
           */
          loopflag = true;
          RSDecoder_OmegaZActual_idx_0 = 0;
          while ((RSDecoder_OmegaZActual_idx_0 < noErrorStatus - 1) && loopflag)
          {
            j = RSDecoder_OmegaZActual_idx_0 + 1;
            while ((j < noErrorStatus) && loopflag) {
              loopflag = (RSDecoder_Errloc[RSDecoder_OmegaZActual_idx_0] !=
                          RSDecoder_Errloc[j]);
              j++;
            }

            RSDecoder_OmegaZActual_idx_0++;
          }

          if (!loopflag) {
            /*  start of assignOutputs():Populate output vectors with proper data */
            /* Corrected message.  If there is a decoding failure, return the input message. */
            rtDW->RSDecoder[bitIdx * 3] = (uint32_T)rtDW->
              BittoIntegerConverter[bitIdx * 7];
            rtDW->RSDecoder[bitIdx * 3 + 1] = (uint32_T)
              rtDW->BittoIntegerConverter[bitIdx * 7 + 1];
            rtDW->RSDecoder[bitIdx * 3 + 2] = (uint32_T)
              rtDW->BittoIntegerConverter[bitIdx * 7 + 2];

            /* Optional output for # of errors corrected */
            /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
            /* If there is a decoding failure, return the input parity. */
            /*  end of assignOutputs()  */
          } else {
            /* PART II - FIND ERROR MAGNITUDES AT EACH OF THE ERROR/ERASURE LOCATIONS, */
            /* AND CORRECT THEM */
            /*  start of correctErrors():Calculate the error magnitude in the current error position,  */
            /*                           and correct the errors. */
            for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
                 10; RSDecoder_OmegaZActual_idx_0++) {
              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0] = 0;
            }

            /*  start of gf_conv():gf_conv convolves A with B and stores the result in retValue  */
            for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
                 8; RSDecoder_OmegaZActual_idx_0++) {
              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0] = 0;
            }

            for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 <
                 5; RSDecoder_OmegaZActual_idx_0++) {
              intVal = RSDecoder_PsiZ[RSDecoder_OmegaZActual_idx_0];
              if ((intVal == 0) || (RSDecoder_Syndrome[0] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_Syndrome[0] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[1] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_Syndrome[1] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0 + 1] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[2] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_Syndrome[2] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0 + 2] ^= temp;
              if ((intVal == 0) || (RSDecoder_Syndrome[3] == 0)) {
                temp = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] +
                        rtConstP.RSDecoder_table2[RSDecoder_Syndrome[3] - 1]) %
                  7;
                if (temp == 0) {
                  temp = 7;
                }

                temp = rtConstP.RSDecoder_table1[temp - 1];
              }

              RSDecoder_OmegaZ[RSDecoder_OmegaZActual_idx_0 + 3] ^= temp;
            }

            /*  end of gf_conv()  */
            /* Disregard terms of x^(2t) and higher in Omega(Z) */
            /* because we have no knowledge of such terms in S(Z). */
            /* That is, retain terms up to x^(2t-1) */
            RSDecoder_OmegaZActual_idx_0 = RSDecoder_OmegaZ[0];
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
                intVal = rtConstP.RSDecoder_table1[6];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_0 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  Temp3 = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_0
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  Temp3 = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              if (RSDecoder_Errloc[j] == 0) {
                intVal = 1;
              } else {
                intVal = rtConstP.RSDecoder_table1[6];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[0] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[0] == 0) || (intVal == 0)) {
                  Temp4 = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[0]
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  Temp4 = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP.RSDecoder_table2[intVal - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_1 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_1
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP.RSDecoder_table2[intVal - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[1] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[1] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[1]
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] << 1) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_2 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_2
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = (rtConstP.RSDecoder_table2[intVal - 1] << 1) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[2] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[2] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[2]
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP.RSDecoder_table2[intVal - 1] * 3 % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (RSDecoder_OmegaZActual_idx_3 > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if (intVal == 0) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[RSDecoder_OmegaZActual_idx_3
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp3 ^= intVal;
              }

              /*  start of gf_pow():gf_pow raises x^yd  */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (RSDecoder_Errloc[j] == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if (temp == 0) {
                intVal = 0;
              } else {
                temp = rtConstP.RSDecoder_table2[temp - 1] % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              if (intVal != 0) {
                temp = rtConstP.RSDecoder_table2[intVal - 1] * 3 % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /*  end of gf_pow()  */
              if (rtDW->RSDecoder_PsiZDeriv[3] > 0) {
                /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
                if ((rtDW->RSDecoder_PsiZDeriv[3] == 0) || (intVal == 0)) {
                  intVal = 0;
                } else {
                  temp = (rtConstP.RSDecoder_table2[rtDW->RSDecoder_PsiZDeriv[3]
                          - 1] + rtConstP.RSDecoder_table2[intVal - 1]) % 7;
                  if (temp == 0) {
                    temp = 7;
                  }

                  intVal = rtConstP.RSDecoder_table1[temp - 1];
                }

                /* end of gf_mul() */
                Temp4 ^= intVal;
              }

              /* Re-use space in Temp1 */
              /*  start of gf_div():gf_div divides the scalars x/b */
              if (Temp4 == 0) {
                temp = 1;
              } else {
                temp = rtConstP.RSDecoder_table1[6 -
                  rtConstP.RSDecoder_table2[Temp4 - 1]];
              }

              /*  start of gf_mul: gf_mul multiplies the scalars  a * b */
              if ((temp == 0) || (Temp3 == 0)) {
                intVal = 0;
              } else {
                temp = (rtConstP.RSDecoder_table2[temp - 1] +
                        rtConstP.RSDecoder_table2[Temp3 - 1]) % 7;
                if (temp == 0) {
                  temp = 7;
                }

                intVal = rtConstP.RSDecoder_table1[temp - 1];
              }

              /* end of gf_mul() */
              /*  end of gf_div() */
              /* Find exponent representations of Errloc ==> get actual error locations */
              /* Correct the current error */
              temp = 6 - rtConstP.RSDecoder_table2[RSDecoder_Errloc[j] - 1];
              rtDW->RSDecoder_CCode[temp] ^= intVal;
            }

            /*  end of correctErrors()  */
            /* Assign outputs.  Reduce cnumerr by the number of punctures and erasures. */
            /*  start of assignOutputs():Populate output vectors with proper data */
            /* Corrected message.  If there is a decoding failure, return the input message. */
            rtDW->RSDecoder[bitIdx * 3] = (uint32_T)rtDW->RSDecoder_CCode[0];
            rtDW->RSDecoder[bitIdx * 3 + 1] = (uint32_T)rtDW->RSDecoder_CCode[1];
            rtDW->RSDecoder[bitIdx * 3 + 2] = (uint32_T)rtDW->RSDecoder_CCode[2];

            /* Optional output for # of errors corrected */
            /* Parity of corrected codeword. If it is punctured, remove the punctured symbols. */
            /* If there is a decoding failure, return the input parity. */
            /*  end of assignOutputs()  */
          }
        }
      }
    }
  }

  /* End of S-Function (scomberlekamp): '<S5>/RS Decoder' */

  /* S-Function (scominttobit): '<S5>/Integer to Bit Converter' incorporates:
   *  S-Function (scomberlekamp): '<S5>/RS Decoder'
   */
  /*  end of COMM_DoBerlekamp()  */
  /* Integer to Bit Conversion */
  for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 2400;
       RSDecoder_OmegaZActual_idx_0++) {
    uint32_T u;
    bitIdx = (RSDecoder_OmegaZActual_idx_0 + 1) * 3;
    u = rtDW->RSDecoder[RSDecoder_OmegaZActual_idx_0];
    rtDW->IntegertoBitConverter[bitIdx - 1] = (int8_T)(u & 1U);
    u >>= 1U;
    rtDW->IntegertoBitConverter[bitIdx - 2] = (int8_T)(u & 1U);
    rtDW->IntegertoBitConverter[bitIdx - 3] = (int8_T)(u >> 1U & 1U);
  }

  /* End of S-Function (scominttobit): '<S5>/Integer to Bit Converter' */

  /* DataTypeConversion: '<S6>/Conversion' incorporates:
   *  S-Function (scominttobit): '<S5>/Integer to Bit Converter'
   */
  for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 7200;
       RSDecoder_OmegaZActual_idx_0++) {
    rtY_descr_in[RSDecoder_OmegaZActual_idx_0] = ((uint8_T)
      rtDW->IntegertoBitConverter[RSDecoder_OmegaZActual_idx_0] != 0);
  }

  /* S-Function (scomscram2): '<S3>/Descrambler' incorporates:
   *  DataTypeConversion: '<S6>/Conversion'
   */
  memset(&shiftReg[0], 0, sizeof(int32_T) << 4U);
  for (j = 0; j < 7200; j++) {
    inv = rtY_descr_in[j];
    bitIdx = inv;
    for (RSDecoder_OmegaZActual_idx_0 = 0; RSDecoder_OmegaZActual_idx_0 < 16;
         RSDecoder_OmegaZActual_idx_0++) {
      bitIdx -= (uint8_T)
        (rtConstP.Descrambler_Polynomial[RSDecoder_OmegaZActual_idx_0 + 1] *
         shiftReg[RSDecoder_OmegaZActual_idx_0]);
    }

    while (bitIdx < 0) {
      bitIdx += 2;
    }

    rtY_data_frame[j] = (bitIdx % 2 != 0);
    for (RSDecoder_OmegaZActual_idx_0 = 14; RSDecoder_OmegaZActual_idx_0 >= 0;
         RSDecoder_OmegaZActual_idx_0--) {
      shiftReg[RSDecoder_OmegaZActual_idx_0 + 1] =
        shiftReg[RSDecoder_OmegaZActual_idx_0];
    }

    shiftReg[0U] = inv;
  }

  /* End of S-Function (scomscram2): '<S3>/Descrambler' */

  /* Outport: '<Root>/dec_in' incorporates:
   *  Inport: '<Root>/rx_frame'
   */
  memcpy(&rtY_dec_in[0], &rtU_v2x_rx_demod_out[128], 16800U * sizeof(boolean_T));
}

/* Model initialize function */
void V2X_RX_Baseband_initialize(RT_MODEL *const rtM, boolean_T
  rtU_v2x_rx_demod_out[16928], boolean_T rtY_data_frame[7200], boolean_T
  rtY_dec_in[16800], boolean_T rtY_descr_in[7200])
{
  DW *rtDW = rtM->dwork;

  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)rtDW, 0,
                sizeof(DW));

  /* external inputs */
  (void)memset(&rtU_v2x_rx_demod_out[0], 0, 16928U * sizeof(boolean_T));

  /* external outputs */
  (void)memset(&rtY_data_frame[0], 0, 7200U * sizeof(boolean_T));
  (void)memset(&rtY_dec_in[0], 0, 16800U * sizeof(boolean_T));
  (void)memset(&rtY_descr_in[0], 0, 7200U * sizeof(boolean_T));
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
