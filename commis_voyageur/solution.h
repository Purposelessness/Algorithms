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
  [[nodiscard]] Path Solve(const Graph& graph, int estimated_len = INT_MAX) {
    approximate_path_length_ = estimated_len;
    optimal_path_.length = INT_MAX;
    Data data{graph, {}};
    print("Graph:\n");
    print_graph(graph);
    print("\n");
    ReduceMatrix(data.graph, data.path.length);
    BnB(std::move(data));
    return optimal_path_;
  }

  void BnB(Data data) {
    auto branch = FindMaxDeltaD(data.graph);
    if (branch.empty()) return;
    IncludeEdge(data, branch[0]);
    if (!IsSimple(data.graph)) {
      ExcludeEdge(data, branch);
    }
  }

  static inline bool IsSimple(const Graph& graph) {
    return graph.data.size() - graph.crossed_rows.size() <= 2;
  }

  static PointSet FindNullEdges(const Graph& graph) {
    PointSet set;
    for_column(0, _size, {for_row(0, _size, {
                 if (_graph[j][i] == 0) {
                   set.emplace(j, i);
                 }
               })});
    return set;
  }

  // Find max Delta d
  // [0] --- coordinates of point
  // [1].y --- column Delta D
  // [1].x --- row Delta D
  static std::vector<Point> FindMaxDeltaD(Graph& graph) {
    auto null_edges = FindNullEdges(graph);
    std::vector<Point> new_branch;
    new_branch.reserve(2);
    int max_d = 0;
    for (const auto& p : null_edges) {
      auto row_edge = FindRowMinimal(graph, p.y, 0, p.x);
      auto row_edge2 =
          FindRowMinimal(graph, p.y, p.x + 1, _size, row_edge.second);
      if (row_edge2.first != -1) {
        row_edge = row_edge2;
      }
      if (row_edge.second == INT_MAX) row_edge.second = 0;

      auto column_edge = FindColumnMinimal(graph, 0, p.y, p.x);
      auto column_edge2 =
          FindColumnMinimal(graph, p.y + 1, _size, p.x, column_edge.second);
      if (column_edge2.first != -1) {
        column_edge = column_edge2;
      }
      if (column_edge.second == INT_MAX) column_edge.second = 0;

      int sum = row_edge.second + column_edge.second;
      if (sum >= max_d) {
        new_branch.clear();
        max_d = sum;
        new_branch.emplace_back(p.y, p.x);
        new_branch[1].x = row_edge.second;
        new_branch[1].y = column_edge.second;
      }
    }
    return new_branch;
  }

  void IncludeEdge(Data data, const Point& edge) {
    print("Including edge: (" << edge.y + 1 << ", " << edge.x + 1 << ")\n\n");
    data.path.PushEdge(edge);
#ifdef DEBUG
    std::cout << "Edges: ";
    for (const auto& e : data.path.edges) {
      std::cout << '(' << e.y + 1 << ", " << e.x + 1 << ") ";
    }
    std::cout << '\n';
    std::cout << "Path: ";
    if (data.path.data.empty()) {
      std::cout << "empty";
    } else {
      for (int i : data.path.data) {
        std::cout << i + 1 << " ";
      }
    }
    std::cout << '\n';
    std::cout << "Length: " << data.path.length << "\n\n";
#endif
    // Check if answer
    if (data.path.data.size() == d_graph.size() + 1) {
      TryOverrideOptimalPath(data.path);
      return;
    }

    // Cross out row and column
    data.graph.crossed_columns.insert(edge.x);
    data.graph.crossed_rows.insert(edge.y);

    CloseBack(data, edge);

    ReduceMatrix(data.graph, data.path.length);

    if (IsStillOptimal(data)) {
      BnB(std::move(data));
    }
  }

  void TryOverrideOptimalPath(const Path& path) {
    if (path.length < optimal_path_.length) {
      optimal_path_ = path;
    }
  }

  static void CloseBack(Data& data, const Point& edge) {
    print("Closing edges:\n");
    d_graph[edge.x][edge.y] = infty;
    print("(" << edge.x + 1 << ", " << edge.y + 1 << ")\n");
    if (data.path.IsConstructed() &&
        data.path.data.size() < data.graph.data.size()) {
      auto p = data.path.GetCyclePoint();
      d_graph[p.y][p.x] = infty;
      print("(" << p.y + 1 << ", " << p.x + 1 << ")\n");
      d_graph[p.x][p.y] = infty;
      print("(" << p.x + 1 << ", " << p.y + 1 << ")\n");
    }
  }

  void ExcludeEdge(Data data, const std::vector<Point>& branch) {
    // Exclude edge
    const auto& edge = branch[0];
    d_graph[edge.y][edge.x] = infty;

    // Reduce matrix
    const auto& d = branch[1];
    if (d.y > 0) {
      data.path.length += d.y;
      Graph& graph = data.graph;
      for_column(0, _size, {
        if (is_valid(j, edge.x)) {
          d_graph[j][edge.x] -= d.y;
        }
      })
    }
    if (d.x > 0) {
      data.path.length += d.x;
      Graph& graph = data.graph;
      for_row(0, _size, {
        if (is_valid(edge.y, i)) {
          d_graph[edge.y][i] -= d.x;
        }
      })
    }

    // If path is still optimal go to next step
    if (IsStillOptimal(data)) {
      BnB(std::move(data));
    }
  }

  static PointSet ReduceMatrix(Graph& graph, int& d) {
    auto null_edges = ReduceRows(graph, d);
    auto column_set = ReduceColumns(graph, d);
    null_edges.insert(column_set.cbegin(), column_set.cend());
    print("Matrix reduced:\n");
    print_graph(graph);
    print("D = " << d << "\n\n");
    return null_edges;
  }

  static PointSet ReduceRows(Graph& graph, int& d) {
    PointSet set;
    for_column(0, _size, {
      auto min_edge = FindRowMinimal(graph, j, 0, _size);
      int min_weight = min_edge.second;
      d += min_weight;
      ReduceRow(graph, j, min_weight, set);
    });
    return set;
  }

  static PointSet ReduceColumns(Graph& graph, int& d) {
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

  [[nodiscard]] bool IsStillOptimal(const Data& data) const {
    // TODO: check if path exists?

    // TODO: Estimations
    if (!IsSimple(data.graph)) {
    }

#ifdef DEBUG
    if (data.path.length > approximate_path_length_ ||
        data.path.length >= optimal_path_.length) {
      std::cout << "Path is not optimal!\n\n";
      return false;
    } else {
      return true;
    }
#endif

    return data.path.length <= approximate_path_length_ &&
           data.path.length < optimal_path_.length;
  }

 private:
  int approximate_path_length_ = INT_MAX;
  Path optimal_path_;
};

#endif  // COMMIS_VOYAGEUR__SOLUTION_H_