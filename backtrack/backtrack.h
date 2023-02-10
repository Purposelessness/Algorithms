#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

struct Rect {
  Rect(int y, int size) : y(y), size(size) {}

  int y;
  int size;
};

int n = 2;
int rect_count = 0;
int min_rect_count = INT_MAX;
int max_rect_count = INT_MAX;
std::vector<int> levels;
std::vector<Rect> partial_answer;
std::vector<Rect> answer;

void BacktrackImpl(int y, int limit) {
  int f = levels[y];
  if (f == 0) {
    BacktrackImpl(y + 1, INT_MAX);
    return;
  }
  if (y == n) {
    if (rect_count < min_rect_count) {
      min_rect_count = rect_count;
      answer = partial_answer;
    }
    return;
  }
  if (rect_count >= min_rect_count || rect_count >= max_rect_count) {
    return;
  }
  int j = std::min({f, limit, n - y, n - 1});
  for (; j >= 1; --j) {
    levels[y] -= j;
    levels[y + j] += j;
    partial_answer.emplace_back(y, j);
    ++rect_count;
    BacktrackImpl(y, j);
    --rect_count;
    partial_answer.pop_back();
    levels[y + j] -= j;
    levels[y] += j;
  }
}

void Backtrack(int n_new) {
  n = n_new;
  levels.resize(n + 1);
  max_rect_count = 6 * std::log2(3 * n - 1) - 9;
  partial_answer.reserve(n);
  answer.reserve(n);
  levels[0] = n;
  BacktrackImpl(0, INT_MAX);
  std::cout << min_rect_count << '\n';
  std::for_each(answer.begin(), answer.end(), [](const auto& p) {
    std::cout << p.y + 1 << ' ' << p.size << '\n';
  });
}

#endif  // BACKTRACK__BACKTRACK_H_
