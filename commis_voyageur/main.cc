#include <fstream>
#include <iostream>

#include "approximate.h"
#include "graph.h"
#include "solution.h"

Graph ReadGraph(const std::string& filename = "") {
  int n = 0;
  Graph graph{};
  if (filename.empty()) {
    std::cin >> n;
    graph.data.resize(n);
    for (int i = 0; i < n; ++i) {
      graph.data[i].resize(n);
      for (int j = 0; j < n; ++j) {
        std::cin >> graph.data[i][j];
      }
    }
  } else {
    std::ifstream file(filename);
    if (!file.good()) {
      return graph;
    }
    file >> n;
    graph.data.resize(n);
    for (int i = 0; i < n; ++i) {
      graph.data[i].resize(n);
      for (int j = 0; j < n; ++j) {
        file >> graph.data[i][j];
      }
    }
  }
  return graph;
}

int main() {
  //    Graph graph = {.data{{infty, 25, 40, 31, 27},
  //                         {5, infty, 17, 30, 25},
  //                         {19, 15, infty, 6, 1},
  //                         {9, 50, 24, infty, 6},
  //                         {22, 8, 7, 10, infty}}};
  Graph graph = {.data{{infty, 27, 43, 16, 30, 26},
                       {7, infty, 16, 1, 30, 25},
                       {20, 13, infty, 35, 5, 0},
                       {21, 16, 25, infty, 18, 18},
                       {12, 46, 27, 48, infty, 5},
                       {23, 5, 5, 9, 5, infty}}};
  //  Graph graph = ReadGraph("1.txt");
  Solution solution;
  auto approximation = Approximate::Solve(graph);
  printf("Approximate path: ");
  for (int i : approximation.data) {
    printf("%d ", i);
  }
  putchar('\n');
  printf("Approximate path length: %d\n\n", approximation.length);
  clock_t time = clock();
  auto path = solution.Solve(graph, approximation.length);
  printf("Time: %lf\n", double(clock() - time) / CLOCKS_PER_SEC);
  printf("%d\n", path.length);
  for (int i : path.data) {
    printf("%d ", i + 1);
  }

  return 0;
}
