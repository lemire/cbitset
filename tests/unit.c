#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "bitset.h"


void test_construct() {
  bitset_t * b = bitset_create();
  for(int k = 0; k < 1000; ++k)
    bitset_set(b,3*k);
  assert(bitset_count(b) == 1000);
  for(int k = 0; k < 3*1000; ++k)
    assert(bitset_get(b,k) == (k/3*3 == k));
}

void test_union_intersection() {
  bitset_t * b1 = bitset_create();
  bitset_t * b2 = bitset_create();

  for(int k = 0; k < 1000; ++k) {
    bitset_set(b1,2*k);
    bitset_set(b2,2*k+1);
  }
  bitset_inplace_union(b1,b2);
  assert(bitset_count(b1) == 2000);
  bitset_inplace_intersection(b1,b2);
  assert(bitset_count(b1) == 1000);
}

int main() {
  test_construct();
  test_union_intersection();
  printf("All asserts passed. Code is probably ok.\n");
}
