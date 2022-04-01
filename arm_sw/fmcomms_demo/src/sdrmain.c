#include "sdr.h"
sdrini_t sdrini={0};
sdrstat_t sdrstat={0};

int main (int argc, char **argv)
{
	/* read in configuration/initialization file */
	if (readinifile(&sdrini) < 0){
		return -1;
	}
	startsdr();
	
	return 0;
}
extern void startsdr(void)
{	
	/* stream data, write it to file then prepare to read file.*/	
	if(startxcvr(&sdrini,&sdrstat) < 0){
		printf("Transceiver FAILED!\n");
		return ;
	}
	printf("SDR Program COMPLETED!\n");
	return ;
}



