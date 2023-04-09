#include "debug.h"

#include <iostream>

void PrintGraph(const Graph& graph) {
  for (int i = -1; i < int(graph.data.size()); ++i) {
    if (graph.crossed_columns.contains(i)) continue;
    printf("%3d ", i + 1);
  }
  putchar('\n');
  for (int i = 0; i < int(graph.data.size()); ++i) {
    if (graph.crossed_rows.contains(i)) continue;
    printf("%3d ", i + 1);
    for (int j = 0; j < int(graph.data.size()); ++j) {
      if (graph.crossed_columns.contains(j)) continue;
      printf("%3d ", _graph[i][j]);
    }
    putchar('\n');
  }
}