# PXD-STL

Standard library under the PixelDancer package.

## Features

- Data structures from scratch with templates.
- Tests for the data structure functionalities.
- ```SIMDString``` as the default string class.
- RE2 library is used as regex engine.

## Usage

- Clone the repository with ```--recursive``` flag for the ```SIMDString``` package.
- In CMake, add the project as subdirectory. The project is built a library.
- Link the library with ```pxd-stl``` and use it with the header files which are under the ```sources``` folder.

## Current Test Results

- You can check the current test results from the ```test_results.txt``` file.
