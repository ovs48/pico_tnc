# PICO TNC

PICO TNC is the Terminal Node Controler for Amateur Packet Radio powered by Raspberry Pi Pico.

This TNC has same functionality as WB8WGA's PIC TNC.

## PIC TNC features

- Encode and decode Bell 202 AFSK signal without modem chip
- Digipeat UI packet up to 1024 byte length
- Send beacon packet
- Support converse mode
- Support GPS tracker feature
- Support both USB serial and UART serial interface

## Additional features

- Support KISS mode
- Support multi-port up to 3 ports

## How to build

### Prerequisites

You need to install PicoSDK and if you are using Windows several toolchains.
Linux Users can use this guide:
```
https://lindevs.com/set-up-raspberry-pi-pico-sdk-on-ubuntu/
```
Windows Users can use this guide:
```
https://shawnhymel.com/2096/how-to-set-up-raspberry-pi-pico-c-c-toolchain-on-windows-with-vs-code/

If you just want to compile this code you only have to prepare the following points if you haven't already:
  1. Directory Setup
  2. Install GNU Arm Embedded Toolchain
  3. Install MinGW-w64
  4. Install CMake
  6. Install Git
  7. Download Pico SDK and Examples
  8. Update Environment Variables
  10. Build Blink Example (which covers configuring GIT shell for compiling and important parameters)
```

### Building

```
git clone https://github.com/ovs48/pico_tnc.git
cd pico_tnc
git submodule init
git submodule update
mkdir build
cd build

#LINUX USERS USE
cmake ..

#WINDOWS USERS USE (See the guide above for more)
cmake -G "<Path To MinGW Files>" ..

make -j4
(flash 'pico_tnc/pico_tnc.uf2' file to your Pico)
```
![bell202-wave](bell202-wave.png)
![command line](command.png)
[![schemantic](schematic.jpg)](schematic.png)
