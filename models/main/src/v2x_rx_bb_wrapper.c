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
#include "V2X_RX_Baseband.h"

// Custom files
#include "v2x_rx_bb_wrapper.h"
#include "v2x_constants.h"

// ---------------------- V2X_RX_Baseband Model ----------------------
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */

// ---------------------- V2X_RX_Baseband IO ----------------------
static boolean_T rtU_v2x_rx_bb_in[RX_BB_IN_LEN];
static boolean_T rtY_data_frame[RX_BB_OUT_LEN];
static boolean_T rtY_dec_in[RX_BB_DEC_LEN];
static boolean_T rtY_descr_in[RX_BB_IN_LEN];

// ---------------------- Function prototype ----------------------
void v2x_rx_bb_one_step(RT_MODEL *const rtM);
int get_rx_input_frame(int frame_num);
int get_data_from_adc();

// ---------------------- Temporary functions ----------------------
int get_adc_packet(bool adc_packet[RX_BB_IN_LEN])
{
    return 0;
}

// ---------------------- Internal functions ----------------------
int get_rx_input_frame(int frame_num)
{
#if DEBUG_BUILD
    // Grab data from file
    boolean_T buffer[RX_BB_IN_LEN * NUM_FRAMES];
    FILE * bin_file = fopen("data/v2x_rx_bb_in.bin", "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);
    fclose(bin_file);

    // Save to global
    for(unsigned int i = 0; i < RX_BB_IN_LEN; i++)
    {
        rtU_v2x_rx_bb_in[i] = buffer[RX_BB_IN_LEN * frame_num + i];
    }
#else
    bool adc_packet[RX_BB_IN_LEN];
    get_adc_packet(adc_packet);
    memcpy(rtU_v2x_rx_bb_in, adc_packet, RX_BB_IN_LEN*sizeof(rtU_v2x_rx_bb_in[0]));
#endif

    // No errors
    return 0;
}

void v2x_rx_bb_one_step(RT_MODEL *const rtM)
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
    V2X_RX_Baseband_step(rtM, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in);

    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}

// ---------------------- External functions ----------------------
int get_rx_bb_out_frame(boolean_T* output_frame, int frame_num)
{
    // Pack model data into RTM
    RT_MODEL *const rtM = rtMPtr;
    rtM->dwork = &rtDW;

    // Initialize and run model
    V2X_RX_Baseband_initialize(rtM, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in);

    get_rx_input_frame(frame_num);
    v2x_rx_bb_one_step(rtM);

    // Save output to external value
    memcpy(output_frame, rtY_data_frame, RX_BB_OUT_LEN*sizeof(rtY_data_frame[0]));

    return 0;
}

