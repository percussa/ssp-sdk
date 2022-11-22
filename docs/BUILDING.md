# building 
This document covers creating/downloading a project , and then building it for the SSP.


# general workflow
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



# B) download/create project

obviously we need to have some code to create a module :)
either we are going to create our own new code, or download an existing project.

lets cover the first.


notes:
Im not going to cover in any detail, the following   
- git, there are many guides on this
- c++ programming, dsp programming and related
(Id recommend the juce docs for a good starter here)



## i) download project
we use github general to store projects, and this comes with a URL 

first Id recomment you store all dev projects in one place..


an example might be: 

e.g. 
```
mkdir ~/projects
cd ~/projects
```



### ii) using the ssp-sdk examples

```
cd ~/projects
git clone https://github.com/percussa/ssp-sdk.git
cd ssp-sdk/
git submodule update --init --recursive
```

ok, congratulations... now we are all set to create our first module!

### creating a new module
I would recommend using the ssp-sdk examples, as a 'template' for your own projects.
essentially you can just copy the examples directory, and modify to your needs.

doing so is beyond the scope of this documents, since you will soon need to understand more complex topics, 
like cmake, build systems, c++ etc...


if you are a non-developer wanting to create modules, you may want to look at 

Important note: if you base of the VST/JUCE example, you need to review licencing of VST3/JUCE.
the 'raw' ssp sdk example has no licensing requirements.


# C) building project 

there are really two steps to building a project 

first we need to prepare the projects build setup (aka makefiles), this is done once
then we need to compile


## create build setup

assuming we are using the ssp-sdk examples as above
so are in `~/projects/ssp-sdk`

```
cd examples
mkdir build 
cd build 
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../xcSSP.cmake .. 
```



## compiling
the moment we have been building up to...
actually compiling the module! 

```
cmake --build . -- -j 4 
```

voila, we have build the modules

you will see that whilst building, it reports the targets... 
aka, the modules that you are going to want to copy to your SSP 

e.g.
```
./api/test/libtest.so
./QVCA_artefacts/Release/VST3/qvca.vst3/Contents/armv7l-linux/qvca.so
```


note: 
you can see that QVCA is reported as a VST... and indeed it is.
however, in reality it **also** supports the SSP SDK API, which is how the SSP actually uses it.

being a vst3, is useful to support other platforms, in particular for testing/development on your desktop.
(this is beyond the scope of this document)





