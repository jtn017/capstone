# Rx IP Control Module

This directory contains all design files used to create our custom Control Interface for our RX IP using Vitis HLS.

## Repository Directories

The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| rx_interface.cpp   | Code that implements a registers that are set via an AXI-LITE Interface  |

## Address Space (offset form AXI Address)

| Address Offset        | Description |
| --------------------- | ------------------------------------------------------------------- |
| 0x18  | Control  |
|       | start/restart  (bit 0)|
|       | AGC Bypass (bit 1) |
|       | future use (bit 31-2) |
| 0x20 | AGC Power Reference Q20.6 (26 Bits) |
| 0x28 | Store Delay (32 bits) |
| 0x30 | Time Slice Select (3 bits) See TEC for more details |
