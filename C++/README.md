# Hyperspectral Imaging Using HALCON in C++

This package shows how HALCON can be used in C++. Before attempting to try this code, you should already have HALCON installed, this is explained in the README of the HALCON package. This code is generated by writing a program on HDevelop, then converting it using the export tool to a C++ program and then fine tuning it.

## About the package
- 'Easy.exe': Visualises a hyperspectral image     
- 'Medium.exe': Calibrates a hyperspectral image using a dark and white reference, then visualize the image.
- 'Hard.exe': Arranges all lines of the hyperspectral image, calibrates a image using a dark and white reference, then visualize the image. (NOT DONE)

## Prerequisites
- Windows 10 or later
- HALCON license
- C++ Compiler (e.g., GCC, Clang, or MSVC)
- CMake (version 3.10 or higher)

## Setup and Installation

1. Open a terminal and navigate to the package

```bash
    cd .../hyperspectral-imaging/src/C++
```

2. Create a build directory and configure the project

```bash
    mkdir build
    cd build
    cmake ..
```

3. Build the project:
```bash 
cmake --build . --config Debug
```

# Usage
1. Run the startup.exe program from the build/Debug directory

```bash 
./Debug/startup.exe
```

2. To run specific programs individually:

```bash 
./Debug/easy.exe
./Debug/medium.exe
```

## Recommendations
While we don't necessarily recommend HALCON as stated in the HALCON package recommendation, we do recommend doing hyperspectral image processing using C++ since it handles computationally heavy tasks much more efficiently compared to languages like Python. 
This package is a great starting point for understanding the fundamentals of hyperspectral image processing. The hard executable is not yet implemented, providing an opportunity for further development.
