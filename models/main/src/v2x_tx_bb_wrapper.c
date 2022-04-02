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
static boolean_T rtU_v2x_tx_bb_in[TX_BB_IN_LEN];
static boolean_T rtY_tx_frame[TX_BB_OUT_LEN];
static boolean_T rtY_scramb_out[TX_BB_IN_LEN];
static boolean_T rtY_enc_out[TX_BB_ENC_LEN];

// ---------------------- Function prototype ----------------------
void v2x_tx_bb_one_step(RT_MODEL *const rtM);
int get_tx_input_frame(int frame_num);
int get_info_packet(bool info_packet[INFO_PKT_LEN]);
int get_audio_packet(bool audio_packet[INFO_PKT_LEN]);

// ---------------------- Temporary functions ----------------------
int get_info_packet(bool info_packet[INFO_PKT_LEN])
{
    return 0;
}

int get_audio_packet(bool audio_packet[INFO_PKT_LEN])
{
    return 0;
}

// ---------------------- Debug functions ----------------------
#if DEBUG_BUILD
// Function prototype
const char* getfield_wrap(char* line, int num);

// Function declarations
const char* getfield_wrap(char* line, int num)
{
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}
#endif

// ---------------------- Internal functions ----------------------
int get_tx_input_frame(int frame_num)
{
#if DEBUG_BUILD
    // Load input
    FILE* instream = fopen("data/v2x_tx_bb_in.csv", "r");
    char line[1024];
    for (int n = 0; n < TX_BB_IN_LEN; n++)
    {
        fgets(line, sizeof(line), instream);
        char* tmp = strdup(line);
        rtU_v2x_tx_bb_in[n] = (boolean_T) atoi(getfield_wrap(tmp, frame_num));
        free(tmp);
    }
    fclose(instream);

    // for (int n = 0; n < 150; n++)
    // {
    //     printf("rtU_v2x_tx_bb_in[%d]: %d\n", n, rtU_v2x_tx_bb_in[n]);
    // }
#else
    // Variable declarations
    bool info_packet[INFO_PKT_LEN];
    bool audio_packet[AUDIO_PKT_LEN];

    // Get data
    get_info_packet(info_packet);
    get_audio_packet(audio_packet);

    // Set data
    memcpy(rtU_v2x_tx_bb_in, info_packet, INFO_PKT_LEN*sizeof(rtU_v2x_tx_bb_in[0]));
    memcpy(rtU_v2x_tx_bb_in, audio_packet, AUDIO_PKT_LEN*sizeof(rtU_v2x_tx_bb_in[0]));
#endif

    // No errors
    return 0;
}

void v2x_tx_bb_one_step(RT_MODEL *const rtM)
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
int get_tx_bb_out_frame(boolean_T* output_frame, int frame_num)
{
    // Pack model data into RTM
    RT_MODEL *const rtM = rtMPtr;
    rtM->dwork = &rtDW;

    // Initialize and run model
    V2X_TX_Baseband_initialize(rtM, rtU_v2x_tx_bb_in, rtY_tx_frame, rtY_scramb_out, rtY_enc_out);

    get_tx_input_frame(frame_num);
    v2x_tx_bb_one_step(rtM);

    // Save output to external value
    memcpy(output_frame, rtY_tx_frame, TX_BB_OUT_LEN*sizeof(rtY_tx_frame[0]));

    return 0;
}

