#ifndef V2X_RX_BASE_WRAPPER_H
#define V2X_RX_BASE_WRAPPER_H

// Includes
#include "rtwtypes.h"

// Function declarations
void rx_bb_init(void);
void get_rx_bb_out(uint8_T* output_frame, int frame_num);

#endif // V2X_RX_BASE_WRAPPER_H