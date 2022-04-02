/*
 * Main file...
*/
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <event.h>
#include <unistd.h>

// Auto-generated files
#include "rtwtypes.h"

// Custom files
#include "v2x_constants.h"
#include "v2x_tx_main.h"
#include "v2x_tx_bb_wrapper.h"
#include "v2x_tx_mod_wrapper.h"

// ---------------------- Global variables ----------------------
boolean_T g_tx_bb_out_frame[TX_BB_OUT_LEN];
cint16_T g_tx_mod_out_frame[TX_MOD_OUT_LEN];
unsigned int g_num_tx_frames;

// ---------------------- Function prototypes ----------------------
double fixed_to_double_txmod(int16_T x);
int16_T double_to_fixed_txmod(double x);
#if DEBUG_BUILD
static boolean_T test_tx_bb_out[TX_BB_OUT_LEN];
static creal_T test_tx_mod_out[TX_MOD_OUT_LEN];

const char* getfield(char* line, int num);
void load_csv(int frame_num);
int compare_actual_vs_exp(boolean_T* tx_bb_out_frame, cint16_T* tx_mod_out_frame);
#endif

// ---------------------- Debug functions ----------------------
#if DEBUG_BUILD
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
    // Reused variables
    char line_real[1024];
    char line_imag[1024];

    // TX Baseband expected output
    FILE* tx_bb_out = fopen("data/v2x_tx_bb_out.csv", "r");
    for (int n = 0; n < TX_BB_OUT_LEN; n++)
    {
        // Real
        fgets(line_real, sizeof(line_real), tx_bb_out);
        char* tmp_real = strdup(line_real);
        test_tx_bb_out[n] = (real_T) strtof(getfield(tmp_real, frame_num), NULL);
        free(tmp_real);
    }
    fclose(tx_bb_out);

    // TX Modulator expected output
    FILE* tx_mod_out_real = fopen("data/v2x_tx_mod_out_real.csv", "r");
    FILE* tx_mod_out_imag = fopen("data/v2x_tx_mod_out_imag.csv", "r");
    for (int n = 0; n < TX_MOD_OUT_LEN; n++)
    {
        // Real
        fgets(line_real, sizeof(line_real), tx_mod_out_real);
        char* tmp_real = strdup(line_real);
        test_tx_mod_out[n].re = (real_T) strtof(getfield(tmp_real, frame_num), NULL);
        free(tmp_real);

        // Imag
        fgets(line_imag, sizeof(line_imag), tx_mod_out_imag);
        char* tmp_imag = strdup(line_imag);
        test_tx_mod_out[n].im = (real_T) strtof(getfield(tmp_imag, frame_num), NULL);
        free(tmp_imag);
    }
    fclose(tx_mod_out_real);
    fclose(tx_mod_out_imag);

    // Return
    return;
}

int compare_actual_vs_exp(boolean_T* tx_bb_out_frame, cint16_T* tx_mod_out_frame)
{
    // Error flag
    int ret_val = 0;

    // TX Baseband real
    for (int n = 0; n < TX_BB_OUT_LEN; n++)
    {
        if (tx_bb_out_frame[n] != test_tx_bb_out[n])
        {
            printf("TX BB ERROR: actual[%d]: %d, expected[%d]: %d\n",
                  n, tx_bb_out_frame[n], n, test_tx_bb_out[n]);
            ret_val = -1;
            break;
        }
    }

    // TX Modulator real
    for (int n = 0; n < TX_MOD_OUT_LEN; n++)
    {
        double temp = fixed_to_double_txmod(tx_mod_out_frame[n].re);
        if (abs(temp - test_tx_mod_out[n].re) > ERROR_TOL)
        {
            printf("TX MOD ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, temp, n, test_tx_mod_out[n].re);
            ret_val = -3;
            break;
        }
    }

    // TX Modulator imag
    for (int n = 0; n < TX_MOD_OUT_LEN; n++)
    {
        double temp = fixed_to_double_txmod(tx_mod_out_frame[n].re);
        if (abs(temp - test_tx_mod_out[n].im) > ERROR_TOL)
        {
            printf("TX MOD ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, temp, n, test_tx_mod_out[n].im);
            ret_val = -4;
            break;
        }
    }

    // Return
    return ret_val;
}
#endif

// ---------------------- Fixed point conversion functions ----------------------
// https://embeddedartistry.com/blog/2018/07/12/simple-fixed-point-conversion-in-c/
double fixed_to_double_txmod(int16_T x)
{
    return ((double) x / (double) (1 << TX_MOD_FRACT_BITS));
}

int16_T double_to_fixed_txmod(double x)
{
    return (int16_T) (round(x * (1 << TX_MOD_FRACT_BITS)));
}

// ---------------------- Main ----------------------
// https://stackoverflow.com/questions/13585284/libevent-make-timer-persistent
void get_tx_frame(int fd, short event, void *arg)
{
    // printf("Hello\n");
    get_tx_bb_out_frame(g_tx_bb_out_frame, 0);
    get_tx_mod_out_frame(g_tx_bb_out_frame, g_tx_mod_out_frame);
    printf("Frame: %d\n", g_num_tx_frames);
    g_num_tx_frames++;
}

int_T main(int_T argc, const char *argv[])
{
    // Unused arguments
    (void)(argc);
    (void)(argv);

    // Get TX baseband output
#if DEBUG_BUILD
    // Set mumber of frames to compare
    int num_frames = 4;
    for (int i = 1; i <= num_frames; i++)
    {
        load_csv(i);
        get_tx_bb_out_frame(g_tx_bb_out_frame, i);
        get_tx_mod_out_frame(g_tx_bb_out_frame, g_tx_mod_out_frame);
        int ret_val = compare_actual_vs_exp(g_tx_bb_out_frame, g_tx_mod_out_frame);

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
    // Event variables
    struct event ev;
    struct timeval tv;
    
    // Set even timer (periodic)
    tv.tv_sec = 0;
    tv.tv_usec = 500000; // 0.5 sec

    // Set event counter
    g_num_tx_frames = 0;

    // Run event
    event_init();
    event_set(&ev, 0, EV_PERSIST, get_tx_frame, NULL);
    evtimer_add(&ev, &tv);
    event_dispatch();
#endif

    return 0;
}
