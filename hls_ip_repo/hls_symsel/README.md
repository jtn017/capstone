# Data to PS IP Module

This directory contains all design files used to create our custom data2ps IP using Vitis HLS.

## Repository Directories

The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| data2ps.cpp   | Code that implements a RAM storage and AXI-LITE Interface  |
| data2ps.h | Header file for data2ps   |

## Notes

The first four 32 bit values (addresses 0x24, 0x28, 0x32, and 0x36) will hold the preamble.