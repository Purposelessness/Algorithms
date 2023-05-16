#include <fstream>
#include <iostream>

#include "approximate.h"
#include "graph.h"
#include "solution.h"
#include "solution_consistent_growth.h"

struct InputData {
  Graph graph;
  int starting_point{};
};

InputData ReadGraph(const std::string& filename = "") {
  int n = 0;
  int starting_point = 0;
  Graph graph{};
  if (filename.empty()) exit(EXIT_FAILURE);

  std::ifstream file(filename);
  if (!file.good()) {
    std::cout << "No such file\n";
    exit(EXIT_SUCCESS);
  }
  file >> n >> starting_point;
  --starting_point;
  if (starting_point < 0 || starting_point >= n) {
    std::cout << "Incorrect starting point!\n";
    exit(EXIT_SUCCESS);
  }
  graph.data.resize(n + 1);
  for (int i = 0; i < n; ++i) {
    graph.data[i].resize(n + 1);
    for (int j = 0; j < n; ++j) {
      file >> graph.data[i][j];
    }
    graph.data[i][n] = 0;
  }
  graph.data[n].resize(n + 1);
  std::fill(graph.data[n].begin(), graph.data[n].end(), infty);
  graph.data[n][starting_point] = 0;
  return {graph, starting_point};
}

std::vector<int> NormalizePath(Path& path, int starting_point, int fictitious) {
  std::vector<int> answer;
  answer.resize(path.data.size() - 2);
  if (path.data[0] == fictitious) {
    std::copy(path.data.begin() + 1, path.data.end() - 1, answer.begin());
  } else if (path.data[0] == starting_point) {
    std::copy(path.data.begin(), path.data.end() - 2, answer.begin());
  } else {
    auto it = std::find(path.data.begin(), path.data.end(), starting_point);
    std::copy(it, path.data.end(), answer.begin());
    std::copy(path.data.begin() + 1, it - 1,
              answer.begin() + (path.data.end() - it));
  }
  return answer;
}

bool IsApproximationValid(const Graph& graph, const Path& path) {
  auto j = path.data[0];
  for (int i = 1; i < int(path.data.size()); ++i) {
    auto next = path.data[i];
    if (_graph[j][next] == infty) return false;
    j = next;
  }
  return true;
}

int main() {
  auto p = ReadGraph("chain.txt");
  auto graph = std::move(p.graph);
  auto starting_point = p.starting_point;

  auto approximation = Approximate::SolveForStartPoint(graph, starting_point);
  if (approximation.data.empty()) {
    std::cout << "Cannot approximate.\n";
  } else {
    approximation.data.erase(approximation.data.end() - 2,
                             approximation.data.end());
    if (!IsApproximationValid(graph, approximation)) {
      std::cout << "Cannot approximate.\n";
      approximation.length = INT_MAX;
    } else {
      printf("Approximate path: ");
      for (int i : approximation.data) {
        printf("%d ", i + 1);
      }
      putchar('\n');
      printf("Approximate path length: %d\n\n", approximation.length);
    }
  }

  clock_t time = clock();
  Solution solution;
  auto path = solution.SolveChain(graph, starting_point);
  SolutionSequentialGrowth solution2;
  auto path2 = solution2.SolveChain(graph, starting_point);
  printf("Time: %lf\n", double(clock() - time) / CLOCKS_PER_SEC);

//  if (path.data.empty()) {
//    std::cout << "No path\n";
//    return 0;
//  }
//  auto answer = NormalizePath(path, starting_point, int(graph.data.size() - 1));
//  printf("Answer is: ");
//  for (int i : answer) {
//    printf("%d ", i + 1);
//  }
//  putchar('\n');
//  printf("Path length: %d\n", path.length);

  if (path2.data.empty()) {
    std::cout << "No path\n";
    return 0;
  }
  auto answer2 = NormalizePath(path2, starting_point, int(graph.data.size() - 1));
  printf("Answer is: ");
  for (int i : answer2) {
    printf("%d ", i + 1);
  }
  putchar('\n');
  printf("Path length: %d\n", path2.length);

  return 0;
}
