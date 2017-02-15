#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "bitset.h"

void test_iterate() {
  bitset_t * b = bitset_create();
  for(int k = 0; k < 1000; ++k)
    bitset_set(b,3*k);
  assert(bitset_count(b) == 1000);
  size_t k = 0;

  for(size_t i = 0; nextSetBit(b,&i) ; i++) {
    assert(i == k);
    k += 3;
  }
  assert(k == 3000);
}

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
  // calling xor twice should leave things unchanged
  bitset_inplace_symmetric_difference(b1,b2);
  assert(bitset_count(b1) == 2000);
  bitset_inplace_symmetric_difference(b1,b2);
  assert(bitset_count(b1) == 1000);
  bitset_inplace_difference(b1,b2);// should make no difference
  assert(bitset_count(b1) == 1000);
  bitset_inplace_union(b1,b2);
  assert(bitset_count(b1) == 2000);
  bitset_inplace_intersection(b1,b2);
  assert(bitset_count(b1) == 1000);
  bitset_inplace_difference(b1,b2);
  assert(bitset_count(b1) == 0);
  bitset_inplace_union(b1,b2);
  bitset_inplace_difference(b2,b1);
  assert(bitset_count(b2) == 0);

}

int main() {
  test_construct();
  test_union_intersection();
  test_iterate();
  printf("All asserts passed. Code is probably ok.\n");
}
