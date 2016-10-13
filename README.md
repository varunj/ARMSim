================================================
Functional Simulator for subset of ARM Processor
================================================

README

# Table of contents
1. [Directory Structure] (#directory-structure)
2. [How to Build] (#how-to-build)
3. [How to Execute] (#how-to-execute)
4. [For Windows] (#for-windows)



## Directory Structure
```
CS112-Project
  |
  |- bin
      |
      |- myARMSim
  |- doc
      |
      |- design-doc.docx
  |- include
      |
      |- myARMSim.h
  |- src
      |- main.c
      |- Makefile
      |- myARMSim.h
  |- test
      |- simple_add.mem
      |- fib.mem
      |- array_add.mem
```

## How to Build

For building:
` $cd src

For cleaning the project:
` $cd src
` $make clean


# How to Execute
  edit
        ./myARMSim test/<filename>.mem
  ` $ make

# For Windows
` gcc myARMSim.c main.c -I ../include -w -o ../bin/myARMSim
` cd ..\bin
` myARMSim add.mem
