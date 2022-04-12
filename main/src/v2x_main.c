/*
 * Main file
*/

// Includes (SDR)
#include "v2x_sdr.h"
#include "v2x_sdr_init.h"
#include "v2x_sdr_xcvr.h"

// Includes (Simulink)
#include "rtwtypes.h"
#include "v2x_constants.h"
#include "v2x_tx_bb_wrapper.h"
#include "v2x_tx_mod_wrapper.h"
#include "v2x_rx_bb_wrapper.h"

// Globals
sdrini_t  sdr_ini  = {0};
sdrstat_t sdr_stat = {0};

// ---------------------- Function Prototype ----------------------
void start_sdr(void);

// ---------------------- SDR functions ----------------------
void start_sdr(void)
{	
    // Stream data, write it to file then prepare to read file	
    if(run_xcvr(&sdr_ini, &sdr_stat) < 0)
    {
        printf("Transceiver FAILED!\n");
        return;
    }

    // Return
    printf("SDR Program COMPLETED!\n");
    return;
}

// ---------------------- Main ----------------------
int main(int argc, char **argv)
{
    // Read in config file
    if (readinifile(&sdr_ini) < 0){
        return -1;
    }

    // Start SDR
    start_sdr();
    return 0;
}
