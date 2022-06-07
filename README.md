[![alt text](docs/images/thumbnail.png?raw=true)](https://www.youtube.com/watch?v=SJ1P-lReLIU)

# Program Details
This effort was part of [UCSD MAS WES](https://jacobsschool.ucsd.edu/mas/wes) Cohort 9 (2020-2022), consisting of the following students:
- Ryan Hiser: rhiser@ucsd.edu
- Jorge Pacheco: j5pachec@ucsd.edu
- Jacob Nguyen: jtn017@ucsd.edu

Other important work/resources are listed here:
- [Google Drive](https://drive.google.com/drive/folders/1-ji_W6FZ46F6Ge3nmcZqJ8UrlpsPyltQ?usp=sharing)
- [iio_device library](https://analogdevicesinc.github.io/libiio/v0.16/group__Device)
- [iio_device TX attenuation example](https://wiki.analog.com/resources/tools-software/linux-drivers/iio-transceiver/ad9361#tx_attenuation_control)
- [ZedBoard Audio (linux device driver)](https://yuhei1-horibe.medium.com/linux-device-driver-for-zedboard-audio-7e2d1efc2941)
- [Zedboard Audio (bitstream design)](https://yuhei1-horibe.medium.com/zedboard-audio-hardware-design-b19c3a1bf453)
- [Petalinux Notes](./docs/bsp_notes.md)
  
# Repository Directories
The following table gives a brief summary of the contents of the top level directories.
Each directory may contain its own readme with more details.
| Directory                  | Details                                      |
| -------------------------- | -------------------------------------------- |
| [arm_sw](arm_sw)           | Contains ZedBoard ARM development test code  |
| [bootfiles](bootfiles)     | Contains Petalinux Generated Boot File (place on SD card boot partition)  |
| [docs](docs)               | Contains docs created for class/git docs     |
| [hdl_ip_repo](hdl_ip_repo) | Contains verilog code for our demodulator IP |
| [hls_ip_repo](hls_ip_repo) | Contains our Vitis HLS IP source code        |
| [hud](hud)                 | Contains HUD webserver and display code      |
| [main](main)               | Contains ZedBoard SW code for TX/RX          |
| [models](models)           | Contains TX/RX MATLAB/Simulink Models        |
| [udp_server](udp_server)           |         |
| [vivado_project](vivado_project)   | Archived Vivado Project         |

# Software Dependencies
Issue the following commands to make sure you're able to run our sourcecode:

```
sudo apt-get update && sudo apt-get install -y git build-essential linux-headers-generic dkms
sudo apt-get install -y libcurl4-openssl-dev libevent-dev gdb
```

# Software Requirements
This repository has saved versions of the latest working build (C code, bit file, kernel image).
In order to modify and/or regenerate the source code/files, the following programs are required.

- MATLAB/Simulink R2021b (C code generation)
  - TX Baseband
  - TX Modulator
  - RX Baseband
- Vitis HLS 2019 (IP core generation)
  - RX Demodulator
- Vivado 2021.1
  - Inter-IP Module connections
- Petalinux 2021.1
- Virtual Box 6.1 (VM for creating petalinux builds)
  - Build boot files
- Arduino IDE
  - ESP8266 Wi-Fi Module


# V2X Motorcycle HUD
![alt text](docs/images/v2x.png?raw=true)

The V2X Motorcycle HUD project seeks to create a V2X (vehicle-to-everything) system.
The motivation behind this effort is to improve motorcycle group riding experiences.
The final system consists of 2 motorcycles (wherein each motorcycle mounts a full V2X Motorcycle HUD setup).
A custom waveform is used to transmit HUD data (shown on the HUD) and audio (played through speakers).
This information includes the other vehicle’s name, geographic location, speed, directions, and distance to next step.

## System Diagram and Equipment
![alt text](docs/images/apparatus.png?raw=true)

As shown in the diagram above, each motorcycle HUD in the V2X system consists of these components:

- [Avnet ZedBoard](https://www.avnet.com/wps/portal/us/products/avnet-boards/avnet-board-families/zedboard/)
  - [AD-FMCOMMS4-EBZ](https://www.analog.com/en/design-center/evaluation-hardware-and-software/evaluation-boards-kits/eval-ad-fmcomms4-ebz.html)
  - Wi-Fi Antennas (2x)
- [PYNQ Z2](http://www.pynq.io/board.html)
- [ESP8266 Wi-Fi Module](https://www.amazon.com/gp/product/B081CSJV2V/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1)
- [OLED Display](https://www.amazon.com/gp/product/B09JWN8K99/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&th=1)

## Dataflow
![alt text](docs/images/dataflow.png?raw=true)

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

## Future Tasks
The current build (as of June 6, 2022) requires more work in order to get a more complete system build working.
Currently this build has gotten as far as an OTA loopback with a singular ZedBoard (no success with a 2 way link so far).
The following list states the components that need more debugging/verification/implementation.

- PLL
  - Does not consistently behave as desired
  - Most likely cause of 2 way link issue (clocks on each board likely not matching)
- TDMA
  - Unable to start implementation since 2 way link never established
- Distance between riders
  - Unable to start implementation since TDMA never implemented
