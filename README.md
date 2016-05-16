# cbitset
[![Build Status](https://travis-ci.org/lemire/cbitset.png)](https://travis-ci.org/lemire/cbitset)

Simple bitset library in C. It includes fast functions
to compute cardinalities, unions, intersections...

Usage in C:
```C
bitset_t * b = bitset_create();
bitset_set(b,10);
bitset_get(b,10);// returns true
```

To run tests:
```bash
make
./unit
```

Prerequisites: GCC-compatible compiler

