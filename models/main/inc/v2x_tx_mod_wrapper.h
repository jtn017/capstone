#ifndef V2X_TX_MOD_WRAPPER_H
#define V2X_TX_MOD_WRAPPER_H

// Includes
#include "rtwtypes.h"

// Function declarations
void tx_mod_init(void);
void get_tx_mod_out(boolean_T* input_frame, cint16_T* output_frame);

#endif // V2X_TX_MOD_WRAPPER_H