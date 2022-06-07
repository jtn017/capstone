# Fine Frequency Correction using a Phase Lock Loop

This directory contains all design files used to create our custom PLL IP using Vitis HLS.

![PLL Block Diagram](../../docs/images/hls_pll_1.png?raw=true)

Above we can see that the design essentially performs a phasor multiplication of the input. The result is used to determine the phase error. The phase error is then fed through a loop filter before being fed back into the phasor. The values of Ki and Kp were chosen via investigation in the matlab script.

## Repository Directories

The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| pll.cpp   | PLL Primary Code  |
| pll.h | Header file for PLL   |
| pll_test.cpp | HLS test bench that uses RMSE to compare to Matlab results |
| script.tcl    | TCL Script for Vitis to generate project  |
| matlab | Directory Containing Matlab scripts for phase normalizer investigation   |
| matlab/pll_test.m | Main script for generating test data for comparison in HLS testbench |
| matlab/QPSK_PLL.m | Matlab script for investigating the PLL |
| matlab/pngen.m | pseudo random sequence generator   |
| matlab/saveIQdat.m |  script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m |  script to save figures generated in matlab to a file |

## Notes on Design
When we had issues with the PLL with the two-way link we modified our design to have two implementations. This is set with a #define in the header file.

One option, "BETTER", uses the an atan. The other option uses no atan and performs the error calculation as described here: https://dsp.stackexchange.com/questions/31497/how-to-correct-the-phase-offset-for-qpsk-i-q-data

This method allows for an easier implementation because no atan is needed.

We also implemented a [non-HLS version](../../hdl_ip_repo/pll.v).

All of the PLL is still in an experimental state needing further debugging.
