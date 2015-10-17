#!/bin/bash
CC=arm-linux-guneaabihf-gcc
$CC  -o list_controls capture.c list_controls.c -I. -std=c11
 arm-linux-gnueabihf-gcc -o list_format capture.c list_format.c -I. -std=c11

###build on debian-armhf
gcc -o capture_jpeg capture.c capture_jpeg.c -I. -std=c99 -ljpeg
