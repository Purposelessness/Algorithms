#ifndef AHO_CORASICK__AHO_CORASICK_H_
#define AHO_CORASICK__AHO_CORASICK_H_

#include <cstring>
#include <queue>
#include <string>
#include <vector>

constexpr int kN = 26;  // size of alphabet
constexpr char kA = 'a';

class AhoCorasick {
 public:
  struct Node {
    Node();

    int to[kN]{};
    bool term = false;
    int link = -1;
    std::string str;
  };

  AhoCorasick();

  std::vector<std::pair<int, int>> Solve(const std::string& text,
                                         const std::vector<std::string>& words);

  void AddToTrie(const std::string& s);

  void ResolveTrie();

 private:
  std::vector<Node> trie_;
};

#endif  // AHO_CORASICK__AHO_CORASICK_H_