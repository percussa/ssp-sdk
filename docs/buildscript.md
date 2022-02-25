TEMP FILE - just so I dont forget steps for MAIN build doc


# log on to build machine

```
ssh pi@rpi.local
```

# make sure we have tools we need

```
sudo apt install cmake git llvm clang
```

note: some distros may combine llvm and clang, some may not... 
check clang is present after install!


# setup requirements for build

buildroot
```
mkdir ~/buildroot
cd ~/buildroot
scp  user@host~/Downloads/arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz  .
tar xzf arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz
```

vst sdk (v3)
```
mkdir ~/SDKs
cd ~/SDKs
tar xzf arm-rockchip-linux-gnueabihf_sdk-buildroot.tar.gz
```

note: expecting SDK in $HOME/SDKs/vst3-sdk", though it can be overriden


# build project
now we can finally build the sdk project 

first we need to grab it from git

```
mkdir ~/projects
cd ~/projects
git clone https://github.com/TheTechnobear/ssp-sdk.git
cd ssp-sdk/
git checkout tb-v3
git submodule update --init --recursive
```

now lets build 

```
cd examples
mkdir build 
cd build
export BUILDROOT=~/buildroot/arm-rockchip-linux-gnueabihf_sdk-buildroot
```



create cmake files

```
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake .. 
```

note: if this fails, e.g due to not finding something entire build directory contents before trying again.
as it caches details, and failing to delete will get confusing results ! 


now the actual build 

```
cmake --build . -v -- -j 8
```

resuls are something like: 
./api/test/libtest.so
./QVCA_artefacts/Release/VST3/qvca.vst3/Contents/armv7l-linux/qvca.so



note you could edit examples/CMakeLists if you dont want to build the vst

