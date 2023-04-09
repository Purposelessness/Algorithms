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
  Graph graph = ReadGraph("0.txt");
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
  printf("Path is: ");
  for (int i : path.data) {
    printf("%d ", i + 1);
  }
  putchar('\n');
  printf("Path length: %d\n", path.length);

  return 0;
}
