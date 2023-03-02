#ifndef A_STAR__SOLUTION_H_
#define A_STAR__SOLUTION_H_

#include <climits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

struct Heuristics {
  template <typename T>
    requires requires(T a, T b) { b - a; }
  constexpr auto operator()(const T& from, const T& to) -> decltype(to - from) {
    return to - from;
  }
};

template <typename Func = Heuristics>
class Solution {
 public:
  std::string Solve(const Graph& graph) {
    graph_ = graph;
    open_.reserve(graph_.finish - graph_.start);
    open_.emplace(graph_.start, &graph_.data[graph_.start]);
    open_[graph_.start]->weight = 0;
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
    char min_ch = 0;
    int min_weight = INT_MAX;
    for (const auto& p : open_) {
      if (p.second->weight < min_weight) {
        min_ch = p.first;
        min_weight = p.second->weight;
      }
    }
    return min_ch;
  }

  inline void SelectNode(char ch) {
    auto& node = open_[ch];
    for (const auto& p : node->paths) {
      if (close_.contains(p.b)) {
        continue;
      }
      if (open_.contains(p.b)) {
        int len = node->path_length + p.length;
        int h = heuristics_(p.b, p.a);
        if (len + 2 * h < open_[p.b]->weight) {
          open_[p.b]->SetParent(ch, len, len + 2 * h);
        }
      } else {
        open_.emplace(p.b, &graph_.data[p.b]);
        int len = node->path_length + p.length;
        int h = heuristics_(p.b, p.a);
        open_[p.b]->SetParent(ch, len, len + 2 * h);
      }
    }
    open_.erase(ch);
    close_.insert(ch);
  }

  Func heuristics_;
  Graph graph_;
  std::unordered_map<char, Node*> open_;
  std::unordered_set<char> close_;
};

#endif  // A_STAR__SOLUTION_H_