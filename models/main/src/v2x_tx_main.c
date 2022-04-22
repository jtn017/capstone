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
#include "v2x_tx_bb_wrapper.h"
#include "v2x_tx_mod_wrapper.h"
#include "v2x_rx_bb_wrapper.h"

// ---------------------- Global variables ----------------------
boolean_T g_tx_bb_out[TX_BB_OUT_BITS];
cint16_T g_tx_mod_out[TX_MOD_OUT_SYMS];
uint8_T g_rx_bb_out[RX_BB_OUT_BYTES];
unsigned int g_num_tx_frames;

// ---------------------- Function prototypes ----------------------
void read_char_from_bin(const char* filename, char* data, unsigned int filesize);
void read_float_from_bin(const char* filename, float* data, unsigned int filesize);

double fixed_to_double_txmod(int16_T x);
int16_T double_to_fixed_txmod(double x);
#if DEBUG_BUILD
static boolean_T test_tx_bb_out[TX_BB_OUT_BITS];
static creal_T test_tx_mod_out[TX_MOD_OUT_SYMS];
static boolean_T test_rx_bb_out[RX_BB_OUT_BYTES];

void load_bin(int frame_num);
int compare_actual_vs_exp(boolean_T* tx_bb_out, cint16_T* tx_mod_out, uint8_T* rx_bb_out);
#endif

// ---------------------- Debug ----------------------
#if DEBUG_BUILD
void load_bin(int frame_num)
{
    // TX Baseband expected output
    boolean_T tx_bb_out[TX_BB_OUT_BITS * NUM_FRAMES];
    read_char_from_bin("data/v2x_tx_bb_out.bin", (char*) tx_bb_out, TX_BB_OUT_BITS * NUM_FRAMES);
    for(unsigned int i = 0; i < TX_BB_OUT_BITS; i++)
    {
        test_tx_bb_out[i] = tx_bb_out[TX_BB_OUT_BITS * frame_num + i];
    }

    // TX Modulator expected output
    float tx_mod_out_real[TX_MOD_OUT_SYMS * NUM_FRAMES];
    float tx_mod_out_imag[TX_MOD_OUT_SYMS * NUM_FRAMES];
    read_float_from_bin("data/v2x_tx_mod_out_real.bin", tx_mod_out_real, TX_MOD_OUT_SYMS * NUM_FRAMES);
    read_float_from_bin("data/v2x_tx_mod_out_imag.bin", tx_mod_out_imag, TX_MOD_OUT_SYMS * NUM_FRAMES);
    for(unsigned int i = 0; i < TX_MOD_OUT_SYMS; i++)
    {
        
        test_tx_mod_out[i].re = (real_T) tx_mod_out_real[TX_MOD_OUT_SYMS * frame_num + i];
        test_tx_mod_out[i].im = (real_T) tx_mod_out_imag[TX_MOD_OUT_SYMS * frame_num + i];
    }

    // RX Modulator expected output
    uint8_T rx_bb_out[RX_BB_OUT_BYTES * NUM_FRAMES];
    read_char_from_bin("data/v2x_rx_bb_out.bin", (char*) rx_bb_out, RX_BB_OUT_BYTES * NUM_FRAMES);
    for(unsigned int i = 0; i < RX_BB_OUT_BYTES; i++)
    {
        test_rx_bb_out[i] = rx_bb_out[RX_BB_OUT_BYTES * frame_num + i];
    }

    // Return
    return;
}

