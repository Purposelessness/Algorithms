#ifndef A_STAR__GREEDY_H_
#define A_STAR__GREEDY_H_

#include <algorithm>
#include <climits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"
#include "solution.h"

#define NODE(x) graph_.data[x]
#define CONTAINS(c, x) (std::find(c.cbegin(), c.cend(), x) != c.end())
#define ERASE(c, x) (c.erase(std::remove(c.begin(), c.end(), x), c.end()))

class Greedy {
 public:
  std::string Solve(const Graph& graph) {
    graph_ = graph;
    path_.reserve(graph_.finish - graph_.start);
    current_ = graph_.start;
    while (current_ != graph_.finish) {
      SelectNearest();
    }
    std::string result;
    result.reserve(path_.size());
    std::for_each(path_.cbegin(), path_.cend(),
                  [&result](Path* path) { result += path->a; });
    result += graph_.finish;
    return result;
  }

 private:
  inline void SelectNearest() {
    int min_len = INT_MAX;
    char min_c = 0;
    auto& node = NODE(current_);
    Path* path = nullptr;
    for (auto& p : node.paths) {
      if (!p.enabled || close_.contains(p.b)) {
        continue;
      }
      int len = node.path_length + int(p.length);
      if (len < min_len) {
        min_len = len;
        min_c = p.b;
        path = &p;
      }
    }
    if (min_c == 0) {
      close_.insert(current_);
      current_ = node.previous;
      path_.pop_back();
      return;
    }
    auto& min_node = NODE(min_c);
    min_node.SetParent(current_, min_len, min_len);
    path->enabled = false;
    path_.push_back(path);
    current_ = min_c;
  }

  Graph graph_;
  char current_ = 0;
  std::vector<Path*> path_;
  std::unordered_set<char> close_;
};

#endif  // A_STAR__GREEDY_H_