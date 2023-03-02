#ifndef A_STAR__GRAPH_H_
#define A_STAR__GRAPH_H_

#include <climits>
#include <unordered_map>
#include <vector>

struct Path {
  char a, b;
  float length;
};

struct Node {
  inline void SetParent(char ch, int new_length, int new_weight) {
    previous = ch;
    path_length = new_length;
    weight = new_weight;
  }

  char data = 0;
  char previous = 0;
  int path_length = 0;
  int weight = INT_MAX;
  std::vector<Path> paths;
};

struct Graph {
  char start, finish;
  std::unordered_map<char, Node> data;
};

#endif  // A_STAR__GRAPH_H_