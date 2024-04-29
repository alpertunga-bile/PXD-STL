# PXD-STL

Standard library under the PixelDancer package.

## Features

- Data structures from scratch with templates.
- Tests for the data structure functionalities.
- ```SIMDString``` as the default string class.

## Usage

- Clone the repository with ```--recursive``` flag for the ```SIMDString``` package.
- In CMake, add the project as subdirectory. The project is built a library.
- Link the library with ```pxd-stl``` and use it with the header files which are under the ```sources``` folder.

## Current Test Results

```
---------------------------------------------------------------
Test Name : Array Tests
  init with array                -> Passed
  copy_to                        -> Passed
  resize less                    -> Passed
  resize much                    -> Passed
  bool compare                   -> Passed
  expand                         -> Passed
  negative index                 -> Passed
  index                          -> Passed
  copy constructor               -> Passed
  move constructor               -> Passed
  assign constructor             -> Passed
---------------------------------------------------------------
Test Name : Queue Tests
  array ctor                     -> Passed
  copy ctor                      -> Passed
  move ctor                      -> Passed
  assign ctor                    -> Passed
  peek                           -> Passed
  push                           -> Passed
  reverse                        -> Passed
  pop                            -> Passed
---------------------------------------------------------------
Test Name : Binary Search Tree Tests
  array default ctor             -> Passed
  copy ctor                      -> Passed
  move ctor                      -> Passed
  assign ctor                    -> Passed
  get balanced tree              -> Passed
  is contain                     -> Passed
  min max                        -> Passed
  remove                         -> Passed
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
Test Name : Stack Tests
  array ctor                     -> Passed
  copy ctor                      -> Passed
  move ctor                      -> Passed
  assign ctor                    -> Passed
  peek                           -> Passed
  push                           -> Passed
  reverse                        -> Passed
  pop                            -> Passed
---------------------------------------------------------------
Test Name : Dynamic Array Tests
  assign ctor                    -> Passed
  move ctor                      -> Passed
  copy ctor                      -> Passed
  expand from 0                  -> Passed
  shrink                         -> Passed
  expand with exist              -> Passed
  add                            -> Passed
  index                          -> Passed
  negative index                 -> Passed
  resize                         -> Passed
---------------------------------------------------------------
```