# cbitset
[![Build Status](https://travis-ci.org/lemire/cbitset.png)](https://travis-ci.org/lemire/cbitset)
![Ubuntu 20.04 CI (GCC 9)](https://github.com/lemire/cbitset/workflows/Ubuntu%2020.04%20CI%20(GCC%209)/badge.svg)
![Ubuntu 18.04 CI (GCC 7)](https://github.com/lemire/cbitset/workflows/Ubuntu%2018.04%20CI%20(GCC%207)/badge.svg)
![MSYS2-CI](https://github.com/lemire/cbitset/workflows/MSYS2-CI/badge.svg)
![Visual Studio-CI](https://github.com/lemire/cbitset/workflows/VS16-CI/badge.svg)

Simple bitset library in C. It includes fast functions
to compute cardinalities, unions, intersections...

- It is tiny: it is made of three files (two header files and one source file).
- It is tested.
- It is fast.
- It is straight C.

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

C11-compatible compiler.

Visual Studio now supports the [C11 and C17 standards](https://devblogs.microsoft.com/cppblog/c11-and-c17-standard-support-arriving-in-msvc/).

