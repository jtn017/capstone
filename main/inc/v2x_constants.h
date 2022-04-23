#ifndef V2X_CONSTANTS_H
#define V2X_CONSTANTS_H

// Debug
#define DEBUG_BUILD 0
#define ERROR_TOL 0.0001
#define NUM_FRAMES 4

// Constants
#define INFO_PKT_BITS 144
#define INFO_PKT_BYTES (INFO_PKT_BITS/8)
#define AUDIO_PKT_BITS 7056
#define AUDIO_PKT_BYTES (AUDIO_PKT_BITS/8)

// TX Baseband
#define TX_BB_IN_BITS 7200
#define TX_BB_IN_BYTES (TX_BB_IN_BITS/8)
#define TX_BB_ENC_BITS 16800
#define TX_BB_OUT_BITS 16928

// TX Modulator
#define TX_MOD_MAP_SYMS 8464
#define TX_MOD_OUT_SYMS 67712

// RX Baseband
#define RX_BB_IN_BITS (TX_BB_OUT_BITS)
#define RX_BB_OUT_BYTES (TX_BB_IN_BYTES)
#define RX_BB_DEC_BITS (TX_BB_ENC_BITS)

// Binary file
#define BIN_FILE_MAX (TX_MOD_OUT_SYMS * NUM_FRAMES)

// Fixed point conversion
// Modulator is sfix16_En14
#define MOD_FRACT_BITS 14

#endif // V2X_CONSTANTS_H