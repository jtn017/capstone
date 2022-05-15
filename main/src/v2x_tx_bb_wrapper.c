/*
 * Wrapper function
*/
// Standard libraries
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Auto-generated files
#include "V2X_TX_Baseband.h"

// Custom files
#include "v2x_tx_bb_wrapper.h"
#include "v2x_constants.h"
#include "v2x_datatypes.h"

// ---------------------- V2X_TX_Baseband Model ----------------------
static RT_MODEL rtM_;
static RT_MODEL *const rtMPtr = &rtM_; /* Real-time model */

// ---------------------- V2X_TX_Baseband IO ----------------------
static uint8_T rtU_v2x_tx_bb_in[TX_BB_IN_BYTES];
static boolean_T rtY_tx_frame[TX_BB_OUT_BITS];
static boolean_T rtY_bits_in[TX_BB_IN_BITS];
static boolean_T rtY_scramb_out[TX_BB_IN_BITS];
static boolean_T rtY_enc_out[TX_BB_OUT_BITS];

// ---------------------- Global variables ----------------------
struct payload_struct g_info_pkt;
static uint8_t g_audio_pkt[AUDIO_PKTS][AUDIO_PKT_BYTES];
static uint16_t g_audio_pkt_num = 0;

// ---------------------- Function prototype ----------------------
static void v2x_tx_bb_one_step(RT_MODEL *const rtM);
static void update_info_packet(void);
static void init_audio_buffer(void);
static void advance_audio_buffer(void);
static void get_tx_input_frame();

// ---------------------- Data retrieval functions ----------------------
static void update_info_packet(void)
{
    // struct char_strings strs;
    unsigned int payload_values = PAYLOAD_VALUES;
    unsigned int payload_str_max = PAYLOAD_STR_MAX;

    char strs[payload_values][payload_str_max];

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    size_t read;

    // Read initial set of parameters from Config.txt file
    fp = fopen("data/Config.txt", "r");

    // Exit program if file doesn't exist
    if (fp == NULL)
    {
        printf("Error, data/Config.txt not found, exiting!\n");
        exit(EXIT_FAILURE);
    }

    // Keep a 'tally' of how many lines we have read.
    // Make sure we read up to 6 lines
    unsigned int i = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        
        if(i==6){
            printf("Error, program only supports up to 6 lines in Config.txt file!\n");
            // Close file descriptor and do house cleaning of line pointer
            fclose(fp);
            if (line)
                free(line);
            exit(EXIT_FAILURE);
        }
        // Be safe and reset each buffer before doing a memcpy
        memset(strs[i], 0, read);
        // Read -2 characters to compensate from '\r\n' at the end of each line
        memcpy(strs[i], line, read-2);
        i++;
    }

    // Close file descriptor and do house cleaning of line pointer
    fclose(fp);
    if (line)
        free(line);

    // Make sure we have read exaclty 6 lines, otherwise send an error
    if(i!=6){
        printf("Error, program did not read enough values from Config.txt file!\n");
        exit(EXIT_FAILURE);
    }

    // Grab all string values and generate first payload
    memcpy(g_info_pkt.name, strs[0], 4); // clip name to only the first 4 characters
    g_info_pkt.lat            = atof(strs[1]);
    g_info_pkt.lon            = atof(strs[2]);
    g_info_pkt.speed          = atoi(strs[3]);
    g_info_pkt.dir            = atoi(strs[4]);
    g_info_pkt.dist_next_step = atof(strs[5]);

    return;
}

static void init_audio_buffer(void)
{
    // Grab data from file
    uint8_t buffer[AUDIO_PKTS * AUDIO_PKT_BYTES];
    FILE * bin_file = fopen("data/sample_audio.bin", "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);
    fclose(bin_file);

    // Copy into audio buffer
    for (int i = 0; i < AUDIO_PKTS; i++)
    {
        for (int j = 0; j < AUDIO_PKT_BYTES; j++)
        {
            g_audio_pkt[i][j] = buffer[i*AUDIO_PKT_BYTES + j];
        }
    }
}

static void advance_audio_buffer(void)
{
    g_audio_pkt_num = (g_audio_pkt_num + 1) % AUDIO_PKTS;
}

// ---------------------- Internal functions ----------------------
static void get_tx_input_frame()
{
    // Update info packet
    update_info_packet();

    // Set data
    memcpy(rtU_v2x_tx_bb_in, (char*) &g_info_pkt, INFO_PKT_BYTES*sizeof(rtU_v2x_tx_bb_in[0]));
    memcpy(rtU_v2x_tx_bb_in + INFO_PKT_BYTES, g_audio_pkt[g_audio_pkt_num],
           AUDIO_PKT_BYTES*sizeof(rtU_v2x_tx_bb_in[0]));

    // Advance audio packet number
    advance_audio_buffer();
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
    V2X_TX_Baseband_step(rtM, rtU_v2x_tx_bb_in, rtY_tx_frame, rtY_bits_in, rtY_scramb_out, rtY_enc_out);

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
    // Initialize info and audio
    update_info_packet();
    init_audio_buffer();

    // Initialized baseband code
    V2X_TX_Baseband_initialize(rtMPtr, rtU_v2x_tx_bb_in, rtY_tx_frame, rtY_bits_in, rtY_scramb_out, rtY_enc_out);
}

void get_tx_bb_out(boolean_T* output_frame)
{
    get_tx_input_frame();
    v2x_tx_bb_one_step(rtMPtr);
    memcpy(output_frame, rtY_tx_frame, TX_BB_OUT_BITS*sizeof(rtY_tx_frame[0]));
}
