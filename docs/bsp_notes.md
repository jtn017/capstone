# FMComms4 Rebuild Attempt
Master for Meta-adi and HDL git repositories use
Vivado 2021.1 and Petalinux 2021.1

Versions:
* Vivado 2021.1
* Ubuntu 16.04 LTS (Virtual Maching)
* Petalinux 2021.1
* Analog Devices HDL (master as of May 2022)
  
## Vivado Install
chmod +x Xilinx_Unified_2021.1_0610_2318_Lin64.bin  && ./Xilinx_Vivado_SDK_Web_2018.3_1207_2324_Lin64.bin

## Petalinux Install
https://www.fpgadeveloper.com/how-to-install-petalinux-2020.1/

## **Notes for HDL build**
	
	$ cd capstone_2
	$ git clone https://github.com/analogdevicesinc/hdl.git
	$ cd hdl/projects/fmcomms2/zed
	$ source /opt/Xilinx/Vivado/2021.1/settings64.sh 
	$ make

## Meta-adi and petalinux

	$ git clone https://github.com/analogdevicesinc/meta-adi.git
	$ source ~/petalinux/2021.1/settings.sh 
	$ petalinux-create -t project --template zynq --name peta_adi_proj
	$ cd peta_adi_proj/
	$ petalinux-config --get-hw-description=/media/sf_Capstone_work/hdl/projects/fmcomms2/zed/fmcomms2_zed.sdk/

note: make sure Vivado but the newest .xsa at the above location.
	
Add layers meta-adi-core and meta-adi-xilinx (in this order). Then...
Also add pl-delete-nodes for fmcomms4 (similar to fmcomms2-3). Also add appropriate file to device-tree.bbappend.

	$ echo "KERNEL_DTB=\"zynq-zed-adv7511-ad9364-fmcomms4\"" >> project-spec/meta-user/conf/petalinuxbsp.conf
	$ cd build
	$ petalinux-build

Now that it has build we can modify the device tree appropriately

Now let's rebuild
	$ petalinux-build
	$ petalinux-package --force --boot --fsbl --fpga --u-boot
	$ cd ../images/linux
	$ cp BOOT.BIN /media/adi/BOOT/
	$ cp image.ub /media/adi/BOOT/
	$ sudo tar xvf rootfs.tar.gz -C /media/adi/rootfs/

# **Below is a log of steps taken** 
These steps are retained for completeness and to fill any gpas in the above steps.
# Getting Started with HDL and Petalinux Project
Project: V2X Motorcycle HUD
For: UCSD MAS 

This is intended to get zed and fmcomms4 working with petalinux and adi's hdl project.

Versions:
* Vivado 2018.3
* Ubuntu 16.04 LTS
* petalinux 2018.3
* Analog Devices HDL hdl_2019_r1


## **Notes for HDL Build**
    cd ~
    mkdir proj
    cd proj
    git clone https://github.com/analogdevicesinc/hdl.git

Note: Most stable release is 2019_R1 so will need to install vivado 2018.3
* https://www.xilinx.com/support/download/index.html/content/xilinx/en/downloadNav/vivado-design-tools/archive.html

Get correct Branch 

    source /tools/Xilinx/Vivado/2018.3/settings64.sh 
    cd hdl
    git checkout hdl_2019_r1
    
Note: 
* No ZCU104 so was going to make it for both Zed and ZCU104(this should be similar to 104), but with webpack version no zcu102 board support. So will proceed with just zed.
* Also believe we have fmcomms4 boards. For now assuming fmcomms2 covers 2/3/4. 

Now generate HDL project:

        cd projects/fmcomms2/zed
        make

## **Build Petalinux**

    source /opt/pkg/petalinux/2018.3/settings.sh

