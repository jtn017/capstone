# Cross Correlator (used for Coarse Frequency Correction)
This directory contains all design files used to create our Custom xcorr IP using Vitis HLS.

## Repository Directories
The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| xcorr.cpp   | XCorr IP Code  |
| xcorr.h | Header file for xcorr   |
| tb_xcorr.cpp | Test bench for Xcorr IP. Generates data files for comparison in matlab|
| script.tcl    | TCL Script for Vitis to generate project  |
| [matlab](matlab) | Directory containing Matlab scripts for AGC investigation   |
| matlab/xcorr.m | Main script showing how the AGC works and also checking the HLS results. File also acts as the testbench for HLS verification|
| matlab/pngen.m | pseudo random sequence generator   |
| matlab/saveIQdat.m |  script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m |  script to save figures generated in matlab to a file |
| matlab/QPSK_CoarseFreqCorrection_preamble.m |  Matlab script to investigate design of coarse frequency correction|

## XCORR
This IP performs a few tasks:

1. First it performs the cross correlation of the incoming signal with our known preamble sequence.
2. Performs peak detection on the correlation results.
3. Delays the incoming signal to align with the output of the correlator.
4. Calculates the coarse frequency offset error.

![alt text](../../docs/images/hls_xcorr_1.png?raw=true)

### Correlation
Correlation is performed by storing a time flipped copy of the preample (xcorr.h). This preamble is a series of 1 and 0's. Our preamble uses the top right quadrant (1,1) and the bottom left quadrant (-1,-1) as the two points of the BPSK sequence. The FPGA then performs correlation by taking each sample and using the sign of the preamble to invert before performing a summ over the length of the preamble. The sliding operation of the correlation is performed automatically as each sample is received by the ADC.

### Peak Detection
The peak detection looks for a correlation value above a threshold. This value is then stored. If another value is detected above the threshold and above the last stored value then it overwrites the last value. After a timeout period the value resets.

![alt text](../../docs/images/hls_xcorr_2.png?raw=true)