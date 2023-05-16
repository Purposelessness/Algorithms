#ifndef COMMIS_VOYAGEUR__APPROXIMATE_H_

#include <climits>

#include "debug.h"
#include "graph.h"

class Approximate {
 public:
  static Path Solve(const Graph& graph) {
    int max_length = INT_MAX;
    Path optimal_path;
    for (int i = 0; i < int(_graph.size()); ++i) {
      auto path = SolveForStartPoint(graph, i);
      if (path.length < max_length) {
        max_length = path.length;
        optimal_path = path;
      }
    }
    return optimal_path;
  }

  static Path SolveForStartPoint(Graph graph, int start_point) {
    Path path{};
    // include starting point in path
    graph.crossed_columns.emplace(start_point);
    print("- Approximation\n\n");
    print("Graph:\n");
    print_graph(graph);
    _path.push_back(start_point);
    _path.reserve(_graph.size());

    // while length of path is less than vertices count
    while (_path.size() < _graph.size()) {
      // if no edge is found --- no path can be found
      if (!SelectNext(graph, path)) {
        return {};
      }
    }

    // convert edges to path
    int j = start_point;
    for (int i = 1; i < int(path.data.size()); ++i) {
      int k = path.data[i];
      path.length += _graph[j][k];
      j = k;
    }
    path.length += _graph[j][start_point];
    path.data.push_back(start_point);

    return path;
  }

  static bool SelectNext(Graph& graph, Path& path) {
    int min_weight = INT_MAX;
    Point min_point;
    int k = 0;
    // for every included in path vertex
    print("Edges available:\n");
#ifdef DEBUG
    printf("     ");
    foreach_row({ printf("%3d ", i + 1); });
    putchar('\n');
#endif

    for (int z = 0; z < int(_path.size()); ++z) {
      int t = path.data[z];
#ifdef DEBUG
      printf("%3d: ", t + 1);
#endif
      // find minimum edge
      foreach_row({
        if (is_valid(t, i)) {
#ifdef DEBUG
          printf("%3d ", _point(t, i));
#endif
          if (_point(t, i) <= min_weight) {
            min_weight = _point(t, i);
            min_point = Point(t, i);
            k = z;
          }
        } else {
          print("inf ");
        }
      });
      print("\n");
    }
    // no edge found
    if (min_weight == INT_MAX) {
      return false;
    }

    // select edge --- add it to path
    print("Selected edge: " << _path[k] + 1 << '-' << min_point.x + 1 << "\n");
    _path.emplace(_path.begin() + k + 1, min_point.x);
    print("Path is: ");
    for (int i : _path) {
      printf("%d ", i + 1);
    }
    print("\n\n");
    graph.crossed_columns.emplace(min_point.x);
    return true;
  }

 private:
};

#endif  // COMMIS_VOYAGEUR__APPROXIMATE_H_