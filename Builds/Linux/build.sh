#!/bin/sh

export BUILDROOT_HOME=/home/bschiett/buildroot
export SYSROOT=$BUILDROOT_HOME/output/staging
export PATH=$PATH:$BUILDROOT_HOME/output/host/usr/bin

export CFLAGS="--sysroot=$SYSROOT -fdiagnostics-color=always"
export CFLAGS="$CFLAGS -DJUCE_CHECK_MEMORY_LEAKS=0" 

export CONFIG=$1 
export SYNTHORVER=$2
export TARGET_ARCH="-mcpu=cortex-a17 -mfloat-abi=hard -mfpu=neon-vfpv4" 

make CC=arm-linux-gcc CXX=arm-linux-g++ -j64 

