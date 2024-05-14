//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// std::generator

#include <generator>

#include <cassert>
#include <ranges>
#include <utility>
#include <vector>
#include <cstddef>

bool test() {
  std::vector<int> expected_fib_vec = {0, 1, 1, 2, 3};
  {
    auto fib = []() -> std::generator<int, int, std::allocator<std::byte>> {
      int a = 0;
      int b = 1;
      while (true) {
        co_yield std::exchange(a, std::exchange(b, a + b));
      }
    };

    auto fib_vec = fib() | std::views::take(5) | std::ranges::to<std::vector<int>>();
    assert(fib_vec == expected_fib_vec);
  }

  {
    auto fib = [](std::allocator_arg_t, std::allocator<std::byte>) -> std::generator<int, int, std::allocator<std::byte>> {
      int a = 0;
      int b = 1;
      while (true) {
        co_yield std::exchange(a, std::exchange(b, a + b));
      }
    };

    auto fib_vec = fib(std::allocator_arg_t{}, {}) | std::views::take(5) | std::ranges::to<std::vector<int>>();
    assert(fib_vec == expected_fib_vec);
  }
  return true;
}

int main() {
  test();
  return 0;
}
