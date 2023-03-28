#ifndef COMMIS_VOYAGEUR__SOLUTION_H_
#define COMMIS_VOYAGEUR__SOLUTION_H_

#include <unordered_set>

#include "graph.h"

class Solution {
 public:
  struct Answer {
    std::vector<int> path;
    int length{};
  };

  [[nodiscard]] static Answer Solve(Graph graph) {
    int d = 0;
    auto null_vec = Reduction(graph, d);
    auto new_branch = Branching(graph, null_vec);
  }

  static PointSet Reduction(Graph& graph, int& d) {
    auto null_edges = RowReduction(graph, d);
    auto column_set = ColumnReduction(graph, d);
    null_edges.insert(column_set.cbegin(), column_set.cend());
    return null_edges;
  }

  static PointSet RowReduction(Graph& graph, int& d) {
    PointSet set;
    for_column(0, _size, {
      auto min_edge = FindRowMinimal(graph, j, 0, _size);
      int min_weight = min_edge.second;
      d += min_weight;
      for_row(0, _size, {
        if (is_valid(j, i)) {
          _graph[j][i] -= min_weight;
          if (_graph[j][i] == 0) {
            set.emplace(j, i);
          }
        }
      });
    });
    return set;
  }

  static PointSet ColumnReduction(Graph& graph, int& d) {
    PointSet set;
    for_row(0, _size, {
      auto min_edge = FindColumnMinimal(graph, 0, _size, i);
      int min_weight = min_edge.second;
      d += min_weight;
      for_column(0, _size, {
        if (is_valid(j, i)) {
          _graph[j][i] -= min_weight;
          if (_graph[j][i] == 0) {
            set.emplace(j, i);
          }
        }
      });
    });
    return set;
  }

  // Return branching edge:
  // 0 - edge position in matrix,
  // 1 - min edge in row,
  // 2 - min edge in column
  static std::vector<Point> Branching(Graph& graph,
                                      const PointSet& null_edges) {
    std::vector<Point> new_branch(3);
    int max_d = 0;
    for (const auto& p : null_edges) {
      auto row_edge = FindRowMinimal(graph, p.y, 0, p.x);
      row_edge = FindRowMinimal(graph, p.y, p.x + 1, _size, row_edge.second);

      auto column_edge = FindColumnMinimal(graph, 0, p.y, p.x);
      column_edge =
          FindColumnMinimal(graph, p.y + 1, _size, p.x, column_edge.second);

      int sum = row_edge.second + column_edge.second;
      if (sum > max_d) {
        max_d = sum;
        new_branch[0] = {p.y, p.x};
        new_branch[1] = {p.y, row_edge.first};
        new_branch[2] = {column_edge.first, p.x};
      }
    }
    return new_branch;
  }

  // Find minimal edge in row
  static inline std::pair<int, int> FindRowMinimal(Graph& graph, int y, int x0,
                                                   int x1,
                                                   int min_weight = INT_MAX) {
    int k = 0;
    for_row(x0, x1, {
      if (is_valid(y, i) && _graph[y][i] < min_weight) {
        min_weight = _graph[y][i];
        k = i;
      }
    });
    return {k, min_weight};
  }

  // Find minimal edge in column
  static inline std::pair<int, int> FindColumnMinimal(
      Graph& graph, int y0, int y1, int x, int min_weight = INT_MAX) {
    int k = 0;
    for (int j = y0; j < y1; ++j) {
      if (is_valid(j, x) && _graph[j][x] < min_weight) {
        min_weight = _graph[j][x];
        k = j;
      }
    }
    return {k, min_weight};
  }
};

#endif  // COMMIS_VOYAGEUR__SOLUTION_H_