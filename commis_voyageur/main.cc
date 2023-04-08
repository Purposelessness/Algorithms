#include <iostream>

#include "approximate.h"
#include "graph.h"
#include "solution.h"

int main() {
  //  Graph graph = {.data{{infty, 25, 40, 31, 27},
  //                       {5, infty, 17, 30, 25},
  //                       {19, 15, infty, 6, 1},
  //                       {9, 50, 24, infty, 6},
  //                       {22, 8, 7, 10, infty}}};
  //  Solution solution;
  //  auto path = solution.Solve(graph);
  //  for (const auto& e : path.edges) {
  //    printf("%d %d\n", e.y, e.x);
  //  }
  Graph graph = {.data{{infty, 27, 43, 16, 30, 26},
                       {7, infty, 16, 1, 30, 25},
                       {20, 13, infty, 35, 5, 0},
                       {21, 16, 25, infty, 18, 18},
                       {12, 46, 27, 48, infty, 5},
                       {23, 5, 5, 9, 5, infty}}};
  auto approximation = Approximate::Solve(graph, 1);

  return 0;
}
