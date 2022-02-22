#### WORK IN PROGRESS ### 


# Building examples

## prerequisites 

- clone this repository locally 
- download the ssp buildroot 
- ensure required dev software is installed 
-- llvm (includes clang)
-- cmake
-- git (optional)  
-- vst3 sdk

TO DO : clarify these steps 


($ENV{HOME}/SDKs/Steinberg/VST3/VST3 SDK) 

## build environment (short explaination)

## TODO : talk about using llvm as cross compiler and the role of build root ## 


## building 

```
cd examples
mkdir build 
cd build 
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake .. 
cmake --build . -- -j 8 
```

### TODO : explain above what is it doing ## 

## installing/testing on SSP



# next steps

## goals/expections 

what are we doing here - how to create a vst using example as a starting point.
what we are NOT doing here - teaching DSP/C++ ;) 


## building your own VST for the SSP.

### TODO ###
basically go thru copying example into new directory, 
changing cmake etc to have a new name

bits of code that will need changing and where
- your own parameters
- your own IO
- designing your own UI 
- implement your own DSP 


## tips on developing 
build on your desktop test as vst

using AudioPluginHost (juce) and why


