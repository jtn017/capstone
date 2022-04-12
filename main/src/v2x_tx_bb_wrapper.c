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
#include "V2X_TX_Baseband.h"

// Custom files
#include "v2x_tx_bb_wrapper.h"
#include "v2x_constants.h"

// ---------------------- V2X_TX_Baseband Model ----------------------
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */
static DW rtDW;                        /* Observable states */

// ---------------------- V2X_TX_Baseband IO ----------------------
static uint8_T rtU_v2x_tx_bb_in[TX_BB_IN_BYTES];
static boolean_T rtY_tx_frame[TX_BB_OUT_BITS];
static boolean_T rtY_scramb_out[TX_BB_IN_BITS];
static boolean_T rtY_enc_out[TX_BB_OUT_BITS];

// ---------------------- Function prototype ----------------------
static void v2x_tx_bb_one_step(RT_MODEL *const rtM);
static void get_tx_input_frame(int frame_num);
static void get_info_packet(char info_packet[INFO_PKT_BYTES]);
static void get_audio_packet(char audio_packet[AUDIO_PKT_BYTES]);

// ---------------------- Temporary functions ----------------------
static void get_info_packet(char info_packet[INFO_PKT_BYTES])
{
    return;
}

static void get_audio_packet(char audio_packet[AUDIO_PKT_BYTES])
{
    return;
}

// ---------------------- Internal functions ----------------------
static void get_tx_input_frame(int frame_num)
{
#if DEBUG_BUILD
    // Grab data from file
    uint8_T buffer[TX_BB_IN_BYTES * NUM_FRAMES];
    FILE * bin_file = fopen("data/v2x_tx_bb_in.bin", "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);
    fclose(bin_file);

    // Save to global
    for(unsigned int i = 0; i < TX_BB_IN_BYTES; i++)
    {
        rtU_v2x_tx_bb_in[i] = buffer[TX_BB_IN_BYTES * frame_num + i];
    }
#else
    // Variable declarations
    char info_packet[INFO_PKT_BYTES];
    char audio_packet[AUDIO_PKT_BYTES];

    // Get data
    get_info_packet(info_packet);
    get_audio_packet(audio_packet);

    // Set data
    memcpy(rtU_v2x_tx_bb_in, info_packet, INFO_PKT_BYTES*sizeof(rtU_v2x_tx_bb_in[0]));
    memcpy(rtU_v2x_tx_bb_in + INFO_PKT_BYTES, audio_packet,
           AUDIO_PKT_BYTES*sizeof(rtU_v2x_tx_bb_in[0]));
#endif
}

static void v2x_tx_bb_one_step(RT_MODEL *const rtM)
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
    V2X_TX_Baseband_step(rtM, rtU_v2x_tx_bb_in, rtY_tx_frame, rtY_scramb_out, rtY_enc_out);

    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}

// ---------------------- External functions ----------------------
void tx_bb_init(void)
{
    rtMPtr->dwork = &rtDW;
    V2X_TX_Baseband_initialize(rtMPtr, rtU_v2x_tx_bb_in, rtY_tx_frame, rtY_scramb_out, rtY_enc_out);
}

void get_tx_bb_out(boolean_T* output_frame, int frame_num)
{
    get_tx_input_frame(frame_num);
    v2x_tx_bb_one_step(rtMPtr);
    memcpy(output_frame, rtY_tx_frame, TX_BB_OUT_BITS*sizeof(rtY_tx_frame[0]));
}
