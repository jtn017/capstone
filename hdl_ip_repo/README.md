# Our complete RX (Demodulator) Intellectual Property (IP)

This folder contains the files for the RX IP which is a Verilog file that connects up all of our [hls_ip](../hls_ip_repo/).

## Repository Directories
| Directory/File        | Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| rx.v   | Verilog file linking all IP and adding other needed functions  |
| tb_rx.h | test bench file for testing rx.v |
| [matlab](matlab) | Directory containing Matlab scripts for TEC investigation   |
| matlab/QPSK.m | Main script showing how our receive processing works|
| matlab/pngen.m | pseudo random sequence generator   |
| matlab/saveIQdat.m | script to save data to a .dat file for use in Vitis HLS testbench |
| matlab/saveplot.m | script to save figures generated in matlab to a file |

## Architecture

The matlab script demonstrates the entire receiver processing. This flow was implemented in a Verilog file. The Verilog file passes the data through the IP and Verilog processing in the following order:

* Clock Crossing (4MHz to 200MHz)
* [Automatic Gain Control](../hls_ip_repo/hls_agc) (AGC)
* Matched Filter - SRRC (uses Xilinx FIR Compiler)
* [Coarse Frequency Correction](../hls_ip_repo/hls_xcorr) (XCORR)
* [Timing Error Correction](../hls_ip_repo/hls_tec) (TEC)
* [Fine Frequency Correction](../hls_ip_repo/hls_pll) (PLL)
* Hard Decision
* Data Storage and AXI interface (data2ps)
  
### Data Processing Chain

Our data rates are as follows below. These were chosen to ensure a transmit rate of .01s and an audio rate (mono, 16bit, 4kHz, 0.01s). Additionally, we added some buffer to allow for multiple radios to have a time slot.
![alt text](../docs/images/rx_data_chain.PNG?raw=true)

### Clocking Architecture

In order to ensure a "Real Time" design approach we need to ensure each of our HLS IPs could complete before the arrival of the next sample. Staying real time allows the processing system (PS) more time for other processing. This real-time processing was accomplished by crossing clock domains. The first domain consists of a clock of 4MHz coming from the ad9361 ADC. Then we cross using a metastability flip-flop circuit to a 200MHz clock. The diagram below illustrates the clock domains and the amount of clocks the IP have to process between samples.

![alt text](../docs/images/rx_clock_chain.PNG?raw=true)

## Test Bench

The [tb_rx](tb_rx.v) file provides stimulation for our rx IP. The matlab script creates data files that the test bench uses as an input to the rx IP. The test bench results can be viewed in the waveform viewer.

![alt text](../docs/images/rx_testbench_output.PNG?raw=true)

### Added Value

The test bench helped the team to identify several issues with our design. 

First, it helped in ensuring all our IP are integrated and connected correctly. Additionally, it allowed us to verify our function behavior and ensure the timing and correct signals were stored.

Secondly, it helped to identify issues with both the timing error correction (TEC) and the phase lock loop (PLL).

*Timing Error Correction:*

Through the testbench we were able to fully understand the output of the TEC. Our design was based of the [poly-phase filter bank synchronization block from GNU Radio](https://github.com/gnuradio/gnuradio/blob/master/gr-digital/lib/pfb_clock_sync_fff_impl.cc). However, that design had extra code for selecting the correct sample. In our investigation we found that this code wasn't needed.

Our simulation also helped us to identify that the matched filters were not working as expected. We used our simulink model to create data input for the Verilog test bench. This allowed us to realize that the filter coefficients for the software transmitter were not the same as what was implemented in the receiver.
