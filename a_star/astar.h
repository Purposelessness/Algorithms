#ifndef A_STAR__ASTAR_H_
#define A_STAR__ASTAR_H_

#include <algorithm>
#include <climits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

#define NODE(x) graph_.data[x]
#define CONTAINS(c, x) (std::find(c.cbegin(), c.cend(), x) != c.end())
#define ERASE(c, x) (c.erase(std::remove(c.begin(), c.end(), x), c.end()))

template <typename T>
  requires requires(T a, T b) {
             { b - a } -> std::convertible_to<float>;
           }
struct Heuristics {
  void SetFinish(const T& to) { to_ = to; }

  constexpr float operator()(const T& from) { return to_ - from; }

 private:
  T to_;
};

template <typename Func = Heuristics<int>>
class AStar {
 public:
  std::string Solve(const Graph& graph) {
    graph_ = graph;
    open_.reserve(graph_.finish - graph_.start);
    heuristics_.SetFinish(graph_.finish);
    open_.push_back(graph_.start);
    NODE(graph_.start).weight = 0;
    char last = 0;
    while (last != graph.finish) {
      char min_ch = FindNearest();
      SelectNode(min_ch);
      last = min_ch;
    }
    std::string result;
    result.reserve(graph.finish - graph.start);
    while (last != 0) {
      result += last;
      last = graph_.data[last].previous;
    }
    std::reverse(result.begin(), result.end());
    return result;
  }

 private:
  inline char FindNearest() {
    int min_weight = INT_MAX;
    char min_c = 0;
    for (auto c : open_) {
      const auto& n = graph_.data[c];
      if (n.weight < min_weight) {
        min_weight = n.weight;
        min_c = c;
      }
    }
    return min_c;
  }

  inline void SelectNode(char ch) {
    auto& node = NODE(ch);
    for (const auto& p : node.paths) {
      if (close_.contains(p.b)) {
        continue;
      }
      auto& b_node = NODE(p.b);
      int len = node.path_length + p.length;
      int h = kHeuristicsCoefficient * heuristics_(p.b);
      if (len + h < b_node.weight) {
        b_node.SetParent(ch, len, len + h);
      }
      if (!CONTAINS(open_, p.b)) {
        open_.push_back(p.b);
      }
    }
    ERASE(open_, ch);
    close_.insert(ch);
  }

  Func heuristics_;
  static constexpr float kHeuristicsCoefficient = 0.5;
  Graph graph_;
  std::vector<char> open_;
  std::unordered_set<char> close_;
};

#endif  // A_STAR__ASTAR_H_