#ifndef V2X_CONSTANTS_H
#define V2X_CONSTANTS_H

// Debug
#define DEBUG_BUILD 1
#define ERROR_TOL 0.0001
#define NUM_FRAMES 4

// Constants
#define INFO_PKT_BITS 144
#define INFO_PKT_BYTES (INFO_PKT_BITS/8)
#define AUDIO_PKT_BITS 640
#define AUDIO_PKT_BYTES (AUDIO_PKT_BITS/8)
#define AUDIO_PKTS 6000 // 60 seconds of 0.01s audio captures

// TX Baseband
#define TX_BB_IN_BITS 784
#define TX_BB_IN_BYTES (TX_BB_IN_BITS/8)
#define TX_BB_ENC_BITS 1848
#define TX_BB_OUT_BITS 1976

// TX Modulator
#define TX_MOD_SPS 8
#define TX_MOD_MAP_SYMS 988
#define TX_MOD_OUT_SYMS 7904

// RX Baseband
#define RX_BB_IN_BITS (TX_BB_OUT_BITS)
#define RX_BB_OUT_BYTES (TX_BB_IN_BYTES)
#define RX_BB_DEC_BITS (TX_BB_ENC_BITS)
#define RX_BB_IN_DWORD 64

// Binary file
#define BIN_FILE_MAX (TX_MOD_OUT_SYMS * NUM_FRAMES)

// Fixed point conversion
// Modulator is sfix16_En14
#define MOD_FRACT_BITS 14

// Flag used to reset payload data structure
#define EXPLICIT_INITIALIZATION

// UDP client related parameters
#define IP_PROTOCOL 0
#define IP_ADDRESS "10.0.0.1" // IP of pynq_z2
// #define IP_ADDRESS "127.0.0.1" // localhost
// #define IP_ADDRESS "192.168.1.23" // localhost
#define PORT_NO 15050
#define sendrecvflag 0

// #define USE_UDP_CLIENT

#endif // V2X_CONSTANTS_H