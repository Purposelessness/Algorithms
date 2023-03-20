#ifndef A_STAR__ASTAR_H_
#define A_STAR__ASTAR_H_

#include <algorithm>
#include <climits>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "debug.h"
#include "graph.h"

#define NODE(x) graph_.data[x]

template <typename T>
  requires requires(T a, T b) {
             { b - a } -> std::convertible_to<float>;
           }
struct Heuristics {
  void setFinish(const T& to) { to_ = to; }

  constexpr float operator()(const T& from) {
    return static_cast<float>(std::abs(to_ - from));
  }

 private:
  T to_;
};

template <typename Func = Heuristics<int>>
class AStar {
 public:
  // Main function
  std::vector<int> solve(const Graph& graph) {
    PRINT("--- A* algorithm ---\n")
    graph_ = graph;
    open_.reserve(std::abs(graph_.finish - graph_.start));
    // Initialize heuristics function
    heuristics_.setFinish(graph_.finish);
    // Add start node to `open_` set
    open_.emplace(graph_.start);
    // Weight of start node = 0
    NODE(graph_.start).weight = 0;

    int last = graph_.start;
    while (last != graph.finish) {
      if (open_.empty()) {
        PRINT("There is no path\n");
        return {};
      }
      auto min_ch = findNearest();
      selectNode(min_ch);
      last = min_ch;
    }
    PRINT("Destination reached\n")

    // Writing down result by going from finish to start
    std::vector<int> result;
    result.reserve(std::abs(graph.finish - graph.start));
    while (last != graph_.start) {
      result.push_back(last);
      last = graph_.data[last].parent;
    }
    result.push_back(graph_.start);
    std::reverse(result.begin(), result.end());
    return result;
  }

 private:
  // Function to find node with the least weight
  inline int findNearest() {
    PRINT("Trying to select nearest node.\n")
    int minWeight = INT_MAX;
    int minC = 0;
    // Iterate over all nodes in `open_` and find with the least weight
    for (auto c : open_) {
      const auto& n = graph_.data[c];
      PRINT("`" << SYMBOL(c) << "`: length = " << n.pathLength
                << ", weight = " << n.weight << "\n");
      if (n.weight < minWeight) {
#ifdef DEBUG
        if (minWeight == INT_MAX) {
          PRINT("Found node `" << SYMBOL(c) << "`\n")
        } else {
          PRINT("Found node `" << SYMBOL(c) << "` (" << n.weight << " < "
                               << minWeight << ")\n")
        }
#endif
        minWeight = n.weight;
        minC = c;
      }
#ifdef DEBUG
      else {
        PRINT("Node is not optimal (" << n.weight << " >= " << minWeight
                                      << ")\n")
      }
#endif
    }
    return minC;
  }

  // Select node with the least weight
  inline void selectNode(int ch) {
    PRINT("Selected node `" << SYMBOL(ch) << "`, resolving nearest nodes.\n")
    auto& node = NODE(ch);
    // Iterate over all adjacent nodes
    for (const auto& p : node.paths) {
      PRINT(SYMBOL(p.a) << " -> " << SYMBOL(p.b) << " = " << p.length << "\n");
      auto& b_node = NODE(p.b);
      int len = node.pathLength + p.length;
      // If node is not in open_ -> add it to open_
      if (!open_.contains(p.b) && !close_.contains(p.b)) {
        PRINT("Adding node " << SYMBOL(p.b) << " to `open` set\n")
        b_node.setParent(ch, len, len + heuristics_(p.b));
        open_.emplace(p.b);
      }
      // If path from selected node is shorter than stored -> recalculate weight
      // of node and add it to `open_`
      else if (b_node.pathLength > len) {
        PRINT("Found better path to `"
              << SYMBOL(p.b) << "`, new length = " << len
              << ", new weight = " << len + heuristics_(p.b) << "\n")
        b_node.setParent(ch, len, len + heuristics_(p.b));
        // Remove node from `close_` if we've found shorter path
        if (close_.contains(p.b)) {
          close_.erase(p.b);
          open_.emplace(p.b);
        }
      }
#ifdef DEBUG
      else {
        PRINT("Cannot find better path from this node\n")
      }
#endif
    }
    PRINT("Adding node `" << SYMBOL(ch) << "` to `close` set\n")
    // This node is now seen.
    open_.erase(ch);
    close_.insert(ch);
  }

  Func heuristics_;
  Graph graph_;
  // Open nodes
  std::unordered_set<int> open_;
  // Visited nodes
  std::unordered_set<int> close_;
};

template <typename T>
  requires requires(T a, T b) {
             { b - a } -> std::convertible_to<float>;
           }
struct BadHeuristics {
  void setFinish(const T& /*unused*/) {}
  constexpr float operator()(const T& /*unused*/) { return 1; }
};

#endif  // A_STAR__ASTAR_H_