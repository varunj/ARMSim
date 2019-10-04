
Functional Simulator for subset of ARM Processor
================================================

# Table of contents
1. [Directory Structure](#directory-structure)
2. [How to Build](#how-to-build)
3. [How to Execute](#how-to-execute)
4. [For Windows](#for-windows)



## <a name="directory-structure">Directory Structure</a>
```
CS112-Project
  |
  |- bin
  |   |
  |   |- myARMSim
  |- doc
  |   |
  |   |- design-doc.docx
  |- include
  |   |
  |   |- myARMSim.h
  |- src
  |   |- main.c
  |   |- Makefile
  |   |- myARMSim.h
  |- test
  |   |- simple_add.mem
  |   |- fib.mem
  |   |- array_add.mem
```

## <a name="how-to-build">How to Build</a>

For building:  
```
$ cd src
```

For cleaning the project:  
```
$ cd src
$ make clean
```


## <a name="how-to-execute">How to Execute</a>
edit
  ./myARMSim test/<filename>.mem  
```
$ make
```

## <a name="for-windows">For Windows</a>
```
gcc myARMSim.c main.c -I ../include -w -o ../bin/myARMSim
cd ..\bin
myARMSim add.mem
```
