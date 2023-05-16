#ifndef ALGORITHMS_AHO_CORASICK_H_
#define ALGORITHMS_AHO_CORASICK_H_

#include <cstring>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

static constexpr int kN = 'z' - 'a' + 1;
static constexpr char kA = 'a';

class Trie {
  struct Node {
    Node() { std::memset(to, -1, sizeof(to)); }

    int to[kN]{};
    bool term = false;
    int link = -1;
    int up = 0;
    int depth = 0;
  };

 public:
  Trie() { t_.emplace_back(); }

  std::unordered_map<std::string, std::vector<int>> Search(
      const std::string& text) {
    std::unordered_map<std::string, std::vector<int>> m;
    int v = 0;
    for (int i = 0; i < static_cast<int>(text.size()); ++i) {
      unsigned char c = text[i] - kA;
      v = t_[v].to[c];
      int u = t_[v].term ? v : t_[v].up;
      while (t_[u].term) {
        int j = i - t_[u].depth + 1;
        std::string pattern(text, j, t_[u].depth);
        m[pattern].push_back(j);
        u = t_[u].up;
      }
    }
    return m;
  }

  void Add(const std::string& word) {
    int v = 0;
    for (unsigned char c : word) {
      c -= kA;
      if (t_[v].to[c] == -1) {
        int u = static_cast<int>(t_.size());
        t_.emplace_back();
        t_[u].depth = t_[v].depth + 1;
        t_[v].to[c] = u;
      }
      v = t_[v].to[c];
    }
    t_[v].term = true;
  }

  void Resolve() {
    auto& t = t_;
    t[0].link = t[0].up = 0;
    for (int& c : t[0].to) {
      if (c == -1) {
        c = 0;
      }
    }

    std::queue<int> q;
    q.push(0);
    while (!q.empty()) {
      int v = q.front();
      q.pop();
      for (int c = 0; c < kN; ++c) {
        int u = t[v].to[c];
        if (t[u].link != -1) {
          // The case if vertex 'u' has already been seen
          continue;
        }
        t[u].link = v == 0 ? 0 : t[t[v].link].to[c];
        t[u].up = t[t[u].link].term ? t[u].link : t[t[u].link].up;
        for (int d = 0; d < kN; ++d) {
          if (t[u].to[d] == -1) {
            t[u].to[d] = t[t[u].link].to[d];
          }
        }
        q.push(u);
      }
    }
  }

 private:
  std::vector<Node> t_;
};

std::unordered_map<std::string, std::vector<int>> AhoCorasick(
    const std::string& text, const std::vector<std::string>& patterns) {
  Trie t;
  for (const auto& p : patterns) {
    t.Add(p);
  }
  t.Resolve();
  return t.Search(text);
}

std::unordered_map<std::string, std::vector<int>> SplitMaskedPattern(
    const std::string& pattern, char mask) {
  std::unordered_map<std::string, std::vector<int>> m;
  int j = -1;
  int i = 0;
  for (; i < static_cast<int>(pattern.size()); ++i) {
    if (pattern[i] == mask) {
      if (j != i - 1) {
        std::string word(pattern, j + 1, i - j - 1);
        m[word].push_back(j + 1);
      }
      j = i;
    }
  }
  if (j != i - 1) {
    std::string word(pattern, j + 1, i - j - 1);
    m[word].push_back(j + 1);
  }
  return m;
}

std::vector<int> AhoCorasickMasked(const std::string& text,
                                   const std::string& pattern, char mask) {
  auto m = SplitMaskedPattern(pattern, mask);
  Trie t;
  int cnt = 0;
  for (const auto& p : m) {
    t.Add(p.first);
    cnt += static_cast<int>(p.second.size());
  }
  t.Resolve();

  auto result = t.Search(text);
  std::vector<int> c(text.size());
  for (const auto& p : result) {
    for (int i : p.second) {
      for (int j : m[p.first]) {
        if (i - j >= 0) {
          ++c[i - j];
        }
      }
    }
  }

  std::vector<int> answer;
  for (int i = 0; i < static_cast<int>(c.size() - pattern.size() + 1); ++i) {
    if (c[i] == cnt) {
      answer.push_back(i);
    }
  }
  return answer;
}

#endif  // ALGORITHMS_AHO_CORASICK_H_
