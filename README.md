# Program Details
This effort was part of [UCSD MAS WES](https://jacobsschool.ucsd.edu/mas/wes) Cohort 9 (2020-2022), consisting of the following students:
- Ryan Hiser: rhiser@ucsd.edu
- Jorge Pacheco: j5pachec@ucsd.edu
- Jacob Nguyen: jtn017@ucsd.edu

Other important work/resources are listed here:
- [Google Drive](https://drive.google.com/drive/folders/1-ji_W6FZ46F6Ge3nmcZqJ8UrlpsPyltQ?usp=sharing)

# Repository Directories
The following table gives a brief summary of the contents of the top level directories:
| Directory   | Details |
| ----------- | ------- |
| arm_sw      | TBD     |
| docs        | TBD     |
| hdl_ip_repo | TBD     |
| hls_ip_repo | TBD     |
| hud         | TBD     |
| main        | Contains ZedBoard SW code for TX/RX   |
| models      | Contains TX/RX MATLAB/Simulink Models |

# V2X Motorcycle HUD
TODO attach image

The V2X Motorcycle HUD project seeks to create a V2X (vehicle-to-everything) system to improve motorcycle group riding experiences.
The final implemented system will consist of 2 motorcycles, where each motorcycle will contain a full V2X Motorcycle HUD setup.
A custom waveform will be used to transmit HUD data and audio data betweem each motorcycle.
Each vehicle will speak with each other through an audio channel and share its driving status, where the driving status is shown on the HUD.
This information includes (but may not be limited to) the other vehicle’s name, geographic location, speed, directions, and distance to next step.

## HUD System Diagram and Materials
TODO attach image

As shown in the diagram above, each motorcycle in the V2X system consists of the 4 main components:
- [Avnet ZedBoard](https://www.avnet.com/wps/portal/us/products/avnet-boards/avnet-board-families/zedboard/)
- [PYNQ Z2](http://www.pynq.io/board.html)
- WiFi Module
- OLED Screen

The data flow is from one V2X TX to the other V2X RX is described as:
- ZedBoard1 SW creates a data packet and processes it (via TX Baseband/Modulator SW)
- ZedBoard1 SW buffers processed data for transmission
- ZedBoard2 PL processes the transmitted waveform (via RX Demod PL)
- ZedBoard2 PL stores demodulated data into UIO
- ZedBoard2 SW retrieves demodulated data and processes it (via RX Baseband SW) 
- ZedBoard2 SW sends data to HUD using HTTP requests

## Waveform Description and Data Packet
The waveform will consist of QPSK modulated data on the WiFi band (both TX/RX using the same frequency).
TDMA will be used to schedule transmission between the two motorcycles, and also to calculate the distance between the two motorcycles.
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
