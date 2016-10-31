
Functional Simulator for subset of ARM Processor
================================================

# Table of contents
1. [Getting Started] (#getting-started)
2. [Directory Structure] (#directory-structure)
3. [How to Build] (#how-to-build)
4. [How to Execute] (#how-to-execute)
5. [For Windows] (#for-windows)

## Getting Started

Clone project:
```
$ git clone https://github.com/varunj/ARMSim
```

## Directory Structure
```
CS112-Project
  |
  |- bin
      |
      |- data_out.mem
  |- doc
      |
      |- design-doc.pdf
  |- include
      |
      |- myARMSim.h
  |- src
      |- main.c
      |- Makefile
      |- myARMSim.h
  |- test
      |- array_add.s
      |- arrSum.mem
      |- fib.mem
      |- fibo.s
      |- simple_add.mem
      |- simple_add_out.mem
```

## How to Build

For building:  
```
$ cd src
```

For cleaning the project:  
```
$ cd src
$ make clean
```


## How to Execute
edit
  ./myARMSim test/<filename>.mem  
```
$ make
```

## For Windows
```
gcc myARMSim.c main.c -I ../include -w -o ../bin/myARMSim
cd ..\bin
myARMSim add.mem
```
