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

# Repository Directories
The following table gives a brief summary of the contents of the top level directories.
Each directory may contain its own readme with more details.
| Directory                  | Details                                      |
| -------------------------- | -------------------------------------------- |
| [arm_sw](arm_sw)           | Contains ZedBoard ARM development test code  |
| [docs](docs)               | Contains docs created for class/git docs     |
| [hdl_ip_repo](hdl_ip_repo) | Contains verilog code for our demodulator IP |
| [hls_ip_repo](hls_ip_repo) | Contains our Vitis HLS IP source code        |
| [hud](hud)                 | Contains HUD webserver and display code      |
| [main](main)               | Contains ZedBoard SW code for TX/RX          |
| [models](models)           | Contains TX/RX MATLAB/Simulink Models        |

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

## FPGA Design

This project is based off Analog Devices base project. https://github.com/analogdevicesinc/hdl

Our additions consist of adding the following:

* Decimation Filter 
* Interpolation Filter
* [RX_IP](hdl_ip_repo) 
  * This module contains several custom IP located in [hls_ip_repo](hls_ip_repo).
  * A test bench exists for testing the RX IP
  * This code was packaged using Vivado IP Packager.

For ease all code that was developed resides in our git repo. However, for ease of reuse the complete Vivado project can be found in the google drive located here: [HDL](https://drive.google.com/drive/folders/1hCCl8DFLbI2U8BpWim0KFGjcfS-IHtDU?usp=sharing)

### Our Additions

The FMCOMMS4 has a minimum sampling rate of 25MHz. Our design is meant to operate at 4 MSPS. As a result we had two options:

* Use the Half-band filters that reside in the AD9361.
* Add interpolating/decimating filters in the FPGA.

We decided to add filters in the FPGA, based on the easy availability of the tutorial below which needed only slight modifications:

https://wiki.analog.com/resources/fpga/docs/hdl/fmcomms2_fir_filt

In our design the filters were added:

![alt text](docs/images/Rate_change.PNG?raw=true)

Then we added our IP and set the addresses appropriately:

![alt text](docs/images/rx_ip_bd.PNG?raw=true)

| IP NAME          | Address     |
| ---------------- | ----------- |
| rx_ip            | 0x43C0_0000 |
| rx_ip_ctrl       | 0xXXXXXXXXX |
