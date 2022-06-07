# UDP Server
Small utility program that listens to UDP packets on port 15050. Program then forwards the packets to the ESP8266 Wifi Module with IP address specified in the file:
`inc/defines.h`. Packet forwarding is turned off by default, but can be enabled by removing the comment in the preprocessing macro
```
#define HTTP_SOCKET
```

Program should be run in conjunction with **xcvr_demo** program in directory `capstone/main`.

## Software Dependencies
Make sure you have the following dependencies installed on your system (these are the same dependencies as the ones specified in the root README.md file):
```
sudo apt-get install -y libcurl4-openssl-dev libevent-dev gdb
```

## Building and Running UDP server
To build:
```
make
```

To run program:
```
make run
```
