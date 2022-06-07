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
| 0x10  | Control  |
|       | start/restart  (bit 0) active high|
|       | AGC Bypass (bit 1) active high|
|       | PLL Bypass (bit 2) active high|
|       | PLL Selection (bit 3) 1'b1 - do not use HLS version|
|       | Reserved for future use (bit 31-4) |
| 0x18 | Peak Detect Threshold |
| 0x20| AGC Power Reference Q20.6 (26 Bits) |
| 0x28 | Store Delay (32 bits) |
| 0x30 | Time Slice Select (3 bits) See TEC for more details |

In our software see [v2x_sdr_vio.h](../../main/inc/v2x_sdr_vio.h) where we handle accessing the axi-registers.