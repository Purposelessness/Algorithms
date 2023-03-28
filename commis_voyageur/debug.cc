#include "debug.h"

#include <iostream>

void PrintGraph(const Graph& graph) {
  for_column(0, _size, {
    for_row(0, _size, { printf("%3d ", _graph[j][i]); });
    putchar('\n');
  });
  putchar('\n');
}