#ifndef COMMIS_VOYAGEUR__APPROXIMATE_H_

#include <climits>

#include "graph.h"

class Approximate {
 public:
  static Path Solve(Graph graph, int start_point) {
    Path path{};
    graph.crossed_columns.emplace(start_point);
    _path.push_back(start_point);
    _path.reserve(_graph.size());

    while (_path.size() < _graph.size()) {
      if (!SelectNext(graph, path)) {
        return {};
      }
    }

    int j = start_point;
    for (int i : _path) {
      path.length += _graph[j][i];
      j = i;
    }

    return path;
  }

  static bool SelectNext(Graph& graph, Path& path) {
    int min_weight = INT_MAX;
    Point min_point;
    int k = 0;
    for (int z = 0; z < int(_path.size()); ++z) {
      int t = path.data[z];
      ForRow(graph, [&](int x) {
        if (is_valid(t, x) && _point(t, x) <= min_weight) {
          min_weight = _point(t, x);
          min_point = Point(t, x);
          k = z;
        }
      });
    }
    if (min_weight == INT_MAX) {
      return false;
    }

    _path.emplace(_path.begin() + k + 1, min_point.x);
    graph.crossed_columns.emplace(min_point.x);
    return true;
  }

 private:
};

#endif  // COMMIS_VOYAGEUR__APPROXIMATE_H_