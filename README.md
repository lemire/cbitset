# cbitset
![Ubuntu 20.04 CI (GCC 9)](https://github.com/lemire/cbitset/workflows/Ubuntu%2020.04%20CI%20(GCC%209)/badge.svg)
![MSYS2-CI](https://github.com/lemire/cbitset/workflows/MSYS2-CI/badge.svg)

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

To build the library and run the tests and benchmarks:

```bash
cmake -B build
cmake --build build
ctest --test-dir build
```

### Installing

To install the compiled library, the headers and the CMake package files (by
default under `/usr/local`):

```bash
cmake -B build
cmake --build build
cmake --install build
```

You can choose a different location with `--prefix`:

```bash
cmake --install build --prefix /path/to/install
```

The header files are installed in a distinct subdirectory (`cbitset`), so you
include them with:

```C
#include <cbitset/bitset.h>
```

### Using cbitset from another CMake project

Once installed, cbitset exports a CMake package. You locate it with
`find_package` and link against the `cbitset::cbitset` target:

```cmake
find_package(cbitset REQUIRED)
add_executable(myapp main.c)
target_link_libraries(myapp PRIVATE cbitset::cbitset)
```

If cbitset is not on the default search path, point CMake at the install
location with `-DCMAKE_PREFIX_PATH=/path/to/install`.

You can also embed cbitset directly in a larger project without installing it,
either via `add_subdirectory` or with `FetchContent`:

```cmake
include(FetchContent)
FetchContent_Declare(cbitset
  GIT_REPOSITORY https://github.com/lemire/cbitset.git
  GIT_TAG master)
FetchContent_MakeAvailable(cbitset)

target_link_libraries(myapp PRIVATE cbitset::cbitset)
```

When cbitset is consumed this way, its tests and benchmarks are skipped by
default. They can be toggled explicitly with `-DCBITSET_BUILD_TESTS=OFF` and
`-DCBITSET_BUILD_BENCHMARKS=OFF`.


## Old-school Makefiles

To run tests:
```bash
make
./unit
```

## Prerequisites

C11-compatible compiler.

Visual Studio now supports the [C11 and C17 standards](https://devblogs.microsoft.com/cppblog/c11-and-c17-standard-support-arriving-in-msvc/).

