#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <stdint.h>
#include <string.h>

#include "bitset.h"

/* Create a new bitset. Return NULL in case of failure. */
bitset_t *bitset_create() {
  bitset_t *bitset = NULL;
  /* Allocate the bitset itself. */
  if( ( bitset = (bitset_t*) malloc( sizeof( bitset_t ) ) ) == NULL ) {
      return NULL;
  }
  bitset->array = NULL;
  bitset->arraysize = 0;
  bitset->capacity = 0;
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
  bitset->capacity = bitset->arraysize;
  if ((bitset->array = (uint64_t *) malloc(sizeof(uint64_t) * bitset->arraysize)) == NULL) {
    free( bitset);
    return NULL;
  }
  memset(bitset->array,0,sizeof(uint64_t) * bitset->arraysize);
  return bitset;
}

/* Create a copy */
bitset_t *bitset_copy( const bitset_t * bitset ) {
  bitset_t *copy = NULL;
  /* Allocate the bitset itself. */
  if( ( copy = (bitset_t*) malloc( sizeof( bitset_t ) ) ) == NULL ) {
      return NULL;
  }
  memcpy(copy,bitset,sizeof(bitset_t));
  copy->capacity = copy->arraysize;
  if ((copy->array = (uint64_t *) malloc(sizeof(uint64_t) * bitset->arraysize)) == NULL) {
    free(copy);
    return NULL;
  }
  memcpy(copy->array,bitset->array,sizeof(uint64_t) * bitset->arraysize);
  return copy;
}

void bitset_clear(bitset_t *bitset) {
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
  if (bitset->capacity < newarraysize) {
    uint64_t *newarray;
    bitset->capacity = newarraysize * 2;
    if ((newarray = (uint64_t *) realloc(bitset->array, sizeof(uint64_t) * bitset->capacity)) == NULL) {
      free(bitset->array);
      return false;
    }
    bitset->array = newarray;
  }
  if (padwithzeroes && (newarraysize > smallest))
    memset(bitset->array + smallest,0,sizeof(uint64_t) * (newarraysize - smallest));
  bitset->arraysize = newarraysize;
  return true; // success!
}



size_t bitset_count(const bitset_t *bitset) {
    size_t card = 0;
    size_t k = 0;
    // assumes that long long is 8 bytes
    for(; k + 7 < bitset->arraysize; k+=8) {
        card += __builtin_popcountll(bitset->array[k]);
        card += __builtin_popcountll(bitset->array[k+1]);
        card += __builtin_popcountll(bitset->array[k+2]);
        card += __builtin_popcountll(bitset->array[k+3]);
        card += __builtin_popcountll(bitset->array[k+4]);
        card += __builtin_popcountll(bitset->array[k+5]);
        card += __builtin_popcountll(bitset->array[k+6]);
        card += __builtin_popcountll(bitset->array[k+7]);
    }
    for(; k + 3 < bitset->arraysize; k+=4) {
        card += __builtin_popcountll(bitset->array[k]);
        card += __builtin_popcountll(bitset->array[k+1]);
        card += __builtin_popcountll(bitset->array[k+2]);
        card += __builtin_popcountll(bitset->array[k+3]);
    }
    for(; k < bitset->arraysize; k++) {
        card += __builtin_popcountll(bitset->array[k]);
    }
    return card;
}


