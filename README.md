# cbitset
![Ubuntu 20.04 CI (GCC 9)](https://github.com/lemire/cbitset/workflows/Ubuntu%2020.04%20CI%20(GCC%209)/badge.svg)
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

Advanced example:


```C
    bitset_t *b = bitset_create();
    for (int k = 0; k < 1000; ++k) {
        bitset_set(b, 3 * k);
    }
    // We have bitset_count(b) == 1000.
    // We have bitset_get(b, 3) is true
    // You can iterate through the values:
    size_t k = 0;
    for (size_t i = 0; bitset_next_set_bit(b, &i); i++) {
        // You will have i == k
        k += 3;
    }
    // We support a wide range of operations on two bitsets such as
    // bitset_inplace_symmetric_difference(b1,b2);
    // bitset_inplace_symmetric_difference(b1,b2);
    // bitset_inplace_difference(b1,b2);// should make no difference
    // bitset_inplace_union(b1,b2);
    // bitset_inplace_intersection(b1,b2);
    // bitsets_disjoint
    // bitsets_intersect
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

