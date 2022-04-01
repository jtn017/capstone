# Demonstration of FMCOMMS4 TX/RX with PYNQ
Ryan Hiser, Jorge Pacheco, Jacob Nguyen
Capstone: V2X Motorcycle HUD

## Steps to Run
* Place files on PYNQ (ssh or FileZilla)
* In build directory run make

        make clean && make 
* After running make run the program
    
        sudo ./xcvr_demo
* Data will be stored in **fmcomms_data.dat**

* Then in browser navigate to 192.168.2.99
* Open the jupyter notebook **read_data_dump.ipynb**
* Run Code and you should see output in plot.

## Notes:

* "This is fine as long as you are within the for loop. Once the buffer is cleared and TX device is destroyed it will reset to the DDS. It is important to understand that even when pushing zeros to the DACs you can still experience LO leakage. To reduce the remaining signal, just leakage in this case, increase the TX attenuation (hardware_gain)." -Travis (from adi)

## Resources (links)
Below links were used to understand and write the code:
* https://analogdevicesinc.github.io/TransceiverToolbox/master/sysobjects/adi.AD9364.Tx/
* https://wiki.analog.com/resources/tools-software/linux-software/libiio_internals
* https://github.com/analogdevicesinc/libiio/blob/master/examples/ad9361-iiostream.c
* https://wiki.analog.com/resources/eval/user-guides/ad-fmcomms2-ebz/software/basic_iq_datafiles#ad-fmcomms2345_basic_iq_datafiles
* https://codedocs.xyz/analogdevicesinc/libiio/group__Buffer.html#ga6caadf077c112ae55a64276aa24ef832

Discussion with ADI:
* https://ez.analog.com/linux-software-drivers/f/q-a/556769/ad9361-iiostream-c-iio-buffer-always-reading-a-sinusoid