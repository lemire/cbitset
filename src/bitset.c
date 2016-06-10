#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <string.h>

#include "bitset.h"

#if __SIZEOF_LONG__ == 4
#error This code assumes  64-bit longs (by use of the GCC intrinsics). Your system is not currently supported.
#endif

/* Create a new bitset. Return NULL in case of failure. */
bitset_t *bitset_create() {
  bitset_t *bitset = NULL;
  /* Allocate the bitset itself. */
  if( ( bitset = (bitset_t*) malloc( sizeof( bitset_t ) ) ) == NULL ) {
      return NULL;
  }
  bitset->array = NULL;
  bitset->arraysize = 0;
  return bitset;
}

/* Create a new bitset able to contain size bits. Return NULL in case of failure. */
bitset_t *bitset_create_with_capacity( size_t size ) {
  bitset_t *bitset = NULL;
  /* Allocate the bitset itself. */
  if( ( bitset = (bitset_t*) malloc( sizeof( bitset_t ) ) ) == NULL ) {
      return NULL;
  }
  bitset->arraysize = (size + sizeof(uint64_t) * 8 - 1) / (sizeof(uint64_t) * 8);
  if ((bitset->array = (uint64_t *) malloc(sizeof(uint64_t) * bitset->arraysize)) == NULL) {
    free( bitset);
    return NULL;
  }
  memset(bitset->array,0,sizeof(uint64_t) * bitset->arraysize);
  return bitset;
}

/* Create a copy */
bitset_t *bitset_copy( const bitset_t *bitset ) {
  bitset_t *copy = NULL;
  /* Allocate the bitset itself. */
  if( ( copy = (bitset_t*) malloc( sizeof( bitset_t ) ) ) == NULL ) {
      return NULL;
  }
  memcpy(copy,bitset,sizeof(bitset_t));
  if ((copy->array = (uint64_t *) malloc(sizeof(uint64_t) * bitset->arraysize)) == NULL) {
    free(copy);
    return NULL;
  }
  memcpy(copy->array,bitset->array,sizeof(uint64_t) * bitset->arraysize);
  return copy;
}

void clear(bitset_t *bitset) {
  memset(bitset->array,0,sizeof(uint64_t) * bitset->arraysize);
}

/* Free memory. */
void bitset_free(bitset_t *bitset) {
  free(bitset->array);
  free(bitset);
}
/* Resize the bitset so that it can support newarraysize * 64 bits. Return true in case of success, false for failure. */
bool bitset_resize( bitset_t *bitset,  size_t newarraysize, bool padwithzeroes ) {
  size_t smallest = newarraysize < bitset->arraysize ? newarraysize : bitset->arraysize;
  uint64_t *newarray;
  if ((newarray = (uint64_t *) realloc(bitset->array, sizeof(uint64_t) * newarraysize)) == NULL) {
    free(bitset->array);
    return false;
  }
  bitset->array = newarray;
  if (padwithzeroes && (newarraysize > smallest))
    memset(bitset->array + smallest,0,sizeof(uint64_t) * (newarraysize - smallest));
  bitset->arraysize = newarraysize;
  return true; // success!
}


size_t bitset_size_in_bits(const bitset_t *bitset) {
  return bitset->arraysize * 64;
}

size_t bitset_size_in_words(const bitset_t *bitset) {
  return bitset->arraysize;
}


size_t bitset_size_in_bytes(const bitset_t *bitset) {
  return bitset->arraysize*sizeof(uint64_t);
}


/* Set the ith bit. Attempts to resize the bitset if needed (may silently fail) */
void bitset_set(bitset_t *bitset,  size_t i ) {
  if ((i >> 6) >= bitset->arraysize) {
    size_t whatisneeded = ((i+64)>>6);
    if( ! bitset_resize(bitset,  whatisneeded, true) ) {
        return;
    }
  }
  bitset->array[i >> 6] |= ((uint64_t)1) << (i % 64);
}

/* Get the value of the ith bit.  */
bool bitset_get(const bitset_t *bitset,  size_t i ) {
  if ((i >> 6) >= bitset->arraysize) {
    return false;
  }
  uint64_t w = bitset->array[i >> 6];
  return ( w & ( ((uint64_t)1) << (i % 64))) != 0 ;
}

size_t bitset_count(const bitset_t *bitset) {
    size_t card = 0;
    size_t k =0;
    for(; k + 7 < bitset->arraysize; k+=8) {
        card += __builtin_popcountl(bitset->array[k]);
        card += __builtin_popcountl(bitset->array[k+1]);
        card += __builtin_popcountl(bitset->array[k+2]);
        card += __builtin_popcountl(bitset->array[k+3]);
        card += __builtin_popcountl(bitset->array[k+4]);
        card += __builtin_popcountl(bitset->array[k+5]);
        card += __builtin_popcountl(bitset->array[k+6]);
        card += __builtin_popcountl(bitset->array[k+7]);
    }
    for(; k + 3 < bitset->arraysize; k+=4) {
        card += __builtin_popcountl(bitset->array[k]);
        card += __builtin_popcountl(bitset->array[k+1]);
        card += __builtin_popcountl(bitset->array[k+2]);
        card += __builtin_popcountl(bitset->array[k+3]);
    }
    for(; k < bitset->arraysize; k++) {
        card += __builtin_popcountl(bitset->array[k]);
    }
    return card;
}


bool bitset_inplace_union(bitset_t *b1, const bitset_t *b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  for(size_t k = 0 ; k < minlength; ++k) {
    b1->array[k] |= b2->array[k];
  }
  if(b2->arraysize > b1->arraysize) {
     size_t oldsize = b1->arraysize;
     if(!bitset_resize( b1, b2->arraysize, false)) return false;
     memcpy(b1->array + oldsize, b2->array + oldsize, (b2->arraysize - oldsize) * sizeof(uint64_t));
  }
  return true;
}

void bitset_inplace_intersection(bitset_t *b1, const bitset_t *b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  for( ; k < minlength; ++k) {
    b1->array[k] &= b2->array[k];
  }
  for(; k < b1->arraysize; ++k) {
    b1->array[k] = 0; // memset could, maybe, be a tiny bit faster
  }
}
