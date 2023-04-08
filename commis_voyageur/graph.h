#ifndef COMMIS_VOYAGEUR__GRAPH_H_
#define COMMIS_VOYAGEUR__GRAPH_H_

#include <functional>
#include <unordered_set>
#include <vector>

#define infty (-1)

struct Graph {
  std::vector<std::vector<int>> data;
  std::unordered_set<int> crossed_rows;
  std::unordered_set<int> crossed_columns;
};

#define for_column(y0, y1, expr)                  \
  for (int j = y0; j < (y1); ++j) {               \
    if (graph.crossed_rows.contains(j)) continue; \
    expr;                                         \
  }

#define for_row(x0, x1, expr)                        \
  for (int i = x0; i < (x1); ++i) {                  \
    if (graph.crossed_columns.contains(i)) continue; \
    expr;                                            \
  }

inline void ForColumn(Graph& graph, const std::function<void(int)>& function,
                      int y0 = -1, int y1 = -1) {
  if (y0 == -1) y0 = 0;
  if (y1 == -1) y1 = int(graph.data.size());

  for (int i = y0; i < y1; ++i) {
    if (graph.crossed_rows.contains(i)) continue;
    function(i);
  }
}

inline void ForRow(Graph& graph, const std::function<void(int)>& function,
                   int x0 = -1, int x1 = -1) {
  if (x0 == -1) x0 = 0;
  if (x1 == -1) x1 = int(graph.data.size());

  for (int i = x0; i < x1; ++i) {
    if (graph.crossed_columns.contains(i)) continue;
    function(i);
  }
}

#define is_valid(y, x) _graph[y][x] > -1
#define _point(y, x) _graph[y][x]

#define _graph graph.data
#define _size int(graph.data.size())

struct Point {
  Point() = default;
  Point(int y, int x) : y(y), x(x) {}
  auto operator<=>(const Point& point) const = default;

  int y = 0;
  int x = 0;
};

struct PointHash {
  std::size_t operator()(const Point& point) const {
    return std::hash<int>()(point.y + point.x);
  }
};

using PointSet = std::unordered_set<Point, PointHash>;

struct Path {
  std::vector<Point> edges;
  std::vector<int> data;
  int length;
};

#define _path path.data

#endif  // COMMIS_VOYAGEUR__GRAPH_H_