#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <climits>
#include <cmath>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <vector>
#include <cstring>

struct Rect {
  Rect(int x, int y, int size) : x(x), y(y), size(size) {}

  int x;
  int y;
  int size;
};

int min_edges = CHAR_MAX;
int n;
int max_edges;
std::vector<int> graph;
std::unordered_map<int, std::vector<int>> rects;
std::unordered_map<int, std::vector<int>> min_rects;

void BacktrackImpl(int y, int limit, int edges) {
  int f = graph[y];
  if (f == 0) {
    BacktrackImpl(y + 1, INT_MAX, edges);
    return;
  }
  if (y == n) {
    if (edges < min_edges) {
      min_edges = std::min(min_edges, edges);
      min_rects = rects;
    }
    return;
  }
  if (edges >= min_edges || edges >= max_edges) {
    return;
  }
  int j = std::min({f, limit, n - y, n - 1});
  for (; j >= 1; --j) {
    graph[y] -= j;
    graph[y + j] += j;
    rects[y].push_back(j);
    ++edges;
    BacktrackImpl(y, j, edges);
    --edges;
    rects[y].pop_back();
    graph[y + j] -= j;
    graph[y] += j;
  }
}

std::vector<std::vector<bool>> rect;

int TryPlace(int y, int size) {
  auto temp_rect = rect;
  int i = 0;
  for (; i < n && temp_rect[y][i]; ++i) {
  }
  int x = i;
  if (x + size > n || y + size > n) {
    return -1;
  }
  int j = y;
  bool flag = true;
  for (; j < y + size; ++j) {
    for (i = x; i < x + size; ++i) {
      if (temp_rect[j][i]) {
        flag = false;
        break;
      }
      temp_rect[j][i] = true;
    }
    if (!flag) {
      break;
    }
  }
  if (flag) {
    rect = temp_rect;
    return x;
  }
//  for (; j >= y; --j) {
//    for (i -= 1; i >= size; --i) {
//      rect[j][i] = false;
//    }
//    i = x + size;
//  }
  return -1;
}

std::vector<Rect> answer;

bool FindCoords(int y) {
  auto& vec = min_rects[y];
  if (vec.empty()) {
    if (y == n - 1) {
      return true;
    }
    return FindCoords(y + 1);
  }
  auto vec_size = vec.size();
  for (int i = 0; i < vec_size; ++i) {
    auto size = vec[0];
    auto x = TryPlace(y, size);
    if (x == -1) {
      vec.erase(vec.begin());
      vec.push_back(size);
      continue;
    }
    vec.erase(vec.begin());
    answer.emplace_back(x, y, size);
    auto flag = FindCoords(y);
    if (flag) {
      return true;
    }
    // clear rect
    for (auto j = y; j < y + size; ++j) {
      for (auto z = x; z < x + size; ++z) {
        rect[j][z] = false;
      }
    }
    answer.pop_back();
    vec.push_back(size);
  }
  return false;
}

void Backtrack(int new_n) {
  n = new_n;
  max_edges = 6 * std::log2(3 * n - 1) - 9;
  graph.resize(n + 1);
  rects.reserve(n + 1);
  graph[0] = n;
  BacktrackImpl(0, INT_MAX, 0);
  std::cout << min_edges << '\n';

//  for (const auto& p : min_rects) {
//    std::cout << p.first << ": ";
//    for (const auto i : p.second) {
//      std::cout << i << ", ";
//    }
//    std::cout << '\n';
//  }

  rect.resize(n);
  for (auto& vec : rect) {
    vec.resize(n);
  }
  FindCoords(0);
  std::for_each(answer.begin(), answer.end(), [](const auto& r) {
    std::cout << r.y << ' ' << r.x << ' ' << r.size << '\n';
  });
}

#endif  // BACKTRACK__BACKTRACK_H_
