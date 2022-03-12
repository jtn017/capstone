/*
 * Adapted from ert_main.c
*/
#include <stddef.h>
#include <stdio.h>            /* This example main program uses printf/fflush */
#include "V2X_TX_Baseband.h"           /* Model's header file */
#include "v2x_tx_bb_main.h"

// Included until we figure out the makefile linking
#include "V2X_TX_Baseband.c"
#include "V2X_TX_Baseband_data.c" 

static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */

// V2X_TX_Baseband IO
static boolean_T rtU_data_frame[7200]; /* '<Root>/data_frame' */
static creal_T rtY_tx_frame[8464]; /* '<Root>/tx_frame' */
static boolean_T rtY_tx_in[7200]; /* '<Root>/tx_in' */
static boolean_T rtY_scrambler_out[7200]; /* '<Root>/scrambler_out' */
static boolean_T rtY_encoder_out[16800]; /* '<Root>/encoder_out' */
static creal_T rtY_mapper_out[8400]; /* '<Root>/mapper_out' */
static creal_T rtY_preamble_out[8464]; /* '<Root>/preamble_out' */

/*
 * Associating rt_OneStep with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function rt_OneStep is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from rt_OneStep.
 */
void rt_OneStep(RT_MODEL *const rtM);
void rt_OneStep(RT_MODEL *const rtM)
{
  static boolean_T OverrunFlag = false;

  /* Disable interrupts here */

  /* Check for overrun */
  if (OverrunFlag) {
    return;
  }

  OverrunFlag = true;

  /* Save FPU context here (if necessary) */
  /* Re-enable timer or interrupt here */
  /* Set model inputs here */

  /* Step the model */
  V2X_TX_Baseband_step(rtM, rtU_data_frame, rtY_tx_frame, rtY_tx_in,
                       rtY_scrambler_out, rtY_encoder_out, rtY_mapper_out,
                       rtY_preamble_out);

  /* Get model outputs here */

  /* Indicate task complete */
  OverrunFlag = false;

  /* Disable interrupts here */
  /* Restore FPU context here (if necessary) */
  /* Enable interrupts here */
}

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching rt_OneStep to a real-time clock is target specific.  This example
 * illustrates how you do this relative to initializing the model.
 */
int_T main(int_T argc, const char *argv[])
{
  RT_MODEL *const rtM = rtMPtr;

  /* Unused arguments */
  (void)(argc);
  (void)(argv);

  /* Pack model data into RTM */
  rtM->dwork = &rtDW;

  /* Initialize model */
  V2X_TX_Baseband_initialize(rtM, rtU_data_frame, rtY_tx_frame, rtY_tx_in,
    rtY_scrambler_out, rtY_encoder_out, rtY_mapper_out, rtY_preamble_out);

  /* Attach rt_OneStep to a timer or interrupt service routine with
   * period 0.01 seconds (the model's base sample time) here.  The
   * call syntax for rt_OneStep is
   *
   *  rt_OneStep(rtM);
   */
  // TODO: Re-enable? or repurpose at a later time
  // printf("Warning: The simulation will run forever. "
  //        "Generated ERT main won't simulate model step behavior. "
  //        "To change this behavior select the 'MAT-file logging' option.\n");
  // fflush((NULL));
  // while (1) {
  //   /*  Perform application tasks here */
  // }
  rt_OneStep(rtM);

  /* The option 'Remove error status field in real-time model data structure'
   * is selected, therefore the following code does not need to execute.
   */
#if 0

  /* Disable rt_OneStep here */
#endif

  return 0;
}
