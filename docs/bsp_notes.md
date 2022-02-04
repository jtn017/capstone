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
