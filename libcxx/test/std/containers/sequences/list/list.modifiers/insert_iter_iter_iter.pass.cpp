//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <list>

// template <InputIterator Iter>
//   iterator insert(const_iterator position, Iter first, Iter last); // constexpr since C++26

#include <list>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "count_new.h"

template <class List>
TEST_CONSTEXPR_CXX26 void test() {
  int a1[] = {1, 2, 3};
  List l1;
  typename List::iterator i = l1.insert(l1.begin(), a1, a1 + 3);
  assert(i == l1.begin());
  assert(l1.size() == 3);
  assert(std::distance(l1.begin(), l1.end()) == 3);
  i = l1.begin();
  assert(*i == 1);
  ++i;
  assert(*i == 2);
  ++i;
  assert(*i == 3);
  int a2[] = {4, 5, 6};
  i        = l1.insert(i, a2, a2 + 3);
  assert(*i == 4);
  assert(l1.size() == 6);
  assert(std::distance(l1.begin(), l1.end()) == 6);
  i = l1.begin();
  assert(*i == 1);
  ++i;
  assert(*i == 2);
  ++i;
  assert(*i == 4);
  ++i;
  assert(*i == 5);
  ++i;
  assert(*i == 6);
  ++i;
  assert(*i == 3);

#if !defined(TEST_HAS_NO_EXCEPTIONS) && !defined(DISABLE_NEW_COUNT)
  if (!TEST_IS_CONSTANT_EVALUATED) {
    globalMemCounter.throw_after = 2;
    int save_count               = globalMemCounter.outstanding_new;
    try {
      i = l1.insert(i, a2, a2 + 3);
      assert(false);
    } catch (...) {
    }
    assert(globalMemCounter.checkOutstandingNewEq(save_count));
    assert(l1.size() == 6);
    assert(std::distance(l1.begin(), l1.end()) == 6);
    i = l1.begin();
    assert(*i == 1);
    ++i;
    assert(*i == 2);
    ++i;
    assert(*i == 4);
    ++i;
    assert(*i == 5);
    ++i;
    assert(*i == 6);
    ++i;
    assert(*i == 3);
  }
#endif
}

TEST_CONSTEXPR_CXX26 bool test() {
  test<std::list<int> >();
#if TEST_STD_VER >= 11
  test<std::list<int, min_allocator<int>>>();
#endif

  return true;
}

int main(int, char**) {
  assert(test());
#if TEST_STD_VER >= 26
  static_assert(test());
#endif

  return 0;
}
