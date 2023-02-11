#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <array>
#include <climits>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

struct Square {
  Square() = default;
  Square(int y, int size) : y(y), size(size) {}

  int y;
  int size;
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

void Backtrack(int n_new) {
  n = n_new;
  max_square_size = 0.7 * n;
  second_max_square_size = 0.5 * n;
  levels = new int[n + 1]{};
  max_square_count = 6 * std::log2(3 * n - 1) - 9;
  partial_answer = new Square[max_square_count]{};
  answer = new Square[max_square_count]{};
  levels[0] = n;
  BacktrackImpl(0, max_square_size);
  std::cout << answer_size << '\n';
  for (int i = 0; i < answer_size; ++i) {
    std::cout << answer[i].y + 1 << ' ' << answer[i].size << '\n';
  }
  delete[] answer;
  delete[] partial_answer;
  delete[] levels;
}

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop