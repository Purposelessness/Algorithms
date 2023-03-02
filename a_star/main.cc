#include <algorithm>
#include <iostream>
#include <sstream>

#include "solution.h"

int main() {
  std::string buf;
  Graph graph{};
  std::getline(std::cin, buf);
  std::stringstream ss{std::move(buf)};
  ss >> graph.start >> graph.finish;
  while (std::getline(std::cin, buf) && !buf.empty() && buf != "\n") {
    char a = 0;
    char b = 0;
    float len = 0;
    ss.str(std::move(buf));
    ss.clear();
    ss >> a >> b >> len;
    graph.data[a].data = a;
    graph.data[a].paths.push_back({a, b, len});
  }
//  for (const auto& p : graph.data) {
//    const Node& n = p.second;
//    printf("%c:\n", n.data);
//    for (const auto& path : n.paths) {
//      printf("\t%c-%c : %f\n", path.a, path.b, path.length);
//    }
//  }
  Solution solution;
  std::cout << solution.Solve(graph).data();
  return 0;
}
