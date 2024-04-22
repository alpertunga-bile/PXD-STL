# PXD-STL

Standard library under the PixelDancer package.

## Features

- Data structures from scratch with templates.
- Tests for the data structure functionalities.
- ```SIMDString``` as the default string class.

## Usage

- Clone the repository with ```--recursive``` flag for the ```SIMDString``` package.
- In CMake, add the project as subdirectory. The project is built a library.
- Use the library with the header files under the ```sources``` folder.

## Current Test Results

```
---------------------------------------------------------------
Test Name : Linked Link Tests
  remove_between                 -> Passed
  assign constructor             -> Passed
  remove_end                     -> Passed
  move constructor               -> Passed
  copy constructor               -> Passed
  remove_head                    -> Passed
  indexing                       -> Passed
  negative indexing              -> Passed
  reverse array init             -> Passed
  reverse                        -> Passed
  normal array init              -> Passed
---------------------------------------------------------------
Test Name : Array Tests
  assign constructor             -> Passed
  resize test                    -> Passed
  copy constructor               -> Passed
  index                          -> Passed
  expand                         -> Passed
  bool compare                   -> Passed
  copy_to                        -> Passed
  move constructor               -> Passed
  negative index                 -> Passed
  init with array                -> Passed
---------------------------------------------------------------
```