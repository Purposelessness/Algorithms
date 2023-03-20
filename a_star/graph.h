#ifndef A_STAR__GRAPH_H_
#define A_STAR__GRAPH_H_

#include <climits>
#include <unordered_map>
#include <vector>

struct Path {
  int a{}, b{};
  float length{};
  bool enabled = true;
};

struct Node {
  inline void setParent(int newParent, int newLength, int newWeight) {
    parent = newParent;
    pathLength = newLength;
    weight = newWeight;
  }

  int data = 0;
  int parent = 0;
  int pathLength = 0;
  int weight = INT_MAX;
  std::vector<Path> paths;
};

struct Graph {
  int start, finish;
  std::unordered_map<int, Node> data;
};

#endif  // A_STAR__GRAPH_H_