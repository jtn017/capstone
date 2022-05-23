# V2X IP Created using VITIS HLS 2021.1
This directory contains our Vitis HLS IP. Each folder contains its own readme with more details, as well as accompanying code and matlab scripts.

| IP      | Name/Details                                                             |
| --------------------- | ------------------------------------------------------------------- |
| [hls_agc](hls_agc)  | Automatic Gain Control |
| [hls_data2ps](hls_data2ps)  | IP to store received data and transfer to ARM |
| [hls_fll_be](hls_fll_be) | We began our Coarse Frequency Correction using Band Edge Filters. It is not used in final design|
| [hls_normalizer](hls_normalizer) | Alternative AGC approach using two CORDICS |
| [hls_rxif](hls_rxif)  | Axi-lite IP used to control the RX_IP from the arm |
| [hls_pll](hls_pll) | Phase Lock Loop for fine frequency correction |
| [hls_symsel](hls_symsel) | Correlator based of hls_xcorr for finding best sample|
| [hls_tec](hls_tec) | Timing Error Correction (TEC) used for Symbol/Timing Recovery |
| [hls_xcorr](hls_xcorr) | Cross Correlator used for Coarse Frequency Correction (CFC) |