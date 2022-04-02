/*
 * Wrapper function
*/
// Standard libraries
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Auto-generated files
#include "V2X_TX_Modulator.h"

// Custom files
#include "v2x_tx_mod_wrapper.h"
#include "v2x_constants.h"

// ---------------------- V2X_TX_Modulator Model ----------------------
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */

// ---------------------- V2X_TX_Modulator IO ----------------------
static boolean_T rtU_v2x_tx_bb_out[TX_BB_OUT_LEN];
static cint16_T rtY_mod_frame[TX_MOD_OUT_LEN];
static creal_T rtY_map_out[TX_MOD_MAP_LEN];
static creal_T rtY_ps_out[TX_MOD_OUT_LEN];

// ---------------------- Function prototype ----------------------
void v2x_tx_mod_one_step(RT_MODEL *const rtM);

// ---------------------- Internal functions ----------------------
void v2x_tx_mod_one_step(RT_MODEL *const rtM)
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
    V2X_TX_Modulator_step(rtM, rtU_v2x_tx_bb_out, rtY_mod_frame, rtY_map_out, rtY_ps_out);

    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}

// ---------------------- External functions ----------------------
int get_tx_mod_out_frame(boolean_T* input_frame, cint16_T* output_frame)
{
    // Pack model data into RTM
    RT_MODEL *const rtM = rtMPtr;
    rtM->dwork = &rtDW;

    // Initialize and run model
    V2X_TX_Modulator_initialize(rtM, rtU_v2x_tx_bb_out, rtY_mod_frame, rtY_map_out, rtY_ps_out);
    memcpy(rtU_v2x_tx_bb_out, input_frame, TX_BB_OUT_LEN*sizeof(rtU_v2x_tx_bb_out[0]));
    v2x_tx_mod_one_step(rtM);

    // Save output to external value
    memcpy(output_frame, rtY_mod_frame, TX_MOD_OUT_LEN*sizeof(rtY_mod_frame[0]));

    return 0;
}

