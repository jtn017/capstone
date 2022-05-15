# Repository Directories
The following table gives a brief summary of the most important top level files and directories:
| Directory | Details                                                    |
| --------- | ---------------------------------------------------------- |
| .vscode   | Contains Visual Studio Code settings for the project       |
| data      | Contains test vectors used in unit testing 				 |
| gen       | Contains the source code generated from Simulink models 	 |
| inc       | Contains the headers of all manually created files         |
| src    	| Contains all manually created source code        			 |

# Testing Setup SW
This directory contains a working setup of the Simulink generated source code.
The main function of this SW is to make sure the code is compilable and to run unit tests against it

## Build instructions
Compilation must be performed on Zedboard as `libiio` will not exist on most machines.
To create the executable, run:
```
make
```

To copy over newly generated Simulink code, run:
```
make mv_gen_code
```
Note that this copied over code will require renaming the `rtConstP` variables as they are not uniquely named
for each TX/RX component built.

To cleanup build artifacts:
```
make clean
```

## Run program
To run the program, use the following commands:
```
sudo ./xcvr_demo
```