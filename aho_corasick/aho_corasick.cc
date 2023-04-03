#include "aho_corasick.h"

#include <unordered_map>

AhoCorasick::Node::Node() { std::memset(to, -1, sizeof(to)); }

AhoCorasick::AhoCorasick() { trie_.emplace_back(); }

std::vector<std::pair<int, int>> AhoCorasick::Solve(
    const std::string& text, const std::vector<std::string>& words) {
  std::unordered_map<std::string, int> indices(words.size());
  for (int i = 0; i < int(words.size()); ++i) {
    indices[words[i]] = {i};
  }
  std::vector<std::pair<int, int>> ans;

  for (const auto& w : words) {
    AddToTrie(w);
  }
  ResolveTrie();
  // Searching
  int v = 0;
  for (int i = 0; i < int(text.size()); ++i) {
    unsigned char c = text[i] - kA;
    v = trie_[v].to[c];
    int d = v;
    while (d != 0) {
      if (trie_[d].term) {
        int index = i - trie_[d].depth + 1;
        std::string str(text, index, trie_[d].depth);
        printf("%d %s\n", index, str.data());
        ans.emplace_back(index, indices[str]);
      }
      d = trie_[d].link;
    }
  }
  return ans;
}

void AhoCorasick::AddToTrie(const std::string& s) {
  int v = 0;
  for (unsigned char c : s) {
    c -= kA;
    if (trie_[v].to[c] == -1) {
      trie_.emplace_back();
      trie_[v].to[c] = trie_.size() - 1;
    }
    int u = trie_[v].to[c];
    trie_[u].depth = trie_[v].depth + 1;
    v = u;
  }
  trie_[v].term = true;
}

void AhoCorasick::ResolveTrie() {
  auto& t = trie_;
  t[0].link = 0;
  for (int& c : t[0].to) {
    if (c != -1) {
      continue;
    }
    c = 0;
  }
  std::queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int c = 0; c < kN; ++c) {
      int u = t[v].to[c];
      // The case if .to[c] leads not to child but some upper-level vertex
      // which we already have seen -> it's .link != -1
      if (t[u].link != -1) {
        continue;
      }
      // The case if `u` is a child
      t[u].link = v == 0 ? 0 : t[t[v].link].to[c];
      for (int d = 0; d < kN; ++d) {
        if (t[u].to[d] != -1) {
          continue;
        }
        t[u].to[d] = t[t[u].link].to[d];
      }
      q.push(u);
    }
  }
}
