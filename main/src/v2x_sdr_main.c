/*
 * Main file
*/

// Includes
#include "v2x_sdr.h"
#include "v2x_sdr_init.h"
#include "v2x_sdr_xcvr.h"

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
    /* read in configuration/initialization file */
    if (readinifile(&sdr_ini) < 0){
        return -1;
    }

    start_sdr();
    return 0;
}
