# Band Edge FLL
We investigated using Band Edge (BE) Frequency Lock Loop (FLL) and this directory contains the completed Matlab code, and the current status of the Vitis HLS code. We did not end up using the BE filters because it seems better suited to systems with larger Doppler offsets.

Regardless these files are here for future use and thoroughness.

## Background and resources
We began by researching Frequency Correction Methods. This led us to fred harris’ paper on System Synchronization:
* Harris, F. (2011). Let’s Assume the System Is Synchronized. In: Prasad, R., Dixit, S., van Nee, R., Ojanpera, T. (eds) Globalization of Mobile and Wireless Communications. Signals and Communication Technology. Springer, Dordrecht. https://doi.org/10.1007/978-94-007-0107-6_20

This text contained the following block diagram which we implemented in matlab.
![alt text](../../docs/images/hls_be_1.png?raw=true)
