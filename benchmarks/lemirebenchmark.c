#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include "bitset.h"


#define BEST_TIME(test,  repeat)                         \
    do {                                                              \
        printf("%s: ", #test);                                        \
        fflush(NULL);                                                 \
        static struct timeval tm1, tm2;                               \
        uint64_t min_diff = (uint64_t)-1;                             \
        for (int i = 0; i < repeat; i++) {                            \
            __asm volatile("" ::: /* pretend to clobber */ "memory"); \
            gettimeofday(&tm1, NULL);                                \
            test;                                                     \
            gettimeofday(&tm2, NULL);                                \
            uint64_t tms = 1000 * (tm2.tv_sec - tm1.tv_sec) + (tm2.tv_usec - tm1.tv_usec) / 1000;              \
            if (tms < min_diff) min_diff = tms;       \
        }                                                             \
        printf(" %d ms", (int) min_diff);           \
        printf("\n");                                                 \
        fflush(NULL);                                                 \
    } while (0)

void create() {
    bitset_t * b1 = bitset_create();
    for(uint32_t k = 0; k < 100000000; k += 100) {
      bitset_set(b1,k);
    }
    bitset_free(b1);
}


size_t iterate(bitset_t *b1) {
  size_t sum = 0;
  for(size_t i = 0; nextSetBit(b1,&i) ; i++) {
    sum++;
  }
  return sum;
}


int main() {
  int repeat = 10;
  BEST_TIME(create(),repeat);
  bitset_t * b1 = bitset_create();
  for(uint32_t k = 0; k < 100000000; k += 100) {
    bitset_set(b1,k);
  }
  assert(bitset_count(b1) == iterate(b1));
  BEST_TIME(bitset_count(b1),repeat);
  BEST_TIME(iterate(b1),repeat);

  bitset_free(b1);

}
