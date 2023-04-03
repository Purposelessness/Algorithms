#include <algorithm>
#include <iostream>

#include "aho_corasick.h"

int main() {
  std::string text;
  int n = 0;
  std::cin >> text;
  std::cin >> n;
  std::vector<std::string> words(n);
  for (int i = 0; i < n; ++i) {
    std::cin >> words[i];
  }
  AhoCorasick aho_corasick;
  auto ans = aho_corasick.Solve(text, words);
  std::sort(ans.begin(), ans.end(), [](const auto& a, const auto& b) {
    if (a.first != b.first) {
      return a.first < b.first;
    }
    return a.second < b.second;
  });
  for (const auto& p : ans) {
    printf("%d %d\n", p.first + 1, p.second + 1);
  }
  return 0;
}
