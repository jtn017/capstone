/*
 * Author: Ryan Hiser
 * Project:
 * File: tb_norm.cpp
 *
 * Description:
 */
#include <stdio.h>
#include <math.h>
#include "norm.h"

void readfile(char *filename, float *I, float *Q, unsigned int len)
{
	float buf[2];

	FILE *fp;
	fp = fopen(filename, "rb");
	if(fp == NULL){
		fprintf(stderr,"Unable to open input file\n");
	}

	for(int idx=0; idx<len;idx++){
		fread(buf,sizeof(float),2,fp);
		I[idx] = buf[0];
		Q[idx] = buf[1];
	}

	fclose(fp);
}

int main(){
	FILE *fp;
	unsigned int size = 2000;
	float x_re[size];
	float x_im[size];
	dout_t y_re[size];
	dout_t y_im[size];

	char *in_filename = (char *) "norm_input.dat";
	char *out_filename = (char *) "norm_output.csv";
	fp = fopen(out_filename, "w");
	readfile(in_filename,x_re,x_im,size);

	for(int i=0; i<size; i++){
		normalizer((din_t)x_re[i], (din_t)x_im[i], &y_re[i], &y_im[i]);
		printf("%f, %f, %f, %f\n", x_re[i], x_im[i], (float) y_re[i], (float) y_im[i]);
		fprintf(fp,"%f, %f, %f, %f\n", x_re[i], x_im[i], (float) y_re[i], (float) y_im[i]);

	}
	printf("DONE WITH TEST BENCH!\n");
	printf("Now proceed to Matlab folder and run agc.m for verification");
	fclose(fp);
	return 0;
}
