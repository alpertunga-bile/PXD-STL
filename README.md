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
Test Name : Array Tests
  init with array test           -> Passed
  copy_to test                   -> Passed
  bool compare test              -> Passed
  expand test                    -> Passed
  resize test                    -> Passed
  index test                     -> Passed
---------------------------------------------------------------
Test Name : Linked Link Tests
  normal array init              -> Passed
  negative indexing              -> Passed
  reverse array init             -> Passed
  reverse                        -> Passed
  indexing                       -> Passed
  assign constructor             -> Passed
  move constructor               -> Passed
  copy constructor               -> Passed
  remove_head                    -> Passed
  remove_end                     -> Passed
  remove_between                 -> Passed
---------------------------------------------------------------
```