int compare_actual_vs_exp(boolean_T* tx_bb_out, cint16_T* tx_mod_out, uint8_T* rx_bb_out)
{
    // Error flag
    int ret_val = 0;

    // TX Baseband
    for (unsigned int n = 0; n < TX_BB_OUT_BITS; n++)
    {
        if (tx_bb_out[n] != test_tx_bb_out[n])
        {
            printf("TX BB ERROR: actual[%d]: %d, expected[%d]: %d\n",
                  n, tx_bb_out[n], n, test_tx_bb_out[n]);
            ret_val = -1;
            break;
        }
    }

    // TX Modulator
    for (unsigned int n = 0; n < TX_MOD_OUT_SYMS; n++)
    {
        // Convert fixed point to float
        double temp_real = fixed_to_double_txmod(tx_mod_out[n].re);
        double temp_imag = fixed_to_double_txmod(tx_mod_out[n].im);

        // Compare real and imag
        if (abs(temp_real - test_tx_mod_out[n].re) > ERROR_TOL)
        {
            printf("TX MOD ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, temp_real, n, test_tx_mod_out[n].re);
            ret_val = -3;
            break;
        }
        if (abs(temp_imag - test_tx_mod_out[n].im) > ERROR_TOL)
        {
            printf("TX MOD ERROR: actual[%d].re: %f, expected[%d].re: %f\n",
                  n, temp_imag, n, test_tx_mod_out[n].im);
            ret_val = -4;
            break;
        }
    }

    // RX Baseband
    for (unsigned int n = 0; n < RX_BB_OUT_BYTES; n++) 
    {
        if (rx_bb_out[n] != test_rx_bb_out[n])
        {
            printf("RX BB ERROR: actual[%d]: %d, expected[%d]: %d\n",
                  n, rx_bb_out[n], n, test_rx_bb_out[n]);
            ret_val = -5;
            break;
        }
    }

    // Return
    return ret_val;
}
#endif

// ---------------------- Read from binary file ----------------------
void read_char_from_bin(const char* filename, char* data, unsigned int filesize)
{
    // Reading char from file
    char buffer[BIN_FILE_MAX];
    FILE * bin_file = fopen(filename, "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);

    // Copy to passed array
    memcpy(data, buffer, filesize);

    // Close file
    fclose(bin_file);
}

void read_float_from_bin(const char* filename, float* data, unsigned int filesize)
{
    // Reading boolean_T (1 char size) from file
    float buffer[BIN_FILE_MAX];
    FILE * bin_file = fopen(filename, "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);

    // Copy to passed array
    memcpy(data, buffer, filesize);

    // Close file
    fclose(bin_file);
}

// ---------------------- Fixed point conversion ----------------------
// https://embeddedartistry.com/blog/2018/07/12/simple-fixed-point-conversion-in-c/
double fixed_to_double_txmod(int16_T x)
{
    return ((double) x / (double) (1 << MOD_FRACT_BITS));
}

int16_T double_to_fixed_txmod(double x)
{
    return (int16_T) (round(x * (1 << MOD_FRACT_BITS)));
}

// ---------------------- Main ----------------------
// https://stackoverflow.com/questions/13585284/libevent-make-timer-persistent
void get_tx_frame(int fd, short event, void *arg)
{
    // printf("Hello\n");
    get_tx_bb_out(g_tx_bb_out, 0);
    get_tx_mod_out(g_tx_bb_out, g_tx_mod_out);
    printf("Frame: %d\n", g_num_tx_frames);
    g_num_tx_frames++;
}

int_T main(int_T argc, const char *argv[])
{
    // Unused arguments
    (void)(argc);
    (void)(argv);

    struct payload_struct pyld;

    // Initialize generated code
    tx_bb_init();
    tx_mod_init();
    rx_bb_init();

    // Get TX baseband output
#if DEBUG_BUILD
    // Set mumber of frames to compare
    for (int i = 0; i < NUM_FRAMES; i++)
    {
        load_bin(i);
        get_tx_bb_out(g_tx_bb_out, i);
        get_tx_mod_out(g_tx_bb_out, g_tx_mod_out);
        get_rx_bb_out(g_rx_bb_out, i);
        int ret_val = compare_actual_vs_exp(g_tx_bb_out, g_tx_mod_out, g_rx_bb_out);

        if (ret_val != 0)
        {
            printf("Frame %d does NOT match recorded CSV!\n", i);
        }
        else
        {
            printf("Frame %d matches recorded CSV!\n", i);
            if (i != 0) {
                parse_payload_packet(g_rx_bb_out, &pyld);
                // tx_payload_wifimodule(&pyld, &fd);
                // tx_payload_wifimodule(&pyld, fds);
                // tx_payload_wifimodule(&pyld);
#ifdef HTTP_SOCKET
                tx_payload_wifimodule2(&pyld);
#endif
            }
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
