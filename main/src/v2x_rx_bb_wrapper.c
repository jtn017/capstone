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
static void get_rx_input(uint32_t* uio_pkt);

// ---------------------- Internal functions ----------------------
static void get_rx_input(uint32_t* uio_pkt)
{
    uint32_t idx = 0;
    for (uint32_t i = 0; i < RX_BB_IN_DWORD; i++)
    {
        for (uint32_t j = 31; j >= 0; j--)
        {
            if (idx < RX_BB_IN_BITS)
            {
                rtU_v2x_rx_bb_in[idx] = ((uio_pkt[i] >> j) & 1);
            }
            else
            {
                break;
            }
            idx++;
        }
    }
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

void get_rx_bb_out(uint32_t* uio_pkt, uint8_T* output_frame)
{
    get_rx_input(uio_pkt);
    v2x_rx_bb_one_step(rtMPtr);
    memcpy(output_frame, rtY_data_frame, RX_BB_OUT_BYTES*sizeof(rtY_data_frame[0]));
}

#define MAX 20
#define N_FLOAT_DIGITS 6
#define EXPLICIT_INITIALIZATION

// ---------------------- External functions ----------------------
int parse_payload_packet(uint8_T* in_frame, struct payload_struct * pyld)
{
    // Get number of bytes in info packet
    unsigned int payload_size = INFO_PKT_BYTES;

    // Crete buffer and copy packets contents locally
    uint8_T payload[payload_size];
    memcpy(payload, in_frame, payload_size);

    // Reset memory of struct to 0
    memset(pyld, 0, sizeof(*pyld));

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
    memcpy(pyld, &payload[0], 14);
    memcpy(&pyld->dist_next_step, &payload[14], 4);
#endif

    float temp = fix_endianness(pyld->lat);
    pyld->lat = temp;

    temp = fix_endianness(pyld->lon);
    pyld->lon = temp;

    temp = fix_endianness(pyld->dist_next_step);
    pyld->dist_next_step = temp;

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

void tx_payload_wifimodule2(struct payload_struct * pyld){
  
    char ip_addr[] = "192.168.1.16";

    char str1[200];
    char str2[200];
    char str3[200];
    char str4[200];
    char str5[200];
    char str6[200];

    // Reset char string structure to 0
    struct char_strings strs;
    memset(&strs, 0, sizeof(strs));

    // Copy every member of payload into string arrays
    memcpy(&strs.name, pyld->name, 4);
    gcvt(pyld->lat, N_FLOAT_DIGITS, strs.lat);
    gcvt(pyld->lon, N_FLOAT_DIGITS, strs.lon);
    snprintf( strs.speed, N_FLOAT_DIGITS, "%d", pyld->speed );
    snprintf( strs.dir, N_FLOAT_DIGITS, "%d", pyld->dir );
    gcvt(pyld->dist_next_step, N_FLOAT_DIGITS, strs.dist_next_step);

    /*
    printf("%s\n",strs.name);
    printf("%s\n",strs.lat);
    printf("%s\n",strs.lon);
    printf("%s\n",strs.speed);
    printf("%s\n",strs.dir);
    printf("%s\n",strs.dist_next_step);
    */

    // Create HTTP GET requests(URL path and query)
    sprintf(str1, "%s/name?value=%s",           ip_addr, strs.name);
    sprintf(str2, "%s/lat?value=%s",            ip_addr, strs.lat);
    sprintf(str3, "%s/lon?value=%s",            ip_addr, strs.lon);
    sprintf(str4, "%s/speed?value=%s",          ip_addr, strs.speed);
    sprintf(str5, "%s/dir?value=%s",            ip_addr, strs.dir);
    sprintf(str6, "%s/dist_next_step?value=%s", ip_addr, strs.dist_next_step);

    /*
    printf("%s\n",str1);
    printf("%s\n",str2);
    printf("%s\n",str3);
    printf("%s\n",str4);
    printf("%s\n",str5);
    printf("%s\n",str6);
    printf("\n");
    */

    send_packet_esp8266(str1);
    send_packet_esp8266(str2);
    send_packet_esp8266(str3);
    send_packet_esp8266(str4);
    send_packet_esp8266(str5);
    send_packet_esp8266(str6);

    return;

}

void send_packet_esp8266(char * str){

    CURL *curl;
    CURLcode res;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, str);
        /* example.com is redirected, so we tell libcurl to follow redirection */
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);
        /* Check for errors */
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",

        curl_easy_strerror(res));

        /* always cleanup */
        curl_easy_cleanup(curl);
    }

}



