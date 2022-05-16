# V2X IP Created using VITIS HLS 2021.1
This directory contains our Vitis HLS IP. Each folder contains its own readme with more details, as well as accompanying code and matlab scripts.

| IP      | Name/Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| hls_agc   | Automatic Gain Control |
| hls_fll_be | We began our Coarse Frequency Correction using Band Edge Filters. It is not used in final design|
| hls_normalizer | Alternative AGC approach using two CORDICS |
| hls_pll | Phase Lock Loop for fine frequency correction |
| hls_tec | Timing Error Correction (TEC) used for Symbol/Timing Recovery |
| hls_xcorr | Cross Correlator used for Coarse Frequency Correction (CFC) | 