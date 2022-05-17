# Program Details
This effort was part of [UCSD MAS WES](https://jacobsschool.ucsd.edu/mas/wes) Cohort 9 (2020-2022), consisting of the following students:
- Ryan Hiser: rhiser@ucsd.edu
- Jorge Pacheco: j5pachec@ucsd.edu
- Jacob Nguyen: jtn017@ucsd.edu

Other important work/resources are listed here:
- [Google Drive](https://drive.google.com/drive/folders/1-ji_W6FZ46F6Ge3nmcZqJ8UrlpsPyltQ?usp=sharing)

# Repository Directories
The following table gives a brief summary of the contents of the top level directories.
Each directory may contain its own readme with more details.
| Directory                  | Details                                     |
| -------------------------- | ------------------------------------------- |
| [arm_sw](arm_sw)           | Contains ZedBoard ARM development test code |
| [docs](docs)               | Contains docs created for class/git docs    |
| [hdl_ip_repo](hdl_ip_repo) | Contains verilog code for our demodulator IP     |
| [hls_ip_repo](hls_ip_repo) | Contains our Vitis HLS IP source code       |
| [hud](hud)                 | Contains HUD webserver and display code     |
| [main](main)               | Contains ZedBoard SW code for TX/RX         |
| [models](models)           | Contains TX/RX MATLAB/Simulink Models       |

# V2X Motorcycle HUD
![alt text](docs/images/v2x.png?raw=true)

The V2X Motorcycle HUD project seeks to create a V2X (vehicle-to-everything) system.
The motivation behind this effort is to improve motorcycle group riding experiences.
The final system consists of 2 motorcycles (wherein each motorcycle mounts a full V2X Motorcycle HUD setup).
A custom waveform is used to transmit HUD data (shown on the HUD) and audio (played through speakers).
This information includes the other vehicle’s name, geographic location, speed, directions, and distance to next step.

## HUD System Diagram and Materials
![alt text](docs/images/apparatus.png?raw=true)

As shown in the diagram above, each motorcycle in the V2X system consists of the 4 main components:

- [Avnet ZedBoard](https://www.avnet.com/wps/portal/us/products/avnet-boards/avnet-board-families/zedboard/)
- [PYNQ Z2](http://www.pynq.io/board.html)
- WiFi Module
- OLED Screen

The data flow is from one V2X TX to the other V2X RX is described as:

- ZedBoard1 SW creates a data packet and processes it (via TX Baseband/Modulator SW)
- ZedBoard1 SW buffers processed data for transmission via DMA and the Digital-to-Analog (DAC).
- ZedBoard2 PL processes the transmitted waveform (via RX Demodulator IP)
- ZedBoard2 PL stores demodulated data into a RAM accessed via UIO on the ARM.
- ZedBoard2 SW retrieves demodulated data and processes it (via RX Baseband SW) 
- ZedBoard2 SW sends data to HUD using HTTP requests

## Waveform Description and Data Packet
The waveform consists of QPSK modulated data on the WiFi band (both TX/RX using the same frequency).
TDMA is used to schedule transmission between motorcycles, and also to calculate distance between the motorcycles.

- Throughput: 78.4 kbps
- Minimum Required Sampling Frequency: 1.58 Msps
- Carrier Frequency: 2.4GHz (WiFi band)
- Maximum Doppler Frequency: 2,400,715 Hz (max speed of 100 MPH)
- Maximum Offset Frequency: 715 Hz
- Maximum Frequency Error: 781.25 KHz
- Maximum detection distance: 0.25mi (0.4km)

The data packet consists of 784 total bits, where the first 144 bits contain HUD data, and the next 640 bits contain audio samples.
| Parameter                         | Data type               |
| --------------------------------- | ----------------------- |
| Identifier                        | char[4]      (32 bits)  |
| Latitude                          | float        (32 bits)  |
| Longitude                         | float        (32 bits)  |
| Speed (mph)                       | uint8_t      (8  bits)  |
| Directions                        | uint8_t      (8  bits)  |
| Distance to next step             | float        (32 bits)  |
| Audio (mono, 16bit, 4kHz, 0.01s)  | uint16_t[40] (640 bits) |
