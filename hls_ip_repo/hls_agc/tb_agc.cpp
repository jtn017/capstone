/*
 * Author: Ryan hiser
 * Project: Capstone V2X Motorcycle HUD
 * File: tb_agc.cpp
 *
 * Description: Test Bench For LOG Based AGC
 */
#include <stdio.h>
#include <math.h>
#include "agc.h"

void readfile(char *filename, short *I, short *Q, unsigned int len)
{
	short buf[2];

	FILE *fp;
	fp = fopen(filename, "r");
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
	short x_re[size];
	short x_im[size];
	dout_t y_re[size];
	dout_t y_im[size];
	gain_t log2_gain[size];

	char *in_filename = (char *) "agc_input.dat";
	char *out_filename = (char *) "agc_output.csv";
	fp = fopen(out_filename, "w");
	readfile(in_filename,x_re,x_im,size);

	for(int i=0; i<size; i++){
		agc(12.0, (din_t)x_re[i], (din_t)x_im[i], &y_re[i], &y_im[i], &log2_gain[i]);
		printf("%8d, %8d, %8d, %8d, %f\n", x_re[i], x_im[i], y_re[i].to_int(), y_im[i].to_int(), (float) log2_gain[i]);
		fprintf(fp,"%d, %d, %d, %d, %f\n", x_re[i], x_im[i], y_re[i].to_int(), y_im[i].to_int(), (float) log2_gain[i]);

	}
	printf("DONE WITH TEST BENCH!\n");
	printf("Now proceed to Matlab folder and run agc.m for verification");
	fclose(fp);
	return 0;
}
