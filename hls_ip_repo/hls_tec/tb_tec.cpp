#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "math.h"
#include "tec.h"

#define SIZE 10000
#define ERROR_TOL 0.01


struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}
};

int main ()
{
	Rmse rmse_R, rmse_I;
	FILE *fin, *fout;
	float IN_R, IN_I;
	dout_t  OUT_R, OUT_I;
	float  GOLD_R, GOLD_I;
	unsigned int bank;
	bool vld;

	// Open files
	fin  = fopen("input.gold.dat",  "r");
	fout = fopen("output.gold.dat", "r");

	// Compare output
	for (int i = 0; i < SIZE; i++)
	{
		// Read input
		fscanf(fin, "%f %f", &IN_R, &IN_I);

		// Run tec
		tec((data_t)IN_R, (data_t)IN_I, &OUT_R, &OUT_I, &vld, &bank);

		if(vld){
			// Read output
			fscanf(fout, "%f %f", &GOLD_R, &GOLD_I);

			// Compare output
#ifdef BIT_ACCURATE
			rmse_R.add_value( OUT_R.to_float() - GOLD_R);
			rmse_I.add_value( OUT_I.to_float() - GOLD_I);
#else
			rmse_R.add_value( OUT_R - GOLD_R);
			rmse_I.add_value( OUT_I - GOLD_I);
#endif

			// Print output comparison
			printf("Expected: % 8.6f, Actual % 8.6f, BANK %d, %f, %f\n", GOLD_R, (float) OUT_R, bank, IN_R, IN_I );
		}
	}

	// Close files
	fclose(fin);
	fclose(fout);

	// Print error results
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(I)\n");
	printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
	printf("----------------------------------------------\n");

	if (rmse_R.error > ERROR_TOL || rmse_I.error > ERROR_TOL)
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
	    return 1;
	}
	else
	{
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
	    return 0;
	}
}
