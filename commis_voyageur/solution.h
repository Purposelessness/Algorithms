#ifndef COMMIS_VOYAGEUR__SOLUTION_H_
#define COMMIS_VOYAGEUR__SOLUTION_H_

#include <climits>
#include <unordered_set>

#include "debug.h"
#include "graph.h"

#define d_graph data.graph.data

class Solution {
  struct Data {
    Graph graph;
    Path path;
  };

 public:
  struct Answer {
    std::vector<int> path;
    int length{};
  };

  [[nodiscard]] Answer Solve(const Graph& graph) {
    print("starting graph\n");
    print_graph(graph);
    Data data{graph, {}};
    auto null_edges = Reduction(data.graph, data.path.length);
    auto new_branch = Branching(data.graph, null_edges);
    IncludeEdge(data, new_branch[0]);
    ExcludeEdge(data, new_branch, null_edges);
  }

  static PointSet Reduction(Graph& graph, int& d) {
    auto null_edges = RowReduction(graph, d);
    auto column_set = ColumnReduction(graph, d);
    null_edges.insert(column_set.cbegin(), column_set.cend());
    return null_edges;
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
      auto row_edge2 =
          FindRowMinimal(graph, p.y, p.x + 1, _size, row_edge.second);
      if (row_edge2.first != -1) {
        row_edge = row_edge2;
      }

      auto column_edge = FindColumnMinimal(graph, 0, p.y, p.x);
      auto column_edge2 =
          FindColumnMinimal(graph, p.y + 1, _size, p.x, column_edge.second);
      if (column_edge2.first != -1) {
        column_edge = column_edge2;
      }

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

  void IncludeEdge(Data data, const Point& edge) {
    data.graph.crossed_columns.insert(edge.x);
    data.graph.crossed_rows.insert(edge.y);
    d_graph[edge.x][edge.y] = infty;
    auto null_edges = Reduction(data.graph, data.path.length);

    if (!IsStillOptimal(data.path.length)) {
      return;
    }
    auto new_branch = Branching(data.graph, null_edges);
    IncludeEdge(data, new_branch[0]);
    ExcludeEdge(data, new_branch, null_edges);
  }

  void ExcludeEdge(Data data, const std::vector<Point>& branch,
                   PointSet null_edges) {
    const auto& edge = branch[0];
    d_graph[edge.y][edge.x] = infty;
    null_edges.erase({edge.y, edge.x});
    const auto& row_edge = branch[1];
    const auto& column_edge = branch[2];
    int row_edge_weight = d_graph[row_edge.y][row_edge.x];
    int column_edge_weight = d_graph[column_edge.y][column_edge.x];
    data.path.length += d_graph[row_edge.y][row_edge.x];
    data.path.length += d_graph[column_edge.y][column_edge.x];

    if (!IsStillOptimal(data.path.length)) {
      return;
    }
    if (row_edge_weight > 0) {
      ReduceRow(data.graph, edge.y, row_edge_weight, null_edges);
    }
    if (column_edge_weight > 0) {
      ReduceColumn(data.graph, edge.x, column_edge_weight, null_edges);
    }
    auto new_branch = Branching(data.graph, null_edges);
    IncludeEdge(data, new_branch[0]);
    ExcludeEdge(data, new_branch, null_edges);
  }

  static PointSet RowReduction(Graph& graph, int& d) {
    PointSet set;
    for_column(0, _size, {
      auto min_edge = FindRowMinimal(graph, j, 0, _size);
      int min_weight = min_edge.second;
      d += min_weight;
      ReduceRow(graph, j, min_weight, set);
    });
    return set;
  }

  static PointSet ColumnReduction(Graph& graph, int& d) {
    PointSet set;
    for_row(0, _size, {
      auto min_edge = FindColumnMinimal(graph, 0, _size, i);
      int min_weight = min_edge.second;
      d += min_weight;
      ReduceColumn(graph, i, min_weight, set);
    });
    return set;
  }

  static inline void ReduceRow(Graph& graph, int y, int weight,
                               PointSet& null_edges) {
    for_row(0, _size, {
      if (is_valid(y, i)) {
        _graph[y][i] -= weight;
        if (_graph[y][i] == 0) {
          null_edges.emplace(y, i);
        }
      }
    });
  }

  static inline void ReduceColumn(Graph& graph, int x, int weight,
                                  PointSet& null_edges) {
    for_column(0, _size, {
      if (is_valid(j, x)) {
        _graph[j][x] -= weight;
        if (_graph[j][x] == 0) {
          null_edges.emplace(j, x);
        }
      }
    });
  }

  // Find minimal edge in row
  static inline std::pair<int, int> FindRowMinimal(Graph& graph, int y, int x0,
                                                   int x1,
                                                   int min_weight = INT_MAX) {
    int k = -1;
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
    int k = -1;
    for_column(y0, y1, {
      if (is_valid(j, x) && _graph[j][x] < min_weight) {
        min_weight = _graph[j][x];
        k = j;
      }
    });
    return {k, min_weight};
  }

  bool IsStillOptimal(int length) { return length <= estimated_len_; }

 private:
  int estimated_len_ = INT_MAX;
  Path optimal_path_;
};

#endif  // COMMIS_VOYAGEUR__SOLUTION_H_