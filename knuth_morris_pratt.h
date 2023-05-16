#ifndef ALGORITHMS_KNUTH_MORRIS_PRATT_H_
#define ALGORITHMS_KNUTH_MORRIS_PRATT_H_

#include <string>
#include <vector>

std::vector<int> PrefixFunction(const std::string& str);

std::vector<int> KnuthMorrisPratt(const std::string& text,
                                  const std::string& pattern) {
  std::vector<int> answer;
  auto pi = PrefixFunction(pattern);
  int j = 0;
  for (int i = 0; i < static_cast<int>(text.size()); ++i) {
    while (j > 0 && text[i] != pattern[j]) {
      j = pi[j - 1];
    }
    if (text[i] == pattern[j]) {
      ++j;
      if (j == static_cast<int>(pattern.size())) {
        answer.push_back(i - j + 1);
        j = pi[j - 1];
      }
    }
  }
  return answer;
}

std::vector<int> PrefixFunction(const std::string& str) {
  std::vector<int> pi(str.size());
  int j = 0;
  for (int i = 1; i < static_cast<int>(str.size()); ++i) {
    while (j > 0 && str[i] != str[j]) {
      j = pi[j - 1];
    }
    if (str[i] == str[j]) {
      ++j;
    }
    pi[i] = j;
  }
  return pi;
}

#endif  // ALGORITHMS_KNUTH_MORRIS_PRATT_H_
