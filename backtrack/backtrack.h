#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <vector>

struct Rect {
  Rect(int x, int size) : x(x), size(size) {}

  int x;
  int size;
};

int min_edges = CHAR_MAX;
int n;
int max_edges;
std::vector<int> graph;
std::vector<Rect> rects;
std::vector<Rect> min_rects;

void BacktrackImpl(int x, int limit, int edges) {
  int f = graph[x];
  if (f == 0) {
    BacktrackImpl(x + 1, INT_MAX, edges);
    return;
  }
  if (x == n) {
    if (edges < min_edges) {
      min_edges = std::min(min_edges, edges);
      min_rects = rects;
    }
    return;
  }
  if (edges >= min_edges || edges >= max_edges) {
    return;
  }
  int j = std::min({f, limit, n - x, n - 1});
  for (; j >= 1; --j) {
    graph[x] -= j;
    graph[x + j] += j;
    rects.emplace_back(x, j);
    ++edges;
    BacktrackImpl(x, j, edges);
    --edges;
    rects.pop_back();
    graph[x + j] -= j;
    graph[x] += j;
  }
}

void Backtrack(int n) {
  max_edges = 6 * std::log2(3 * n - 1) - 9;
  graph.resize(n + 1);
  graph[0] = n;
  BacktrackImpl(0, INT_MAX, 0);
  std::cout << min_edges << '\n';
  std::for_each(min_rects.begin(), min_rects.end(), [](const auto& p) {
    std::cout << p.x + 1 << ' ' << p.size << '\n';
  });
}

#endif  // BACKTRACK__BACKTRACK_H_
