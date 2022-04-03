#ifndef V2X_CONSTANTS_H
#define V2X_CONSTANTS_H

// Debug
#define DEBUG_BUILD 1
#define ERROR_TOL 0.0001
#define NUM_FRAMES 4

// Constants
#define INFO_PKT_LEN 144
#define AUDIO_PKT_LEN 7056
#define TX_BB_IN_LEN 7200
#define TX_BB_ENC_LEN 16800
#define TX_BB_OUT_LEN 16928
#define TX_MOD_MAP_LEN 8464
#define TX_MOD_OUT_LEN 67712

// Binary file
#define BIN_FILE_MAX (TX_MOD_OUT_LEN * NUM_FRAMES)

// Fixed point conversion
// TX Modulator is sfix16_En14
#define TX_MOD_FRACT_BITS 14

#endif // V2X_CONSTANTS_H