
## FPGA Design

This project is based off Analog Devices base project. https://github.com/analogdevicesinc/hdl

Our additions consist of adding the following:

* Decimation Filter
* Interpolation Filter
* [RX_IP](../hdl_ip_repo)
  * This module contains several custom IP located in [hls_ip_repo](../hls_ip_repo).
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

![alt text](../docs/images/Rate_change.PNG?raw=true)

Then we added our IP and set the addresses appropriately:

![alt text](../docs/images/rx_ip_bd.PNG?raw=true)

| IP NAME          | Address     | Other  |
| ---------------- | ----------- |------- |
| rx_ip            | 0x43C0_0000 | [see the code](./main/inc/v2x_sdr_uio.h)       |
| rx_interface     | 0x4000_0000 | [see the code](./main/inc/v2x_sdr_uio.h)       |

### FPGA Utilization

| Resource | Utilization | Available | Utilization % |
| -------- | ------------| --------- | ------------- |
|LUT|25555|53200|48.035713|
|LUTRAM|2992|17400|17.195402|
|FF|34391|106400|32.32237|
|BRAM|72.5|140|51.785713|
|DSP|126|220|57.272728|
|IO|124|200|62.0|
|BUFG|8|32|25.0|
|MMCM|2|4|50.0|