# PXD-STL

Standard library under the PixelDancer package.

## Features

- Includes tests for the data structure functionalities.
- ```SIMDString``` as the default string class.
- ```RE2``` library is used as the regex engine.

## Usage

- Clone the repository with ```--recursive``` flag.
- In CMake, add the project as subdirectory. The project is built a library.
- Link the library with ```pxd-stl``` and use it with the header files which are under the ```includes``` folder.

## TODO

- [ ] Undirectional Graph
- [ ] Directional Graph
- [ ] Octree with Point
- [ ] Octree with AABB
- [ ] K-d tree
- [ ] Red-black tree
- [ ] Segment tree
- [ ] Self organizing list
- [ ] Skip list
- [ ] Unrolled linked list

## Current Test Results

- You can check the current test results from the ```test_results.txt``` file.
