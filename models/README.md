# Repository Directories
The following table gives a brief summary of the most important top level files and directories.
Each directory may contain its own readme with more details.
| Directory/File         | Details                                                             |
| ---------------------- | ------------------------------------------------------------------- |
| [audio_gen](audio_gen) | Audio files, MATLAB code for conversion/playback                    |
| [main](main)           | Folder containing buildable SW using Simulink generated code        |
| [misc](misc)           | Miscellaneous files (currently houses old and unused MATLAB scripts |
| [src](src)             | Directory containing the Simulink generated C code                  |
| run_v2x_baseband_tb.m  | Runs the V2X Baseband simulation (TX/RX baseband)                   |
| run_v2x_modem_tb.m     | Runs the V2X Modem simulation (TX/RX baseband and mod/demod)        |
| v2x_baseband.slx       | Library Simulink file containing the V2X Baseband blocks            |
| v2x_baseband_init.m    | Initialization file for V2X Baseband variables/settings             |
| v2x_baseband_tb.slx    | Simulink model containing blocks for TX/RX baseband verification    |
| v2x_modem.slx          | Library Simulink file containing the V2X Modulator blocks           |
| v2x_modem_init.m       | Initialization file for V2X Modem variables/settings                |
| v2x_modem_tb.slx       | Simulink model containing blocks for TX/RX modem verification       |
| [QPSK.m](QPSK.m)       | RX Demodulator MATLAB simulation script                             |

# V2X Models
This directory contains the MATLAB scripts and Simulink models used to model the V2X system (TX and RX).
From these Simulink models, C code is generated and used in the V2X software.
Generally speaking, the workflow thus far has been to update the Simulink model, generate the C code, then finally
update the `main` folder so that running `make` inside `main` runs successfully and the data from the C code matches
the simulink vectors.

## Full Datapath Simulation
![alt text](../docs/images/simulink_modem.PNG?raw=true)

The full datapath simulation can be exercised by running the file `run_v2x_modem_tb.m`.
The TX to RX processing chain occurs in the following order:
- TX Baseband
- TX Modulator
- RX Demodulator
- RX Baseband

HUD data and audio samples are transmitted and processed from known values and binary files.
These data packets consist of 98 bytes total, and are sent at 0.01 second intervals.

## TX Baseband
![alt text](../docs/images/simulink_tx_baseband.PNG?raw=true)
The TX Baseband consists of the following blocks:
- Byte to Bit converter
- Scrambler
- Encoder (Reed-Solomon (7, 3, 3))
- Preamble Prepender (N = 6, 32 bits out, repeated 2x)

## TX Modulator
![alt text](../docs/images/simulink_tx_modulator.PNG?raw=true)
The TX Modulator consists of the following blocks:
- Mapper (QPSK)
- Pulse Shaping (SRRC)

## RX Demodulator
![alt text](../docs/images/simulink_rx_demodulator.PNG?raw=true)
The RX Demodulator consists of the following blocks:
- AGC
- Matched Filter (SRRC)
- Coarse Frequency Compensation (FLL)
- Symbol Timing Recovery (TEC)
- Fine Frequency Compensation (PLL)

## RX Baseband
![alt text](../docs/images/simulink_rx_baseband.PNG?raw=true)
The RX Baseband consists of the following blocks:
- Preamble Extractor
- Decoder (Reed-Solomon (7, 3, 3))
- Descrambler
- Bit to Byte converter
