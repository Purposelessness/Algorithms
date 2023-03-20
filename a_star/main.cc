#include <iostream>
#include <iterator>
#include <sstream>

#include "astar.h"
#include "config.h"
#include "greedy.h"

void PrintAnswer(std::vector<int>& vec, bool is_char) {
  if (is_char) {
    for (auto c : vec) {
      std::cout << static_cast<char>(c);
    }
  } else {
    for (auto d : vec) {
      std::cout << d;
    }
  }
  std::cout << '\n';
}

int main() {
  Graph graph{};
  std::string buf;
  std::getline(std::cin, buf);
  std::stringstream ss{std::move(buf)};
  TYPE a = 0;
  TYPE b = 0;
  ss >> a >> b;
  graph.start = a;
  graph.finish = b;
  while (std::getline(std::cin, buf), !buf.empty() && buf != "\n") {
    float len = 0;
    ss.str(std::move(buf));
    ss.clear();
    ss >> a >> b >> len;
    graph.data[a].data = int(a);
    graph.data[a].paths.push_back({a, b, len});
    if (!std::cin) {
      break;
    }
  }
  Greedy greedy;
  auto greedy_result = greedy.solve(graph);
  if (greedy_result.empty()) {
    std::cout << "Greedy: no answer\n";
  } else {
    std::cout << "Greedy: ";
    PrintAnswer(greedy_result, IS_CHAR);
  }
  PRINT('\n')
  AStar a_star;
  auto a_star_result = a_star.solve(graph);
  if (a_star_result.empty()) {
    std::cout << "A*: no answer\n";
  } else {
    std::cout << "A*: ";
    PrintAnswer(a_star_result, IS_CHAR);
  }
  return 0;
}
