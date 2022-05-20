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

#include <fcntl.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP_PROTOCOL 0
// #define IP_ADDRESS "10.0.0.1" // IP of pynq_z2
//#define IP_ADDRESS "127.0.0.1" // localhost
#define IP_ADDRESS "192.168.1.23" // localhost
#define PORT_NO 15050
#define sendrecvflag 0

// ---------------------- Global variables ----------------------
boolean_T g_tx_bb_out[TX_BB_OUT_BITS];
cint16_T g_tx_mod_out[TX_MOD_OUT_SYMS];
unsigned int g_num_tx_frames;

// ---------------------- Function prototypes ----------------------
void read_char_from_bin(const char* filename, char* data, unsigned int num_elem);
void read_float_from_bin(const char* filename, float* data, unsigned int num_elem);

double fixed_to_double_txmod(int16_T x);
int16_T double_to_fixed_txmod(double x);
#if DEBUG_BUILD
static boolean_T test_tx_bb_out[TX_BB_OUT_BITS];
static creal32_T test_tx_mod_out[TX_MOD_OUT_SYMS];
static boolean_T test_rx_bb_out[RX_BB_OUT_BYTES];

void load_file(uint8_T *rx_bb_out);
void load_frame(uint8_T *rx_bb_out, uint8_T * g_rx_bb_out, int frame_num);
int compare_actual_vs_exp(boolean_T* tx_bb_out, cint16_T* tx_mod_out, uint8_T* rx_bb_out);
#endif

// ---------------------- Debug ----------------------
#if DEBUG_BUILD
void load_file(uint8_T * rx_bb_out)
{
    // RX Modulator expected output
    unsigned int num_frames = NUM_FRAMES;
    unsigned int num_bytes = RX_BB_OUT_BYTES;
    
    read_char_from_bin("data/v2x_rx_bb_out.bin", (char*) rx_bb_out, num_frames * num_bytes);

    // Return
    return;
}

void load_frame(uint8_T *rx_bb_out, uint8_T * g_rx_bb_out, int frame_num)
{
    unsigned int num_bytes = RX_BB_OUT_BYTES;
    unsigned int idx = RX_BB_OUT_BYTES * frame_num;
    memcpy(&g_rx_bb_out[0], &rx_bb_out[idx], num_bytes);

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
            printf("TX MOD ERROR: actual[%d].im: %f, expected[%d].im: %lf\n",
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
void read_char_from_bin(const char* filename, char* data, unsigned int num_elem)
{
    // Reading char from file
    char buffer[BIN_FILE_MAX_RX];
    FILE * bin_file = fopen(filename, "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);

    // Copy to passed array
    memcpy(data, buffer, num_elem * sizeof(*buffer));

    // Close file
    fclose(bin_file);
}

void read_float_from_bin(const char* filename, float* data, unsigned int num_elem)
{
    // Reading float (4 char size) from file
    float buffer[BIN_FILE_MAX];
    FILE * bin_file = fopen(filename, "rb");
    fread(buffer, sizeof(buffer), 1, bin_file);

    // Copy to passed array
    memcpy(data, buffer, num_elem * sizeof(*buffer));

    // Close file
    fclose(bin_file);
}

// ---------------------- Fixed point conversion ----------------------
// https://embeddedartistry.com/blog/2018/07/12/simple-fixed-point-conversion-in-c/
double fixed_to_double_txmod(int16_T x)
{
    double numerator = x * 1.0;
    double denominator = (1 << MOD_FRACT_BITS);
    return (numerator/denominator);
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

    // Initialize generated code
    tx_bb_init();
    tx_mod_init();
    rx_bb_init();

    unsigned int num_frames = NUM_FRAMES;
    unsigned int num_bytes = RX_BB_OUT_BYTES;
    uint8_T rx_bb_out[num_frames * num_bytes];
    uint8_T g_rx_bb_out[num_bytes];

    unsigned int frame_size = TX_BB_IN_BYTES;
    int sockfd;//, nBytes;
	struct sockaddr_in addr_con;
	int addrlen = sizeof(addr_con);
	addr_con.sin_family = AF_INET;
	addr_con.sin_port = htons(PORT_NO);
	addr_con.sin_addr.s_addr = inet_addr(IP_ADDRESS);

	// socket()
	sockfd = socket(AF_INET, SOCK_DGRAM,IP_PROTOCOL);

	if (sockfd < 0)
	{
		printf("\nfile descriptor not received!!\n");
		exit(-1);
	}
	else{
		printf("\nfile descriptor %d received\n", sockfd);
	}


    // Get TX baseband output
#if DEBUG_BUILD

    printf("Testing %d frames!\n",NUM_FRAMES_TO_TEST);
    load_file(rx_bb_out);
    // Set number of frames to compare
    for (int i = 0; i < NUM_FRAMES_TO_TEST; i++)
    {
        load_frame(rx_bb_out,g_rx_bb_out,i);
        
        fix_payload_packet(g_rx_bb_out);

        usleep(150e3);

        sendto(sockfd, (void *)g_rx_bb_out, frame_size, sendrecvflag, (struct sockaddr*)&addr_con, addrlen);

        printf("\n---------Data Sent for frame %d---------\n",i+1);
        
    }

    close(sockfd);

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
