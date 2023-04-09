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

#define _erase(c, x) c.erase(std::remove(c.begin(), c.end(), x), c.end())

struct Path {
  [[nodiscard]] bool IsConstructed() const {
    return !data.empty();
  }

  void TryContinuePath() {
    int begin = data[0];
    int end = data[data.size() - 1];
    while (!edges.empty()) {
      auto it = std::find_if(
          edges.begin(), edges.end(),
          [begin, end](const Point& p) { return p.x == begin || p.y == end; });
      if (it == edges.end()) {
        return;
      }
      if (it->x == begin) {
        data.insert(data.begin(), it->y);
        begin = data[0];
      } else {
        data.push_back(it->x);
        end = data[data.size() - 1];
      }
      edges.erase(it);
    }
  }

  bool TryMakePath() {
    if (edges.size() < 2) {
      return false;
    }
    for (const auto& e : edges) {
      auto copy = edges;
      std::vector<int> path{e.y, e.x};
      _erase(copy, e);
      while (!copy.empty()) {
        int next = path[path.size() - 1];
        auto it = std::find_if(copy.begin(), copy.end(),
                               [next](const Point& p) { return p.y == next; });
        if (it == copy.end()) {
          break;
        }
        path.push_back(it->x);
        copy.erase(it);
      }
      if (copy.empty()) {
        edges.clear();
        data = path;
        return true;
      }
    }
    return false;
  }

  [[nodiscard]] Point GetCyclePoint() {
    if (!IsConstructed()) {
      return {-1, -1};
    }
    return {data[data.size() - 1], data[0]};
  }

  void PushEdge(const Point& edge) {
    edges.push_back(edge);
    if (data.empty()) {
      TryMakePath();
    } else {
      TryContinuePath();
    }
  }

  std::vector<Point> edges;
  std::vector<int> data;
  int length = 0;
};

#define _path path.data

#endif  // COMMIS_VOYAGEUR__GRAPH_H_