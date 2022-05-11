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
static boolean_T rtU_v2x_rx_bb_in[RX_BB_IN_BITS];
static uint8_T rtY_data_frame[RX_BB_OUT_BYTES];
static boolean_T rtY_dec_in[RX_BB_DEC_BITS];
static boolean_T rtY_descr_in[TX_BB_IN_BITS];
static boolean_T rtY_bits_out[TX_BB_IN_BITS];

// ---------------------- Function prototype ----------------------
static void v2x_rx_bb_one_step(RT_MODEL *const rtM);
static void get_rx_input(int frame_num);
static void get_adc_packet(bool adc_packet[RX_BB_IN_BITS]);

// ---------------------- Data retrieval functions ----------------------
static void get_adc_packet(bool adc_packet[RX_BB_IN_BITS])
{
    return;
}

// ---------------------- Internal functions ----------------------
static void get_rx_input(int frame_num)
{
#if DEBUG_BUILD
    // Grab data from file
    boolean_T buffer[RX_BB_IN_BITS * NUM_FRAMES];
    FILE * bin_file = fopen("data/v2x_rx_bb_in.bin", "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);
    fclose(bin_file);

    // Save to global
    for(unsigned int i = 0; i < RX_BB_IN_BITS; i++)
    {
        rtU_v2x_rx_bb_in[i] = buffer[RX_BB_IN_BITS * frame_num + i];
    }
#else
    bool adc_packet[RX_BB_IN_BITS];
    get_adc_packet(adc_packet);
    memcpy(rtU_v2x_rx_bb_in, adc_packet, RX_BB_IN_BITS*sizeof(rtU_v2x_rx_bb_in[0]));
#endif
}

static void v2x_rx_bb_one_step(RT_MODEL *const rtM)
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
    V2X_RX_Baseband_step(rtM, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in, rtY_bits_out);

    /* Get model outputs here */

    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */
}

// ---------------------- External functions ----------------------
void rx_bb_init(void)
{
    rtMPtr->dwork = &rtDW;
    V2X_RX_Baseband_initialize(rtMPtr, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in, rtY_bits_out);
}

void get_rx_bb_out(uint8_T* output, int frame_num)
{
    get_rx_input(frame_num);
    v2x_rx_bb_one_step(rtMPtr);
    memcpy(output, rtY_data_frame, RX_BB_OUT_BYTES*sizeof(rtY_data_frame[0]));
}

#define EXPLICIT_INITIALIZATION

// ---------------------- External functions ----------------------
int fix_payload_packet(uint8_T* in_frame)
{
    struct payload_struct pyld;
    
    // Get number of bytes in info packet
    // unsigned int payload_size = INFO_PKT_BYTES;

    // Crete buffer and copy packets contents locally
    // uint8_T payload[payload_size];
    // memcpy(payload, in_frame, payload_size);

    // Reset memory of struct to 0
    memset(&pyld, 0, sizeof(pyld));
    float temp1, temp2, temp3;

    /* 
        Use implicit or explicit initialization.
        Note that shorter version requires using memcpy twice
        since struct is not aligned to 32-bits (just some C-weirdness).
    */
   
#ifndef EXPLICIT_INITIALIZATION 
    memcpy(&pyld->name, &payload[0], 4);
    memcpy(&pyld->lat, &payload[4], 4);
    memcpy(&pyld->lon, &payload[8], 4);
    pyld->speed = payload[12];
    pyld->dir = payload[13];
    memcpy(&pyld->dist_next_step, &payload[14], 4);
#else
    memcpy(&pyld, &in_frame[0], 14);
    memcpy(&pyld.dist_next_step, &in_frame[14], 4);
#endif

    temp1 = fix_endianness(pyld.lat);
    pyld.lat = temp1;

    temp2 = fix_endianness(pyld.lon);
    pyld.lon = temp2;

    temp3 = fix_endianness(pyld.dist_next_step);
    pyld.dist_next_step = temp3;

    memcpy(&in_frame[0], &pyld, 14);
    memcpy(&in_frame[14], &pyld.dist_next_step, 4);

    return 0;
}

float fix_endianness(float val)
{    
    // Bit masking with floating point is not supported by compiler...
    // Need to copy floating value to a uint32_t for masking
    unsigned int temp_val = 0;
    memcpy(&temp_val, &val, 4);

    float flt_val = 0.0f;
    
    //create mask of bits 1111
    unsigned int mask = 0xF;

    // Create array of 4 chars (4 bytes = 1 single pt value) to
    // rearrange bits and produce the correct floating pt value
    unsigned char swapped_bits[4];
    
    swapped_bits[3] = (((mask<<4) & temp_val) | ((mask<<0) & temp_val)) >> 0;
    
    swapped_bits[2] = (((mask<<12) & temp_val) | ((mask<<8) & temp_val)) >> 8;

    swapped_bits[1] = (((mask<<20) & temp_val) | ((mask<<16) & temp_val)) >> 16;

    swapped_bits[0] = (((mask<<28) & temp_val) | ((mask<<24) & temp_val)) >> 24;

    // copy 4 bytes into single pt value
    memcpy(&flt_val, swapped_bits, 4);

    // return correct floating pt value
    return flt_val;
}

