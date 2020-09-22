# cbitset
[![Build Status](https://travis-ci.org/lemire/cbitset.png)](https://travis-ci.org/lemire/cbitset)

Simple bitset library in C. It includes fast functions
to compute cardinalities, unions, intersections...

- It is tiny: it is made of three files (two header files and one source file).
- It is tested.
- It is fast.

Usage in C:
```C
bitset_t * b = bitset_create();
bitset_set(b,10);
bitset_get(b,10);// returns true
bitset_free(b); // frees memory
```

## CMake

```
mkdir build
cd build
cmake ..
cmake --build . --config Release  
ctest .
```

The cmake build also supports installation. The header files will be installed in a distinct subdirectory (cbitset).


## Old-school Makefiles

To run tests:
```bash
make
./unit
```

## Prerequisites

C11-compatible compiler

