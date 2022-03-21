/*
 * Adapted from ert_main.c
*/
#include <stddef.h>
#include <stdio.h>
#include "V2X_TX_Baseband.h"
#include "v2x_tx_bb_main.h"

// Other libraries...
#include <stdbool.h>
#include <stdlib.h>

// Size defines
#define TX_BB_IN_SZ 7200
#define TX_BB_OUT_SZ 8464

// Error tolerance
#define ERROR_TOL 0.001

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

// Test IO
static boolean_T test_tx_bb_in[TX_BB_IN_SZ];
static creal_T test_tx_bb_out[TX_BB_OUT_SZ];

// Function declarations
void rt_OneStep(RT_MODEL *const rtM);
const char* getfield(char* line, int num);
void loadCSV(int frame_num);

// ---------------------- Helper functions ----------------------
// Wrapper
void rt_OneStep(RT_MODEL *const rtM)
{
  /* Step the model */
  V2X_TX_Baseband_step(rtM, rtU_data_frame, rtY_tx_frame, rtY_tx_in,
                       rtY_scrambler_out, rtY_encoder_out, rtY_mapper_out,
                       rtY_preamble_out);
}

// CSV reader (bare bones)
const char* getfield(char* line, int num)
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

// Load CSV (input and expected output)
void loadCSV(int frame_num)
{
    // Load input
    FILE* instream = fopen("data/v2x_tx_bb_in.csv", "r");
    char line[1024];
    for (int n = 0; n < TX_BB_IN_SZ; n++)
    {
        fgets(line, sizeof(line), instream);
        char* tmp = strdup(line);
        rtU_data_frame[n] = (boolean_T) atoi(getfield(tmp, frame_num));
        test_tx_bb_in[n] = rtU_data_frame[n];
        free(tmp);
    }
    fclose(instream);

    // Load expected output
    FILE* outstream_real = fopen("data/v2x_tx_bb_out_real.csv", "r");
    FILE* outstream_imag = fopen("data/v2x_tx_bb_out_imag.csv", "r");
    char line_real[1024];
    char line_imag[1024];
    for (int n = 0; n < TX_BB_OUT_SZ; n++)
    {
        // Real
        fgets(line_real, sizeof(line_real), outstream_real);
        char* tmp_real = strdup(line_real);
        test_tx_bb_out[n].re = (real_T) strtof(getfield(tmp_real, frame_num), NULL);
        free(tmp_real);

        // Imag
        fgets(line_imag, sizeof(line_imag), outstream_imag);
        char* tmp_imag = strdup(line_imag);
        test_tx_bb_out[n].im = (real_T) strtof(getfield(tmp_imag, frame_num), NULL);
        free(tmp_imag);
    }
    fclose(outstream_real);
    fclose(outstream_imag);

    // Return
    return;
}

int compareOut(void)
{
    // Error flag
    int ret_val = 0;

    // Compare input
    for (int n = 0; n < TX_BB_IN_SZ; n++)
    {
        if (rtY_tx_in[n] != test_tx_bb_in[n])
        {
            printf("ERROR: rtU_data_frame[%d]: %d, test_tx_bb_in[%d]: %d\n",
                    n, rtY_tx_in[n], n, test_tx_bb_in[n]);
            ret_val = -1;
            break;
        }
    }

    // Compare expected output (real)
    for (int n = 0; n < TX_BB_OUT_SZ; n++)
    {
        if (abs(rtY_tx_frame[n].re - test_tx_bb_out[n].re) > ERROR_TOL)
        {
            printf("ERROR: rtY_tx_frame[%d].re: %f, test_tx_bb_out[%d].re: %f\n",
                    n, rtY_tx_frame[n].re, n, test_tx_bb_out[n].re);
            ret_val = -2;
            break;
        }
    }

    // Compare expected output (imag)
    for (int n = 0; n < TX_BB_OUT_SZ; n++)
    {
        if (abs(rtY_tx_frame[n].im - test_tx_bb_out[n].im) > ERROR_TOL)
        {
            printf("ERROR! Value at line %d, ending comparison\n", n);
            ret_val = -3;
            break;
        }
    }

    // Return
    return ret_val;
}


// ---------------------- Main ----------------------
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

    // Set mumber of frames to compare
    int num_frames = 4;
    for (int i = 1; i <= num_frames; i++)
    {
        loadCSV(i);
        rt_OneStep(rtM);
        int ret_val = compareOut();

        if (ret_val != 0)
        {
            printf("Frame %d does NOT match recorded CSV!\n", i);
        }
        else
        {
            printf("Frame %d matches recorded CSV!\n", i);
        }
    }

    return 0;
}

