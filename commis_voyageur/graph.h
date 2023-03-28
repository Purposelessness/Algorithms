#ifndef COMMIS_VOYAGEUR__GRAPH_H_
#define COMMIS_VOYAGEUR__GRAPH_H_

#include <vector>

#define infty (-1)

struct Graph {
  auto operator<=>(const Graph& graph) const = default;

  std::vector<std::vector<int>> data;
};

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

#endif  // COMMIS_VOYAGEUR__GRAPH_H_