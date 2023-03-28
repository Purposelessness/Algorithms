#include <iostream>

#include "graph.h"
#include "solution.h"

int main() {
  Graph graph = {.data{{infty, 25, 40, 31, 27},
                       {5, infty, 17, 30, 25},
                       {19, 15, infty, 6, 1},
                       {9, 50, 24, infty, 6},
                       {22, 8, 7, 10, infty}}};
  Solution solution;
  solution.Solve(graph);
  return 0;
}