bool bitset_inplace_union(bitset_t * restrict b1, const bitset_t * restrict b2) {
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

size_t bitset_minimum(const bitset_t *bitset) {
  for(size_t k = 0; k < bitset->arraysize; k++) {
    uint64_t w = bitset->array[k];
    if ( w != 0 ) {
      return __builtin_ctzll(w) + k * 64;
    }
  }
  return 0;
}

size_t bitset_maximum(const bitset_t *bitset) {
  for(size_t k = bitset->arraysize ; k > 0 ; k--) {
    uint64_t w = bitset->array[k - 1];
    if ( w != 0 ) {
      return  63 - __builtin_clzll(w) + (k - 1) * 64;
    }
  }
  return 0;
}

size_t bitset_union_count(const bitset_t *restrict b1, const bitset_t * restrict b2) {
  size_t answer = 0;
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  for( ; k + 3 < minlength; k += 4) {
    answer += __builtin_popcountll ( b1->array[k] | b2->array[k]);
    answer += __builtin_popcountll ( b1->array[k+1] | b2->array[k+1]);
    answer += __builtin_popcountll ( b1->array[k+2] | b2->array[k+2]);
    answer += __builtin_popcountll ( b1->array[k+3] | b2->array[k+3]);
  }
  for( ; k < minlength; ++k) {
    answer += __builtin_popcountll ( b1->array[k] | b2->array[k]);
  }
  if(b2->arraysize > b1->arraysize) {
    //k = b1->arraysize;
    for(; k + 3 < b2->arraysize; k+=4) {
      answer += __builtin_popcountll (b2->array[k]);
      answer += __builtin_popcountll (b2->array[k+1]);
      answer += __builtin_popcountll (b2->array[k+2]);
      answer += __builtin_popcountll (b2->array[k+3]);
    }
    for(; k < b2->arraysize; ++k) {
      answer += __builtin_popcountll (b2->array[k]);
    }
  } else {
    //k = b2->arraysize;
    for(; k  + 3 < b1->arraysize; k+=4) {
      answer += __builtin_popcountll (b1->array[k]);
      answer += __builtin_popcountll (b1->array[k+1]);
      answer += __builtin_popcountll (b1->array[k+2]);
      answer += __builtin_popcountll (b1->array[k+3]);
    }
    for(; k < b1->arraysize; ++k) {
      answer += __builtin_popcountll (b1->array[k]);
    }
  }
  return answer;
}

void bitset_inplace_intersection(bitset_t * restrict b1, const bitset_t * restrict b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  for( ; k < minlength; ++k) {
    b1->array[k] &= b2->array[k];
  }
  for(; k < b1->arraysize; ++k) {
    b1->array[k] = 0; // memset could, maybe, be a tiny bit faster
  }
}

size_t bitset_intersection_count(const bitset_t * restrict b1, const bitset_t * restrict b2) {
  size_t answer = 0;
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  for(size_t k = 0 ; k < minlength; ++k) {
    answer += __builtin_popcountll ( b1->array[k] & b2->array[k]);
  }
  return answer;
}

void bitset_inplace_difference(bitset_t *restrict b1, const bitset_t * restrict b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  for( ; k < minlength; ++k) {
    b1->array[k] &= ~ (b2->array[k]);
  }
}


size_t  bitset_difference_count(const bitset_t *restrict b1, const bitset_t * restrict b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  size_t answer = 0;
  for( ; k < minlength; ++k) {
    answer += __builtin_popcountll (b1->array[k] & ~ (b2->array[k]));
  }
  for( ; k < b1->arraysize ; ++k) {
    answer += __builtin_popcountll (b1->array[k]);
  }
  return answer;
}

bool bitset_inplace_symmetric_difference(bitset_t *restrict b1, const bitset_t * restrict b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  for( ; k < minlength; ++k) {
    b1->array[k] ^= b2->array[k];
  }
  if(b2->arraysize > b1->arraysize) {
     size_t oldsize = b1->arraysize;
     if(!bitset_resize( b1, b2->arraysize, false)) return false;
     memcpy(b1->array + oldsize, b2->array + oldsize, (b2->arraysize - oldsize) * sizeof(uint64_t));
  }
  return true;
}

size_t  bitset_symmetric_difference_count(const bitset_t *restrict b1, const bitset_t * restrict b2) {
  size_t minlength = b1->arraysize < b2->arraysize ? b1->arraysize : b2->arraysize;
  size_t k = 0;
  size_t answer = 0;
  for( ; k < minlength; ++k) {
    answer += __builtin_popcountll(b1->array[k] ^ b2->array[k]);
  }
  if(b2->arraysize > b1->arraysize) {
    for( ; k < b2->arraysize; ++k) {
      answer += __builtin_popcountll(b2->array[k]);
    }
  } else {
    for( ; k < b1->arraysize; ++k) {
      answer += __builtin_popcountll(b1->array[k]);
    }
  }
  return answer;
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

bool bitset_trim(bitset_t * bitset) {
  size_t newsize = bitset->arraysize;
  while(newsize > 0) {
    if(bitset->array[newsize - 1] == 0) newsize -= 1;
  }
  if(bitset->capacity == newsize) return true; // nothing to do
  bitset->capacity = newsize;
  bitset->arraysize = newsize;
  uint64_t *newarray;
  if ((newarray = (uint64_t *) realloc(bitset->array, sizeof(uint64_t) * bitset->capacity)) == NULL) {
      free(bitset->array);
      return false;
  }
  bitset->array = newarray;
  return true;
}
