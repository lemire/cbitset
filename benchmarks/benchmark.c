#include "bitset.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define RDTSC_START(cycles)                                                    \
  do {                                                                         \
    (cycles) = clock();                                                        \
  } while (0)

#define RDTSC_FINAL(cycles)                                                    \
  do {                                                                         \
    (cycles) = clock();                                                        \
  } while (0)

#define BEST_TIME(test, repeat, size)                                          \
  do {                                                                         \
    printf("%s: ", #test);                                                     \
    fflush(NULL);                                                              \
    uint64_t cycles_start, cycles_final, cycles_diff;                          \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      RDTSC_START(cycles_start);                                               \
      test;                                                                    \
      RDTSC_FINAL(cycles_final);                                               \
      cycles_diff = (cycles_final - cycles_start);                             \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = (uint64_t)size;                                               \
    double cycle_per_op =                                                      \
        (min_diff)*1000000000. / ((double)S * CLOCKS_PER_SEC);                 \
    printf(" %.2f ns per operation", cycle_per_op);                            \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

void benchmark_inplace_intersection() {
  bitset_t *b1 = bitset_create();
  bitset_t *b2 = bitset_create();
  uint32_t size = 1000 * 64; // 64 words

  for (uint32_t k = 0; k < size; k += 3) {
    bitset_set(b1, k);
  }

  for (uint32_t k = 0; k < size; k += 5) {
    bitset_set(b2, k);
  }

  BEST_TIME(bitset_inplace_intersection(b1, b2), 500, size / 64);
}

int main() { benchmark_inplace_intersection(); }