Meta-adi project cannot be in shared folder (idk)

    cd ~ 
    git clone https://github.com/analogdevicesinc/meta-adi.git
    cd meta-adi
    git fetch
    git branch -v -a
    git checkout 2019_R1
    git pull origin fix_axi_sysid
    petalinux-create -t project --template zynq --name peta_zed
    cd peta_zed
    petalinux-config --get-hw-description=/home/v2x/proj/hdl/projects/fmcomms2/zed/fmcomms2_zed.sdk/


Add layers in this order (Yocto -> Layers): meta-adi-core, meta-adi-xilinx
* /home/v2x/proj/meta-adi/meta-adi-core
* /home/v2x/proj/meta-adi/meta-adi-xilinx

If desired change the username and password to root in file: /meta-adi-xilinx/recipes-core/images/petalinux-user-image-bbappend

Save and Exit.

This didn't work:

    echo "KERNEL_DTB = "zynq-zed-adv7511-ad9361-fmcomms2-3" " >> project-spec/meta-user/conf/petalinuxbsp.conf

So I copied it instead of using echo (May need to use our own modified version for DTS see proj/zynq)-zed-adv7511-ad9361-fmcomms2-3.dts)

    KERNEL_DTB = "zynq-zed-adv7511-ad9361-fmcomms2-3"   

Then

    cd build
    petalinux-build
    petalinux-packackage --force --boot --fsbl --fpga --u-boot

Move files to SD Card
    cd ../images/linux
    cp BOOT.BIN /media/v2x/BOOT
    cp image.ub /media/v2x/BOOT

## **Root File System**
Now Lets get the rootfs that we want.

### PYNQ Image

sudo tar xvf focal.arm.2.7.0_2021_11_17.tar.gz -C /media/v2x/rootfs

## **SD Card Setup**
    2 partitions:
        * size: 1G, format: Fat32, set boot flag, label: boot
        * size: Remaining, format: ext4, label: rootfs
    
## **Installations**
### Install Vivado 2018.3 (requires Ubuntu 16.04)
    cd /media/sf_capstone/software_installers
    chmod +x /VivadoVersionInstaller
    sudo ./VivadoVersionInstaller

Note: Did webpack verions; however, zcu104 may not be supported (atleast for pynq the hdmi requires license.)

### Install Petalinux

    sudo dpkg-reconfigure dash

Select **yes**

From UG1144:

    sudo apt-get install -y gcc git make net-tools libncurses5-dev tftpd zlib1g-dev libssl-dev flex bison libselinux1 gnupg wget diffstat chrpath socat xterm autoconf libtool tar unzip texinfo zlib1g-dev gcc-multilib build-essential libsdl1.2-dev libglib2.0-dev zlib1g:i386 screen pax gzip

Following were added after encountering errors:

    sudo apt install gawk
    sudo apt install python

Got error so replaced '-dev zlib1g:i386' with 'zlib1g:i386' and 'zlib1g-dev'

    mkdir -p /opt/pkg/petalinux/2018.3
    sudo chmod -R 755 /opt/
    ./petalinux-v2018.3-final-installer.run /opt/pkg/petalinux/2018.3

**sudo chmod 755** didn't work so tried **777** for /opt/ and all folders down to **2018.3** also did a chown -R v2x:v2x /opt/pkg/petalinux


## Other/Useful
* https://ez.analog.com/sw-interface-tools/f/q-a/540822/pyadi-iio-on-zedboard-fmcomms4-eval
* sudo usermod -a -G vboxsf $(whoami) 
  * then log out and back in for shared folder permissions

# Notes on PL to PS Data Transfer Using UDMA/UIO

We intend to not use the current adi dma, and instead add in another DMA, so we still use the defual ADI image, but also have our RX signal processing run at our own clock rate. This should allow us more control on how we want to send data.

## First Steps
* Copied ADI base FPGA project to /capstone/zed/
* Then modified the copy.
* Add DMA block to design (we are only doing RX or PL->PS)
* Created a simple counter block. This block sends a 32 bit value where the upper 16 bits are 0xFEED and the lower 16bits are a count.
* Generate Bitstream and export .hdf
  
