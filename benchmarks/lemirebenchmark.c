#include "bitset.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BEST_TIME(test, repeat)                                                \
  do {                                                                         \
    printf("%s: ", #test);                                                     \
    fflush(NULL);                                                              \
    static struct timeval tm1, tm2;                                            \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      gettimeofday(&tm1, NULL);                                                \
      test;                                                                    \
      gettimeofday(&tm2, NULL);                                                \
      uint64_t tmus = 1000 * 1000 * (tm2.tv_sec - tm1.tv_sec) +                \
                      (tm2.tv_usec - tm1.tv_usec);                             \
      if (tmus < min_diff)                                                     \
        min_diff = tmus;                                                       \
    }                                                                          \
    printf(" %f ms", min_diff / 1000.0);                                       \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

#define BEST_TIME_CHECK(test, check, repeat)                                   \
  do {                                                                         \
    printf("%s: ", #test);                                                     \
    fflush(NULL);                                                              \
    static struct timeval tm1, tm2;                                            \
    uint64_t min_diff = (uint64_t)-1;                                          \
    for (int i = 0; i < repeat; i++) {                                         \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      gettimeofday(&tm1, NULL);                                                \
      if (test != check)                                                       \
        printf("bug");                                                         \
      gettimeofday(&tm2, NULL);                                                \
      uint64_t tmus = 1000 * 1000 * (tm2.tv_sec - tm1.tv_sec) +                \
                      (tm2.tv_usec - tm1.tv_usec);                             \
      if (tmus < min_diff)                                                     \
        min_diff = tmus;                                                       \
    }                                                                          \
    printf(" %f ms", min_diff / 1000.0);                                       \
    printf("\n");                                                              \
    fflush(NULL);                                                              \
  } while (0)

void create() {
  bitset_t *b1 = bitset_create();
  for (uint32_t k = 0; k < 100000000; k += 100) {
    bitset_set(b1, k);
  }
  bitset_free(b1);
}

size_t iterate(bitset_t *b1) {
  size_t sum = 0;
  for (size_t i = 0; nextSetBit(b1, &i); i++) {
    sum++;
  }
  return sum;
}

size_t iterateb(bitset_t *b1) {
  size_t sum = 0;
  for (size_t i = 0; nextSetBit(b1, &i); i++) {
    sum += i;
  }
  return sum;
}

bool incr(size_t value, void *param) {
  (void)value;
  size_t sum;
  memcpy(&sum, param, sizeof(size_t));
  sum++;
  memcpy(param, &sum, sizeof(size_t));
  return true;
}

size_t iterate2(bitset_t *b1) {
  size_t sum = 0;
  bitset_for_each(b1, incr, &sum);
  return sum;
}

bool incrb(size_t value, void *param) {
  size_t sum;
  memcpy(&sum, param, sizeof(size_t));
  sum += value;
  memcpy(param, &sum, sizeof(size_t));
  return true;
}

size_t iterate2b(bitset_t *b1) {
  size_t sum = 0;
  bitset_for_each(b1, incrb, &sum);
  return sum;
}

size_t iterate3(bitset_t *b1) {
  size_t sum = 0;
  size_t buffer[256];
  size_t howmany = 0;
  for (size_t startfrom = 0;
       (howmany = nextSetBits(b1, buffer, 256, &startfrom)) > 0; startfrom++) {
    for (size_t i = 0; i < howmany; i++) {
      sum++;
    }
  }
  return sum;
}
size_t iterate3b(bitset_t *b1) {
  size_t sum = 0;
  size_t buffer[256];
  size_t howmany = 0;
  for (size_t startfrom = 0;
       (howmany = nextSetBits(b1, buffer, 256, &startfrom)) > 0; startfrom++) {
    for (size_t i = 0; i < howmany; i++) {
      sum += buffer[i];
    }
  }
  return sum;
}

int main() {
  int repeat = 10;
  BEST_TIME(create(), repeat);
  bitset_t *b1 = bitset_create();
  size_t count = 0;
  for (uint32_t k = 0; k < 100000000; k += 100) {
    bitset_set(b1, k);
    ++count;
  }
  assert(bitset_count(b1) == iterate(b1));
  BEST_TIME_CHECK(bitset_count(b1), count, repeat);
  BEST_TIME_CHECK(iterate(b1), count, repeat);
  BEST_TIME_CHECK(iterate2(b1), count, repeat);
  BEST_TIME_CHECK(iterate3(b1), count, repeat);
  size_t expected = iterate3b(b1);
  BEST_TIME_CHECK(iterateb(b1), expected, repeat);
  BEST_TIME_CHECK(iterate2b(b1), expected, repeat);
  BEST_TIME_CHECK(iterate3b(b1), expected, repeat);

  bitset_free(b1);
}
