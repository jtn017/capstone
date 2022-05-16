# Automatic Gain Control
This directory contains all design files used to create our Custom AGC IP using Vitis HLS.

## Repository Directories
The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| agc.cpp   | AGC Code  |
| agc.h | Header file for AGC   |
| lut.h | Look-up-table header that is generated using matlab script |
| script.tcl    | TCL Script for Vitis to generate project  |
| matlab | Directory containing Matlab scripts for AGC investigation   |
| matlab/agc.m | Main script showing how the AGC works and also checking the HLS results. File also acts as the testbench for HLS verification|
| matlab/lut_gen.m | script to generate 'lut.h' file which is a look-up-table for 2^x   |
| matlab/saveIQdat.m |  script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m |  script to save figures generated in matlab to a file |

## HLS AGC 
We investigated three methods of Automatic Gain Control:
* ***Log Based AGC***
* Cordic Amplitude Agnostic Normalizer (using CORDICs)
* FMComms4/AD9361 built-in AGC

The AGC in this folder is implemented using a log() to and the exp() function.

![alt text](../../docs/images/hls_agc_1.png?raw=true)

### Logarithm and Exponential Implementation
The two functions can use any type of log() and because our system is a binary system we preferred to use log2() and exp2(). In our code we used the HLS Math library provided by Vitis in order to implement the log2() function. For the exp2() function we used a look-up-table (LUT), although depending on the required precision a shift operator can also be used. Using a shift operator would reduce resource usage.

Additionally, we added limits to the amount of gain based on expected input. This allowed us to limit the size of the exp2() LUT.

### Magnitude Approximation
Matlab provides the abs() function to calculate the magnitude. The normal method is to do sqrt(I^2+Q^2). We chose to simplify the calculation by performing an approximation.


````
abs_i = hls::abs(out_i_r);
abs_q = hls::abs(out_q_r);
if(abs_i >= abs_q){
abs_min = abs_i;
abs_max = abs_q;
}else{
abs_min = abs_q;
abs_max = abs_i;
}

mag_t mag_step1 = abs_max+(abs_min>>1);
mag_t mag_step2 = mag_step1 * 15;
agc_t mag = mag_step2 >> 4;
````

## Expected Output of HLS AGC Testbench
![alt text](../../docs/images/hls_agc_2.png?raw=true)