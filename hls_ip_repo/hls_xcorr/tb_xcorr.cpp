/*
 * Author: Ryan hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: tb_xcorr.cpp
 *
 * Description: Test Bench For LOG Based AGC
 */
#include <stdio.h>
#include <math.h>
#include "xcorr.h"

void readfile(char *filename, short *I, short *Q, unsigned int len)
{
	short buf[2];

	FILE *fp;
	fp = fopen(filename, "rb");
	if(fp == NULL){
		fprintf(stderr,"Unable to open input file\n");
	}

	for(int idx=0; idx<len;idx++){
		fread(buf,sizeof(short),2,fp);
		I[idx] = buf[0];
		Q[idx] = buf[1];
	}

	fclose(fp);
}

int main(){
	FILE *fp;
	unsigned int size = 2000;
	din_t x_re[size];
	din_t x_im[size];
	dout_t y_re[size];
	dout_t y_im[size];
	corr_t corr[size];
	cmpy_t to_atan[2];
	bool max_vld;

	char *in_filename = (char *) "xcorr_input.dat";
	char *out_filename = (char *) "xcorr_output.csv";

	fp = fopen(out_filename, "w");
	readfile(in_filename,x_re,x_im,size);

	for(int i=0; i<size; i++){
		xcorr(x_re[i], x_im[i], &y_re[i], &y_im[i], &corr[i], &max_vld, &to_atan[0], &to_atan[1]);
		fprintf(fp,"%d, %d, %d, %d, %f, %d, %d, %d\n", x_re[i],  x_im[i], y_re[i], y_im[i], (float) corr[i], max_vld, to_atan[0], to_atan[1]);

	}
	printf("DONE WITH TEST XCORR BENCH!\n");
	printf("Now proceed to Matlab folder and run xcorr.m for verification");
	fclose(fp);
	return 0;
}
