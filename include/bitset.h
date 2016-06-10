#ifndef BITSET_H
#define BITSET_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

struct bitset_s {
    uint64_t *array;
    size_t arraysize;
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
size_t bitset_size_in_bytes(const bitset_t *bitset);

/* returns how many bits can be accessed */
size_t bitset_size_in_bits(const bitset_t *bitset);

/* returns how many words (64-bit) of memory the backend buffer uses */
size_t bitset_size_in_words(const bitset_t *bitset);

/* Set the ith bit. Attempts to resize the bitset if needed (may silently fail) */
void bitset_set(bitset_t *bitset,  size_t i );

/* Get the value of the ith bit.  */
bool bitset_get(const bitset_t *bitset,  size_t i );

/* Count number of bit sets.  */
size_t bitset_count(const bitset_t *bitset);

/* compute the union in-place (to b1), returns true if successful */
bool bitset_inplace_union(bitset_t *b1, const bitset_t *b2);

/* compute the intersection in-place (to b1) */
void bitset_inplace_intersection(bitset_t *b1, const bitset_t *b2);

#endif
