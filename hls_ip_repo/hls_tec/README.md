# Timing Error Correction
This directory contains all design files used to create our custom Timing Error Correction (TEC) IP using Vitis HLS.

## Repository Directories
The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| tec.cpp   | TEC Code  |
| tec.h | Header file for TEC   |
| coefs.h | Filter coefficients 
| script.tcl    | TCL Script for Vitis to generate project  |
| [matlab](matlab) | Directory containing Matlab scripts for TEC investigation   |
| matlab/QPSK_TEC.m | Main script showing how the TEC works|
| matlab/xcorr.m | Main script for checking the HLS results. File also acts as the testbench for HLS verification. |
| matlab/create_coefs_h.m | script to generate 'coefs.h' file  |
| matlab/pngen.m | pseudo random sequence generator   |
| matlab/saveIQdat.m |  script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m |  script to save figures generated in matlab to a file |


## Background and resources

In our research of the frequency correction we also found the same paper useful for timing error correction. So... fred harris’ paper on System Synchronization:

* Harris, F. (2011). Let’s Assume the System Is Synchronized. In: Prasad, R., Dixit, S., van Nee, R., Ojanpera, T. (eds) Globalization of Mobile and Wireless Communications. Signals and Communication Technology. Springer, Dordrecht. https://doi.org/10.1007/978-94-007-0107-6_20