## Petalinux Steps
* Make changes to device tree file /peta_adi_proj/project-spec/meta-user/recipes-bsp/device-tree/files/system-user.dtsi

        /include/ "system-conf.dtsi"
        /{
        };

        /*
        * Device tree for petalinux 2018.3
        */

        / {


            chosen {
                bootargs = "console=ttyPS0,115200 consoleblank=0 cma=1024M root=/dev/mmcblk0p2 rootfstype=ext4 rw rootwait earlyprintk uio_pdrv_genirq.of_id=generic-uio";
            };

                udmabuf-tx {
                compatible  = "ikwzm,udmabuf-0.10.a";
                device-name = "udmabuf-tx";
                size = <0x00100000>;
                sync-direction = <1>;
                    };
        
            udmabuf-rx {
                compatible  = "ikwzm,udmabuf-0.10.a";
                device-name = "udmabuf-rx";
                size = <0x00100000>;
                sync-direction = <2>;
                };
        };

        v2x_signal_processing_capstone_rx_dmac{
            compatible = "generic-uio";
        };

* In Petalinux folder run the following
  
    petalinux-config --get-hw-description=~/capstone/zed/fmcomms2_zed.sdk/

* Create the DMA petalinux kernel module

    petalinux-create -t modules -n udmabuf --enable

* Copy udmabuf.c into petalinux project at recipes_modules/udmabuf/files/ This file was obtained from [github.com/ikwzm/udmabuf/](https://raw.githubusercontent.com/ikwzm/udmabuf/v1.4.7/udmabuf.c)

* Now run:
  
        $: petalinux-config -c kernel
            * device drivers -> userspace io -> <*> for uio_pdrv_genirq and uio_dmem_genirq
        $ petalinux-build
        $ petalinux-package --force --boot --fsbl --fpga --u-boot

        $ cd ../images/linux

        $ cp BOOT.BIN /media/adi/BOOT/

        $ cp image.ub /media/adi/BOOT/

* Do a search for udmabuf.ko and uio_pdrv_genirq.ko
* Grab the lib/modules folder from ~/capstone/peta_adi_proj/build/tmp/deploy/images/plnx-zynq7/modules--4.14.tar.gz
  
## Root Filesystem Steps

* Boot the device (note in petalinux you can set the kernel name)
  
         $ sudo cp *.ko /lib/modules/4.14.0-xilinx-v2018.3/kernel/
         $ sudo nano /lib/modules/4.14.0-xilinx-v2018.3/modules.builtin

* add the following to the file (at end) 
        
        /kernel/udmabuf.ko
* note: uio_pdrv_genirq.ko already there.
* Repeat for modules.order

* Boot the device

        $ mkdir -p /lib/modules/$(uname -r)/
        $ cp *.ko /lib/modules/$(uname -r)/
        $ depmod -a

* Got warning so edited the two files by adding the following lines to each file modules.builtin and modules.order
        
        udmabuf.ko
        uio_pdrv_genirq.ko

* Then

        $ depmod -a
        $ modprobe udmabuf
        $ modprobe uio_pdrv_genirq

* Then Run

        $ sudo insmod udmabuf.ko
        $ sudo insmod uio_pdrv_genirq.ko

* Then open file for editing
  
        $ nano /etc/modules

* Add line

        $ uio_pdrv_genirq.ko
        $ udmabuf.ko

* Then reboot

        $ reboot

* Then 
  
        % ls /dev/

Should see udmabuf-rx and udmabuf-tx

**!!! NEED TO FIX why the /etc/modules aren't being loaded at boot time !!!**

* for now after boot do
        
        $ cd /lib/modules/$(uname -r)/
        $ sudo insmod uio_pdrv_genirq.ko
        $ sudo insmod udmabuf.ko
        
## Software Steps
See [uio](../main/src/v2x_sdr_uio.c), [vio](../main/src/v2x_sdr_vio.c), and /dev/mem in our code.
