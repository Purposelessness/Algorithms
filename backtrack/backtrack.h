#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

struct Square {
  Square() = default;
  Square(int y, int size) : y(y), size(size) {}

  int x = -1;
  int y = 0;
  int size = 0;
};

int n = 2;
int max_square_size = INT_MAX;
int second_max_square_size = INT_MAX;

int square_count = 0;
int max_square_count = INT_MAX;
int answer_size = INT_MAX;

int* levels;
Square* partial_answer;
Square* answer;

void BacktrackImpl(int y, int limit) {
  int f = levels[y];
  if (f == 0) {
    if (y == 0) {
      second_max_square_size = partial_answer[1].size;
    }
    BacktrackImpl(y + 1, second_max_square_size);
    return;
  }
  if (y == n) {
    if (square_count < answer_size) {
      answer_size = square_count;
      std::memcpy(answer, partial_answer, answer_size * sizeof(Square));
    }
    return;
  }
  if (square_count >= answer_size || square_count >= max_square_count) {
    return;
  }
  int size = std::min({f, n - y, limit});
  for (; size >= 1; --size) {
    levels[y] -= size;
    levels[y + size] += size;
    partial_answer[square_count].y = y;
    partial_answer[square_count].size = size;
    ++square_count;
    BacktrackImpl(y, size);
    --square_count;
    levels[y + size] -= size;
    levels[y] += size;
  }
}

std::bitset<40>* rect;
int answer_left = 0;
int x_max = 0;

bool FindCoords(int y) {
  if (y == n) {
    return answer_left <= 0;
  }
  if (rect[y].count() == n) {
    return FindCoords(y + 1);
  }
  for (int i = 0; i < answer_size; ++i) {
    if (answer[i].y > y) {
      break;
    }
    if (answer[i].x != -1 || answer[i].y != y) {
      continue;
    }
    auto size = answer[i].size;
    // Set square
    std::bitset<40> row(std::pow(2, size) - 1);
    int x = 39;
    for (; x >= x_max && rect[y][x]; --x) {
    }
    row <<= x - size + 1;
    for (int j = y; j < y + size; ++j) {
      if ((rect[j] & row).any()) {
        goto next_square;
      }
    }
    for (int j = y; j < y + size; ++j) {
      rect[j] |= row;
    }
    // ------
    answer[i].x = 40 - x;
    --answer_left;
    if (FindCoords(y)) {
      return true;
    }
    ++answer_left;
    answer[i].x = -1;
    // Clear square
    row.flip();
    for (int j = y; j < y + size; ++j) {
      rect[j] &= row;
    }
    // ----
  next_square:
    continue;
  }
  return false;
}

struct Pair {
  int num = 1;
  int degree = 0;
};

int IsPrimeOfTwo(int size) {
  Pair primes[] = {{3, 2}, {7, 3}, {31, 5}};
  for (auto& prime : primes) {
    if (size == prime.num) {
      return prime.degree;
    }
  }
  return -1;
}

bool IsPrime(int c) {
  int primes[] = {3, 5, 7, 11, 13, 17, 23, 29, 31, 37};
  for (int num : primes) {
    if (c == num) {
      return true;
    }
  }
  return false;
}

void PrimeTwoSearch() {
  answer[0].x = 1;
  answer[0].y = 0;
  int size = (n + 1) / 2;
  answer[0].size = size;
  answer[1].size = answer[2].size = size - 1;
  answer[1].x = size + 1;
  answer[2].x = 1;
  answer[2].y = size;
  answer_size = 3;
  int t = n;
  while (size > 1) {
    size /= 2;
    answer[answer_size].size = answer[answer_size + 1].size =
        answer[answer_size + 2].size = size;
    t -= size;
    answer[answer_size].x = answer[answer_size].y = t + 1;
    answer[answer_size + 1].x = t + 1;
    answer[answer_size + 1].y = t - size;
    answer[answer_size + 2].x = t - size + 1;
    answer[answer_size + 2].y = t;
    answer_size += 3;
  }
}

void Backtrack(int n_new) {
  n = n_new;
  max_square_count = 6 * std::log2(3 * n - 1) - 9;
  answer = new Square[max_square_count]{};

  auto deg = IsPrimeOfTwo(n_new);
  if (deg != -1) {
    PrimeTwoSearch();
  } else {
    // Finding square sizes
    if (IsPrime(n)) {
      max_square_size = 0.57 * n;
    } else {
      max_square_size = 0.75 * n;
    }
    second_max_square_size = 0.5 * n;
    partial_answer = new Square[max_square_count]{};
    levels = new int[n + 1]{};
    levels[0] = n;
    BacktrackImpl(0, max_square_size);
    delete[] levels;
    delete[] partial_answer;
    // Finding coords
    answer_left = answer_size;
    x_max = 40 - n;
    rect = new std::bitset<40>[n];
    FindCoords(0);
    delete[] rect;
  }

  std::cout << answer_size << '\n';
  for (int i = 0; i < answer_size; ++i) {
    std::cout << answer[i].y + 1 << ' ' << answer[i].x << ' ' << answer[i].size
              << '\n';
  }

  delete[] answer;
}

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop