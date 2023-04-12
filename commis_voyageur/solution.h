#ifndef COMMIS_VOYAGEUR__SOLUTION_H_
#define COMMIS_VOYAGEUR__SOLUTION_H_

#include <climits>
#include <queue>
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
  // Main function to solve commis-voyageur task
  [[nodiscard]] Path Solve(const Graph& graph, int estimated_len = INT_MAX) {
    optimal_path_.length = estimated_len;
    Data data{graph, {}};
    print("Graph:\n");
    print_graph(graph);
    print("\n");
    ReduceMatrix(data.graph, data.path.length);
    BnB(std::move(data));
    return optimal_path_;
  }

  // Main function to solve commis-voyageur chain task
  [[nodiscard]] Path SolveChain(const Graph& graph, int starting_point,
                                int estimated_len = INT_MAX) {
    optimal_path_.length =
        estimated_len == INT_MAX ? estimated_len : estimated_len + 1;
    Data data{graph, {}};
    Point exit_edge = {_size - 1, starting_point};

    // Select path from fictitious vertex to starting point
    data.path.PushEdge(exit_edge);
    data.graph.crossed_columns.insert(exit_edge.x);
    data.graph.crossed_rows.insert(exit_edge.y);
    data._graph[exit_edge.x][exit_edge.y] = infty;

    print("Graph:\n");
    print_graph(graph);
    print("\n");
    ReduceMatrix(data.graph, data.path.length);
    BnB(std::move(data));
    return optimal_path_;
  }

  // Main recursion function
  void BnB(Data data) {
    bool status = false;
    // Find new edge to include/exclude
    auto branch = FindMaxDeltaD(data.graph, status);
    if (!status) return;
    IncludeEdge(data, branch.first);
    // Exclude edge only if matrix is greater than 2x2
    if (!IsSimple(data.graph)) {
      ExcludeEdge(data, branch);
    }
  }

  static inline bool IsSimple(const Graph& graph) {
    return graph.data.size() - graph.crossed_rows.size() <= 2;
  }

  static PointSet FindNullEdges(const Graph& graph) {
    PointSet set;
    foreach_column({
      foreach_row({
        if (_graph[j][i] == 0) {
          set.emplace(j, i);
        }
      });
    });
    return set;
  }

  // Find max Delta d
  // [0] --- coordinates of point
  // [1].y --- column Delta D
  // [1].x --- row Delta D
  static std::pair<Point, Point> FindMaxDeltaD(Graph& graph, bool& status) {
    auto null_edges = FindNullEdges(graph);
    std::pair<Point, Point> new_branch;
    int max_d = 0;
    // Check all null edges and find one with max d
    for (const auto& p : null_edges) {
      auto row_edge = FindRowMinimal(graph, p.y, 0, p.x);
      auto row_edge2 =
          FindRowMinimal(graph, p.y, p.x + 1, _size, row_edge.second);
      if (row_edge2.first != -1) row_edge = row_edge2;
      if (row_edge.second == INT_MAX) row_edge.second = 0;

      auto column_edge = FindColumnMinimal(graph, p.x, 0, p.y);
      auto column_edge2 =
          FindColumnMinimal(graph, p.x, p.y + 1, _size, column_edge.second);
      if (column_edge2.first != -1) column_edge = column_edge2;
      if (column_edge.second == INT_MAX) column_edge.second = 0;

      int sum = row_edge.second + column_edge.second;
      if (sum >= max_d) {
        status = true;
        max_d = sum;
        new_branch.first = {p.y, p.x};
        new_branch.second = {column_edge.second, row_edge.second};
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

    // Close backwards path
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

  void ExcludeEdge(Data data, const std::pair<Point, Point>& branch) {
    // Exclude edge
    const auto& edge = branch.first;
    d_graph[edge.y][edge.x] = infty;
    print("Closing edge: (" << edge.y + 1 << ", " << edge.x + 1 << ")\n");

    // Reduce matrix
    const auto& d = branch.second;
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
    print("Matrix reduced:\n");
    print_graph(data.graph);
    print("D = " << data.path.length << "\n\n");

    // If path is still optimal go to next step
    if (IsStillOptimal(data)) {
      BnB(std::move(data));
    }
  }

  static void ReduceMatrix(Graph& graph, int& d) {
    ReduceRows(graph, d);
    ReduceColumns(graph, d);
    print("Matrix reduced:\n");
    print_graph(graph);
    print("D = " << d << "\n\n");
  }

  static void ReduceRows(Graph& graph, int& d) {
    for_column(0, _size, {
      auto min_edge = FindRowMinimal(graph, j);
      int min_weight = min_edge.second;
      if (min_weight == INT_MAX) continue;
      d += min_weight;
      ReduceRow(graph, j, min_weight);
    });
  }

  static void ReduceColumns(Graph& graph, int& d) {
    for_row(0, _size, {
      auto min_edge = FindColumnMinimal(graph, i);
      int min_weight = min_edge.second;
      if (min_weight == INT_MAX) continue;
      d += min_weight;
      ReduceColumn(graph, i, min_weight);
    });
  }

  static inline void ReduceRow(Graph& graph, int y, int weight) {
    for_row(0, _size, {
      if (is_valid(y, i)) {
        _graph[y][i] -= weight;
      }
    });
  }

  static inline void ReduceColumn(Graph& graph, int x, int weight) {
    for_column(0, _size, {
      if (is_valid(j, x)) {
        _graph[j][x] -= weight;
      }
    });
  }

  // Find minimal edge in row
  // .first --- 'x' position
  // .second --- value
  static std::pair<int, int> FindRowMinimal(Graph& graph, int y, int x0 = -1,
                                            int x1 = -1,
                                            int min_weight = INT_MAX) {
    if (x0 == -1) x0 = 0;
    if (x1 == -1) x1 = int(graph.data.size());
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
  // .first --- 'y' position
  // .second --- value
  static std::pair<int, int> FindColumnMinimal(Graph& graph, int x, int y0 = -1,
                                               int y1 = -1,
                                               int min_weight = INT_MAX) {
    if (y0 == -1) y0 = 0;
    if (y1 == -1) y1 = int(graph.data.size());
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
    // Check 2 estimations
    if (!IsSimple(data.graph)) {
      if (data.path.length + TwoEdgesEstimation(data.graph) >
          optimal_path_.length) {
        print("Two edges estimation failed\n");
        return false;
      }
      if (MinimalSpanningTreeEstimation(data.graph, data.path) > optimal_path_.length) {
        print("Minimal spanning tree estimation failed\n");
        return false;
      }
    }

#ifdef DEBUG
    if (data.path.length >= optimal_path_.length) {
      std::cout << "Path is not optimal: " << data.path.length
                << " >= " << optimal_path_.length << "\n\n";
      return false;
    }

    return true;
#endif

    return data.path.length < optimal_path_.length;
  }

  static int TwoEdgesEstimation(const Graph& graph) {
    int first = INT_MAX;
    int second = INT_MAX;
    foreach_column({
      foreach_row({
        if (is_valid(j, i)) {
          if (_graph[j][i] < second) {
            second = _graph[j][i];
          }
          if (second < first) {
            std::swap(second, first);
          }
        }
      });
    });
    if (first == INT_MAX || second == INT_MAX) {
      return 0;
    }
    return (first + second) / 2;
  }

  static int MinimalSpanningTreeEstimation(const Graph& graph, const Path& path) {
    int length = path.length;
    std::unordered_set<int> vertices;
    vertices.reserve(path.data.size());
    for (int i : path.data) {
      vertices.insert(i);
    }

    auto comparer = [&graph](const Point& l, const Point& r) {
      return _graph[l.y][l.x] > _graph[r.y][r.x];
    };
    std::priority_queue<Point, std::vector<Point>, decltype(comparer)> queue{
        comparer};
    foreach_row({
      foreach_column({
        if (is_valid(j, i)) {
          queue.emplace(j, i);
        }
      });
    });

    while (vertices.size() < graph.data.size()) {
      if (queue.empty()) return 0;
      auto edge = queue.top();
      queue.pop();
      if (vertices.contains(edge.y) && vertices.contains(edge.x)) {
        continue;
      }
      length += _graph[edge.y][edge.x];
      vertices.insert({edge.y, edge.x});
    }
    return length;
  }

 private:
  Path optimal_path_;
};

#endif  // COMMIS_VOYAGEUR__SOLUTION_H_