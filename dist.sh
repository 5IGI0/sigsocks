#!/bin/bash
# sudo apt install gcc-arm-linux-gnueabi
CC="gcc -m64 " TARGET_EXEC="x86_64-linux-gnu" make
CC="gcc -m32 " TARGET_EXEC="i386-linux-gnu" make
CC="arm-linux-gnueabi-gcc" make