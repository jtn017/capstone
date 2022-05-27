# Boot Files

Once we had a working FPGA .bit and .xsa file we used Petalinux with the [meta-adi](https://github.com/analogdevicesinc/meta-adi/) Yocto layer to build the required [sd card boot files](bootfiles).

The boot files contain all the important code to ensure the linux project can interface with our Vivado design.

These files are combined with a [Pynq root file system](https://github.com/CaptainKey/PYNQ-2.4-Zedboard) that was build for the zedboard.

In depth steps and instructions can be found [here](../docs/bsp_notes.md)