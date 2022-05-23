# Data to PS IP Module

This directory contains all design files used to create our custom data2ps IP using Vitis HLS.

## Repository Directories

The following table gives a brief summary of the most important top level files and directories:
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| data2ps.cpp   | Code that implements a RAM storage and AXI-LITE Interface  |
| data2ps.h | Header file for data2ps   |

## Address Space (offset form AXI Address)

| Address Offset        | Description |
| --------------------- | ------------------------------------------------------------------- |
| 0x000 | reserved |
| 0x004 | reserved |
| 0x008 | reserved |
| 0x00c | reserved |
| 0x010 | rdy  (bit 0)|
| 0x014 | rdy_ap_vld (bit 0)|
| 0x020 | Firmware IP Version Date |
| 0x024 | version_ap_vld (bit 0) |
| 0x100 - 0x1ff | Results of RX_IP (64 * 32b) |

## Notes

The first four 32 bit values (addresses 0x24, 0x28, 0x32, and 0x36) will hold the preamble.