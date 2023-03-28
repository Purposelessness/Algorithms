#ifndef COMMIS_VOYAGEUR__GRAPH_H_
#define COMMIS_VOYAGEUR__GRAPH_H_

#include <vector>
#include <unordered_set>

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

#define is_valid(y, x) _graph[y][x] > -1

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

#endif  // COMMIS_VOYAGEUR__GRAPH_H_