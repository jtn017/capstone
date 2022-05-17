# Amplitude Normatlization (AGC for quadrature signals)

This directory contains all design files used to create our Custom Normalizer IP using Vitis HLS. We also consider this to be called our CORDIC AGC.

## Repository Directories

The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| norm.cpp   | Normalizer Code  |
| norm.h | Header file for normalizer   |
| script.tcl    | TCL Script for Vitis to generate project  |
| matlab | Directory Containing Matlab scripts for phase normalizer investigation   |
| matlab/phnorm.m | Main script showing how the Normalizer works and also checking the HLS results. File also acts as the testbench for HLS verification   |
| matlab/saveIQdat.m |  script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveIQdat_f.m |  script to save float data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m |  script to save figures generated in matlab to a file |

## HLS Normalization 
We investigated three methods of Automatic Gain Control:
* Log Based AGC
* ***Cordic Amplitude Agnostic Normalizer (using CORDICs)***
* FMComms4/AD9361 built-in AGC

The AGC in this folder is implemented using two sequential CORDICs. The first CORDIC uses a atan2 function to calculate the phase, and then the next CORDIC uses the phase to regenerate the signal with an unit amplitude. The CORDICs are easily implemented with the HLS Math library.

![alt text](../../docs/images/hls_norm_1.png?raw=true)

## Expected Output of HLS Norm Testbench
![alt text](../../docs/images/hls_norm_2.png?raw=true)