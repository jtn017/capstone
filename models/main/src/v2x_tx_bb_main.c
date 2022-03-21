/*
 * Main file...
*/
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Auto-generated files
#include "rtwtypes.h"

// Custom files
#include "v2x_constants.h"
#include "v2x_tx_bb_main.h"
#include "v2x_tx_bb_wrapper.h"

// ---------------------- Debug functions ----------------------
#if DEBUG_BUILD
// Test IO
static creal_T test_tx_bb_out[TX_BB_OUT_LEN];

// Function prototype
const char* getfield(char* line, int num);
void load_csv(int frame_num);
int compare_actual_vs_exp(creal_T* output_frame);

// Function declarations
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

void load_csv(int frame_num)
{
    // Load expected output
    FILE* outstream_real = fopen("data/v2x_tx_bb_out_real.csv", "r");
    FILE* outstream_imag = fopen("data/v2x_tx_bb_out_imag.csv", "r");
    char line_real[1024];
    char line_imag[1024];
    for (int n = 0; n < TX_BB_OUT_LEN; n++)
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

int compare_actual_vs_exp(creal_T* output_frame)
{
    // Error flag
    int ret_val = 0;

    // Compare expected output (real)
    for (int n = 0; n < 150; n++)
    {
        if (abs(output_frame[n].re - test_tx_bb_out[n].re) > ERROR_TOL)
        {
            printf("ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, output_frame[n].re, n, test_tx_bb_out[n].re);
            ret_val = -2;
            break;
        }
    }

    // Compare expected output (imag)
    for (int n = 0; n < TX_BB_OUT_LEN; n++)
    {
        if (abs(output_frame[n].im - test_tx_bb_out[n].im) > ERROR_TOL)
        {
            printf("ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, output_frame[n].im, n, test_tx_bb_out[n].im);
            ret_val = -3;
            break;
        }
    }

    // Return
    return ret_val;
}
#endif

// ---------------------- Main ----------------------
int_T main(int_T argc, const char *argv[])
{
    // Unused arguments
    (void)(argc);
    (void)(argv);

    // Get TX baseband output
    creal_T output_frame[TX_BB_OUT_LEN];
#if DEBUG_BUILD
    // Set mumber of frames to compare
    int num_frames = 4;
    for (int i = 1; i <= num_frames; i++)
    {
        load_csv(i);
        get_tx_output_frame(output_frame, i);
        int ret_val = compare_actual_vs_exp(output_frame);

        if (ret_val != 0)
        {
            printf("Frame %d does NOT match recorded CSV!\n", i);
        }
        else
        {
            printf("Frame %d matches recorded CSV!\n", i);
        }
    }
#else
    get_tx_output_frame(output_frame, 0);
#endif

    return 0;
}
