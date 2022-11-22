# Setting up a development environment
This document discusses how to setup a development environment for building SSP modules.


# General workflow
There are three steps to build SSP modules, which must be completed in order.

a) setup the development environment 
done once, and overed in DEVENV.md

b) download/create project 
done once, per project, and covered in this document.

c) build project
done each time you want to build/create the modules for the SSP


note: you will need to ensure the build tools from the development enviroment are on your path.
(as covered in DEVENV.md)



Note: 
Specific directories and examples are mentioned in these documents.
however, most can be changed to your own requirements, with simple overrides.
but this is not covered extensively here, to keep things clear and simple.


# install Linux development tools 
```
    sudo apt install cmake git llvm clang g++-10-arm-linux-gnueabihf 
```
this is debian package manager, similar with pacman etc on other distros

(although this is v10 of g++ others will likely work)


# install macOS development using homebrew
homebrew is useful package manager for macos for many open source tools.
highly recommended :)  see https://brew.sh



```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
brew install cmake git llvm pkg-config arm-linux-gnueabihf-binutils

```
(some installs may need gcc-arm-embedded)


## mac m1 - homebrew config
place the following in .zshrc
```
export PATH=/opt/homebrew/bin:"${PATH}"
```

## intel macs - homebrew config 
place the following in .zshrc/.bashrc as appropiate.


```
export PATH=/usr/local/bin:"${PATH}"
```




# setup requirements for build

we need a copy of the build root for the SSP to build
this is detailed in this post:
https://forum.percussa.com/t/update-13072022-superbooth-2022-sd-card-image-fixes-for-usb-audio-sample-rate-switching-asio-support/1556

we need to download from :
https://sw13072022.s3.us-west-1.amazonaws.com/arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz


unzip into `~/buildroot` e.g.

```
mkdir ~/buildroot
cd ~/buildroot
mv ~/Downloads/arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz  .
tar xzf arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz
```

decompression (gz) may not be necessary on some OS versions or browsers, as they may automatically unzip.
if its just .tar use.
```
tar xf arm-rockchip-linux-gnueabihf_sdk-buildroot.tar
```

if all is ok, then we will see
```
% ls ~/buildroot/arm-rockchip-linux-gnueabihf_sdk-buildroot/libexec 
awk     c++-analyzer    ccc-analyzer    gcc
```

note: this directory can be overriden using environment var (if you need to..)
```
export BUILDROOT=$HOME/buildroot/ssp/arm-rockchip-linux-gnueabihf_sdk-buildroot
```

### VST SDK

the modules are build as VST3 with an interface that then bridges this to the SSP SDK.
so, we also need to download the VST 3 SDK from steinberg and place into SDKs dirctory
https://www.steinberg.net/developers/


by default build expects the '~/SDKs/'
```
mkdir ~/SDKs
cd ~/SDKs
```
then unzip into this diretory

note: expecting SDK in $HOME/SDKs/vst3-sdk", 

you may need to do something like 

```
unzip VST_SDK.zip
mv VST_SDK/vst3sdk vst3-sdk
```


then in vst3-sdk you wil seen something like
```
% ls ~/SDKs/vst3-sdk 
CMakeLists.txt          VST3_License_Agreement.pdf  bin             index.html          tools
LICENSE.txt         VST3_Usage_Guidelines.pdf   cmake               pluginterfaces          vstgui4
README.md           base                doc             public.sdk
```


note: this directory can be overriden using environment var (if you need to..)
```
export VSTSDK=$HOME/SDKs/vst3-sdk
```








