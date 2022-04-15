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
static boolean_T rtY_descr_in[RX_BB_IN_BITS];

// ---------------------- Function prototype ----------------------
static void v2x_rx_bb_one_step(RT_MODEL *const rtM);
static void get_rx_input(int frame_num);
static void get_adc_packet(bool adc_packet[RX_BB_IN_BITS]);

// ---------------------- Temporary functions ----------------------
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
    V2X_RX_Baseband_step(rtM, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in);

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
    V2X_RX_Baseband_initialize(rtMPtr, rtU_v2x_rx_bb_in, rtY_data_frame, rtY_dec_in, rtY_descr_in);
}

void get_rx_bb_out(uint8_T* output, int frame_num)
{
    get_rx_input(frame_num);
    v2x_rx_bb_one_step(rtMPtr);
    memcpy(output, rtY_data_frame, RX_BB_OUT_BYTES*sizeof(rtY_data_frame[0]));
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
    
    return 0;
}

#define BUFFER_SIZE 1024

void tx_payload_wifimodule(struct payload_struct * pyld){

    // int fd;
    unsigned int n_sockets = N_SOCKETS;
    int fds[n_sockets];

#ifdef HTTP_SOCKET
    char ip_addr[] = "192.168.1.16";
    in_port_t port = 80;

    // fd = socket_connect(ip_addr,port);
    for(int i=0;i<n_sockets;i++){
        fds[i] = socket_connect(ip_addr,port);
    }
#endif

    char str1[200];
    char str2[200];
    char str3[200];
    char str4[200];
    char str5[200];
    char str6[200];

    char buffer[BUFFER_SIZE];

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
    sprintf(str1, "GET /name?value=%s HTTP/1.1\r\n\r\n", strs.name);
    sprintf(str2, "GET /lat?value=%s HTTP/1.1\r\n\r\n", strs.lat);
    sprintf(str3, "GET /lon?value=%s HTTP/1.1\r\n\r\n", strs.lon);
    sprintf(str4, "GET /speed?value=%s HTTP/1.1\r\n\r\n", strs.speed);
    sprintf(str5, "GET /dir?value=%s HTTP/1.1\r\n\r\n", strs.dir);
    sprintf(str6, "GET /dist_next_step?value=%s HTTP/1.1\r\n\r\n", strs.dist_next_step);

#ifdef HTTP_SOCKET
    int ret;
    // Do a GET HTTP request to ESP8266 Module
    ret = write(fds[0], str1, strlen(str1)); 
    if (ret != strlen(str1)){
		printf("Something went wrong with str1!\n");
	}

    ret = write(fds[1], str2, strlen(str2)); 
    if (ret != strlen(str2)){
		printf("Something went wrong with str2!\n");
	}

    ret = write(fds[2], str3, strlen(str3)); 
    if (ret != strlen(str3)){
		printf("Something went wrong with str3!\n");
	}

    ret = write(fds[3], str4, strlen(str4)); 
    if (ret != strlen(str4)){
		printf("Something went wrong with str4!\n");
	}

    ret = write(fds[4], str5, strlen(str5)); 
    if (ret != strlen(str5)){
		printf("Something went wrong with str5!\n");
	}

    ret = write(fds[5], str6, strlen(str6)); 
    if (ret != strlen(str6)){
		printf("Something went wrong with str6!\n");
	}
#endif

    /*
    printf("%s\n",str1);
    printf("%s\n",str2);
    printf("%s\n",str3);
    printf("%s\n",str4);
    printf("%s\n",str5);
    printf("%s\n",str6);
    */

#ifdef HTTP_SOCKET
    // close(fds);
    
    for(int i=0;i<n_sockets;i++){
        shutdown(fds[i], SHUT_RDWR); 
        close(fds[i]);
    }

#endif

    return;
}



