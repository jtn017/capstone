# Repository Directories
The following table gives a brief summary of the most important top level files and directories:
| Directory    | Details                                                    |
| ------------ | ---------------------------------------------------------- |
| [conf](conf) | Contains configuration files for the ZedBoard/FMCOMMS4     |
| [data](data) | Contains sample HUD/audio data used in actual transmission |
| [gen](gen)   | Contains the source code generated from Simulink models 	|
| [inc](inc)   | Contains the headers of all manually created files     	|
| [src](src)   | Contains all manually created source code               	|


# Zedboard SW
This directory contains the ZedBoard SW source files.
The main function of this SW is to generate two threaded processes, 1 for TX and 1 for RX.
The TX thread periodically transmits packets (generated from files in `data`) using the kernel driver code.
The RX thread periodically checks for data in the UIO to process.

## Build instructions
Compilation must be performed on Zedboard as `libiio` will not exist on most machines.
To create the executable, run:

```
make
```

To cleanup build artifacts:

```
make clean
```

## Run program
To run the program, use the following commands:

```
sudo ./xcvr_demo
```
