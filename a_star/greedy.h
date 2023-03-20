#ifndef A_STAR__GREEDY_H_
#define A_STAR__GREEDY_H_

#include <algorithm>
#include <climits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "graph.h"

#define NODE(x) graph_.data[x]

class Greedy {
 public:
  // Main function of the algorithm
  std::vector<int> solve(const Graph& graph) {
    PRINT("--- Greedy algorithm ---\n")
    graph_ = graph;
    path_.reserve(std::abs(graph_.finish - graph_.start));
    // Set start node
    current_ = graph_.start;
    // Loop while do not find destination
    while (current_ != graph_.finish) {
      PRINT(SYMBOL(current_) << " -> ?\n")
      if (!selectNearest()) {
        PRINT("There is no path\n");
        return {};
      }
    }
    PRINT("Destination reached\n")
    // Writing down results
    std::vector<int> result;
    result.reserve(path_.size());
    std::for_each(path_.cbegin(), path_.cend(),
                  [&result](Path* path) { result.push_back(path->a); });
    result.push_back(graph_.finish);
    return result;
  }

 private:
  // Function for searching and selecting nearest node
  inline bool selectNearest() {
    PRINT("Trying to select nearest node.\n")
    int minLen = INT_MAX;
    int minC = 0;
    bool found = false;
    auto& node = NODE(current_);
    Path* path = nullptr;
    // Iterate over all paths from current node
    for (auto& p : node.paths) {
      PRINT(SYMBOL(p.a) << " -> " << SYMBOL(p.b) << " = " << p.length
                        << " (enabled = " << p.enabled << ")\n");
      // If path is disabled -> program already visited it
      // If close contains node -> this node is dead end
      if (!p.enabled || close_.contains(p.b)) {
        PRINT("This path is unavailable\n")
        continue;
      }
      // Calculate new length
      int len = node.pathLength + int(p.length);
      if (len < minLen) {
#ifdef DEBUG
        if (!found) {
          PRINT("Selecting this path\n")
        } else {
          PRINT("This path is better than previous one ("
                << len - node.pathLength << " < " << minLen - node.pathLength
                << ")\n")
        }
#endif
        minLen = len;
        minC = p.b;
        path = &p;
        found = true;
      }
#ifdef DEBUG
      else {
        PRINT("This path isn't optimal ("
              << len - node.pathLength << " >= " << minLen - node.pathLength << ")\n")
      }
#endif
    }
    // If node has no adjacent nodes then it is a dead end -> add it to close_
    if (!found) {
      PRINT("The path is dead end.\n")
      close_.insert(current_);
      current_ = node.parent;
      if (path_.empty()) {
        return false;
      }
      path_.pop_back();
      return true;
    }
    PRINT("Selected node `" << SYMBOL(minC) << "`, total length = " << minLen
                            << '\n')
    auto& minNode = NODE(minC);
    // Select new node
    minNode.setParent(current_, minLen, minLen);
    path->enabled = false;
    path_.push_back(path);
    current_ = minC;
    return true;
  }

  Graph graph_;
  int current_ = 0;
  // The result path
  std::vector<Path*> path_;
  // Dead-end nodes
  std::unordered_set<int> close_;
};

#endif  // A_STAR__GREEDY_H_