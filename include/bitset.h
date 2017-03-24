#ifndef BITSET_H
#define BITSET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "portability.h"

struct bitset_s {
    uint64_t * restrict array;
    size_t arraysize;
    size_t capacity;

};

typedef struct bitset_s bitset_t;

/* Create a new bitset. Return NULL in case of failure. */
bitset_t *bitset_create();

/* Create a new bitset able to contain size bits. Return NULL in case of failure. */
bitset_t *bitset_create_with_capacity( size_t size );

/* Free memory. */
void bitset_free(bitset_t *bitset);

/* Set all bits to zero. */
void bitset_clear(bitset_t *bitset);

/* Create a copy */
bitset_t * bitset_copy(const bitset_t *bitset);

/* Resize the bitset. Return true in case of success, false for failure. Pad with zeroes new buffer areas if requested. */
bool bitset_resize( bitset_t *bitset,  size_t newarraysize, bool padwithzeroes );

/* returns how many bytes of memory the backend buffer uses */
static inline size_t bitset_size_in_bytes(const bitset_t *bitset) {
  return bitset->arraysize*sizeof(uint64_t);
}

/* returns how many bits can be accessed */
static inline size_t bitset_size_in_bits(const bitset_t *bitset) {
  return bitset->arraysize * 64;
}

/* returns how many words (64-bit) of memory the backend buffer uses */
static inline size_t bitset_size_in_words(const bitset_t *bitset) {
  return bitset->arraysize;
}


/* Grow the bitset so that it can support newarraysize * 64 bits with padding. Return true in case of success, false for failure. */
static inline bool bitset_grow( bitset_t *bitset,  size_t newarraysize ) {
  if (bitset->capacity < newarraysize) {
    uint64_t *newarray;
    bitset->capacity = newarraysize * 2;
    if ((newarray = (uint64_t *) realloc(bitset->array, sizeof(uint64_t) * bitset->capacity)) == NULL) {
      free(bitset->array);
      return false;
    }
    bitset->array = newarray;
  }
  memset(bitset->array + bitset->arraysize ,0,sizeof(uint64_t) * (newarraysize - bitset->arraysize));
  bitset->arraysize = newarraysize;
  return true; // success!
}


/* Set the ith bit. Attempts to resize the bitset if needed (may silently fail) */
static inline void bitset_set(bitset_t *bitset,  size_t i ) {
  size_t shiftedi = i >> 6;
  if (shiftedi >= bitset->arraysize) {
    if( ! bitset_grow(bitset,  shiftedi + 1) ) {
        return;
    }
  }
  bitset->array[shiftedi] |= ((uint64_t)1) << (i % 64);
}

/* Get the value of the ith bit.  */
static inline bool bitset_get(const bitset_t *bitset,  size_t i ) {
  size_t shiftedi = i >> 6;
  if (shiftedi >= bitset->arraysize) {
    return false;
  }
  return ( bitset->array[shiftedi] & ( ((uint64_t)1) << (i % 64))) != 0 ;
}

/* Count number of bit sets.  */
size_t bitset_count(const bitset_t *bitset);

/* Find the index of the first bit set.  */
size_t bitset_minimum(const bitset_t *bitset);

/* Find the index of the last bit set.  */
size_t bitset_maximum(const bitset_t *bitset);


/* compute the union in-place (to b1), returns true if successful, to generate a new bitset first call bitset_copy */
bool bitset_inplace_union(bitset_t * restrict b1, const bitset_t * restrict b2);

/* report the size of the union (without materializing it) */
size_t bitset_union_count(const bitset_t * restrict b1, const bitset_t * restrict b2);

/* compute the intersection in-place (to b1), to generate a new bitset first call bitset_copy */
void bitset_inplace_intersection(bitset_t * restrict b1, const bitset_t * restrict b2);

/* report the size of the intersection (without materializing it) */
size_t bitset_intersection_count(const bitset_t * restrict b1, const bitset_t * restrict b2);


/* compute the difference in-place (to b1), to generate a new bitset first call bitset_copy */
void bitset_inplace_difference(bitset_t * restrict b1, const bitset_t * restrict b2);

/* compute the size of the difference */
size_t  bitset_difference_count(const bitset_t *restrict b1, const bitset_t * restrict b2) ;

/* compute the symmetric difference in-place (to b1), return true if successful, to generate a new bitset first call bitset_copy */
bool bitset_inplace_symmetric_difference(bitset_t * restrict b1, const bitset_t * restrict b2);

/* compute the size of the symmetric difference  */
size_t  bitset_symmetric_difference_count(const bitset_t *restrict b1, const bitset_t * restrict b2);

/* iterate over the set bits
 like so :
  for(size_t i = 0; nextSetBit(b,&i) ; i++) {
    //.....
  }
  */
static inline bool nextSetBit(const bitset_t *bitset, size_t *i) {
      size_t x = *i >> 6;
      if (x >= bitset->arraysize) {
        return false;
      }
      uint64_t w = bitset->array[x];
      w >>= (*i & 63);
      if (w != 0) {
        *i += __builtin_ctzll(w);
        return true;
      }
      x ++;
      while (x < bitset->arraysize) {
        w = bitset->array[x];
        if (w != 0) {
          *i = x * 64 + __builtin_ctzll(w);
          return true;
        }
        x ++;
      }
      return false;
}

static inline void bitset_print(const bitset_t *b) {
  printf("{");
  for(size_t i = 0; nextSetBit(b,&i) ; i++) {
    printf("%zu, ",i);
  }
  printf("}");
}

#endif
