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
  bitset_free(b);
}

bool increment(size_t value, void *param) {
  size_t k; 
  memcpy(&k, param, sizeof(size_t));
  assert(value == k);
  k += 3;
  memcpy(param, &k, sizeof(size_t));
  return true;
}

void test_iterate2() {
  bitset_t * b = bitset_create();
  for(int k = 0; k < 1000; ++k)
    bitset_set(b,3*k);
  assert(bitset_count(b) == 1000);
  size_t k = 0;
  bitset_for_each(b,increment,&k);
  assert(k == 3000);
  bitset_free(b);
}


void test_construct() {
  bitset_t * b = bitset_create();
  for(int k = 0; k < 1000; ++k)
    bitset_set(b,3*k);
  assert(bitset_count(b) == 1000);
  for(int k = 0; k < 3*1000; ++k)
    assert(bitset_get(b,k) == (k/3*3 == k));
  bitset_free(b);
}

void test_max_min() {
  bitset_t * b = bitset_create();
  for(size_t k = 100; k < 1000; ++k) {
    bitset_set(b,3*k);
    assert(bitset_minimum(b) == 3 * 100);
    assert(bitset_maximum(b) == 3 * k);
  }
  bitset_free(b);
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
  bitset_free(b1);
  bitset_free(b2);
}

void test_counts() {
  bitset_t * b1 = bitset_create();
  bitset_t * b2 = bitset_create();

  for(int k = 0; k < 1000; ++k) {
    bitset_set(b1,2*k);
    bitset_set(b2,3*k);
  }
  assert(bitset_intersection_count(b1,b2) == 334);
  assert(bitset_union_count(b1,b2) == 1666);
  bitset_free(b1);
  bitset_free(b2);
}


int main() {
  test_construct();
  test_union_intersection();
  test_iterate();
  test_iterate2();
  test_max_min();
  test_counts();
  printf("All asserts passed. Code is probably ok.\n");
}
