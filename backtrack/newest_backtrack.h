#ifndef BACKTRACK__NEWEST_BACKTRACK_H_
#define BACKTRACK__NEWEST_BACKTRACK_H_

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>
#include <bitset>

#define CLEAN_RECT(x, y, size)           \
  for (int j = y; j < y + size; ++j) {   \
    for (int i = x; i < x + size; ++i) { \
      rect[j][i] = false;                \
    }                                    \
  }

struct Rect {
  Rect(int x, int y, int size) : x(x), y(y), size(size) {}

  int x;
  int y;
  int size;
};

int max_square_count = INT_MAX;
int n = 2;

std::vector<std::bitset<40>> rect;

inline int TryPlace(int y, int size) {
  int i = 0;
  for (; i < n && rect[y][i]; ++i) {
  }
  int x = i;
  if (x + size > n || y + size > n) {
    return -1;
  }
  int j = y;
  for (; j < y + size; ++j) {
    for (i = x; i < x + size; ++i) {
      if (rect[j][i]) {
        if (i == x) {
          --j;
          i = x + size - 1;
        } else {
          --i;
        }
        for (; j >= y; --j) {
          for (; i >= x; --i) {
            rect[j][i] = false;
          }
          i = x + size - 1;
        }
        return -1;
      }
      rect[j][i] = true;
    }
  }
  return x;
}

std::vector<int> heights;

int square_count = 0;
int min_square_count = INT_MAX;
std::vector<Rect> partial_answer;
std::vector<Rect> answer;

void NewBacktrack(int y, int limit) {
  int width = heights[y];
  if (width == n) {
    if (y == n - 1) {
      if (square_count < min_square_count) {
        min_square_count = square_count;
        answer = partial_answer;
      }
      return;
    }
    NewBacktrack(y + 1, INT_MAX);
  }
  if (square_count >= min_square_count || square_count >= max_square_count) {
    return;
  }
  int size = std::min({n - y, n - width, limit, n - 1});
  for (; size >= 1; --size) {
    auto x = TryPlace(y, size);
    if (x == -1) {
      continue;
    }
    partial_answer.emplace_back(x, y, size);
    for (int j = y; j < y + size; ++j) {
      heights[j] += size;
    }
    ++square_count;
    NewBacktrack(y, size);
    --square_count;
    for (int j = y; j < y + size; ++j) {
      heights[j] -= size;
    }
    partial_answer.pop_back();
    CLEAN_RECT(x, y, size);
  }
}

void Backtrack(int new_n) {
  n = new_n;
  heights.resize(n);
  max_square_count = 6 * std::log2(3 * n - 1) - 9;
  rect.resize(n);
  partial_answer.reserve(n);
  answer.reserve(n);
  NewBacktrack(0, INT_MAX);
  std::cout << min_square_count << '\n';
  for (auto& r : answer) {
    std::cout << r.y << ' ' << r.x << ' ' << r.size << '\n';
  }
}

#endif  // BACKTRACK__NEWEST_BACKTRACK_H_