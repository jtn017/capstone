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
static boolean_T rtU_v2x_tx_bb_out[TX_BB_OUT_BITS];
static cint16_T rtY_mod_frame[TX_MOD_OUT_SYMS];
static creal_T rtY_map_out[TX_MOD_MAP_SYMS];
static creal_T rtY_ps_out[TX_MOD_OUT_SYMS];

// ---------------------- Function prototype ----------------------
static void v2x_tx_mod_one_step(RT_MODEL *const rtM);

// ---------------------- Internal functions ----------------------
static void v2x_tx_mod_one_step(RT_MODEL *const rtM)
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
void tx_mod_init(void)
{
    rtMPtr->dwork = &rtDW;
    V2X_TX_Modulator_initialize(rtMPtr, rtU_v2x_tx_bb_out, rtY_mod_frame, rtY_map_out, rtY_ps_out);
}

void get_tx_mod_out(boolean_T* input, cint16_T* output)
{
    tx_mod_init();
    memcpy(rtU_v2x_tx_bb_out, input, TX_BB_OUT_BITS*sizeof(rtU_v2x_tx_bb_out[0]));
    v2x_tx_mod_one_step(rtMPtr);
    memcpy(output, rtY_mod_frame, TX_MOD_OUT_SYMS*sizeof(rtY_mod_frame[0]));
}
