#include "debug.h"

inline void PrintColored(const std::string& text, const std::string& color) {
  std::cout << color << text << "\033[0m";
}

inline void PrintColored(char c, const std::string& color) {
  std::cout << color << c << "\033[0m";
}

inline void PrintColored(int i, const std::string& color) {
  std::cout << color << i << "\033[0m";
}

void DisplayPi(const std::string& word, const std::vector<int>& pi, int i,
               int j, bool success) {
  std::string color = success ? "\033[32m" : "\033[31m";
  for (int n = 0; n < int(word.size()); ++n) {
    if (n == i || n == j) {
      PrintColored(word[n], color);
    } else {
      std::cout << word[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(pi.size()); ++n) {
    if (n == i) {
      PrintColored(pi[n], "\033[34m");
    } else {
      std::cout << pi[n];
    }
  }
  std::cout << '\n';
}

void DisplayPiTransition(const std::string& word, const std::vector<int>& pi,
                         int i, int j, int j_next, int pi_j) {
  for (int n = 0; n < int(word.size()); ++n) {
    if (n == i || n == j) {
      PrintColored(word[n], "\033[31m");
    } else if (n == j_next) {
      PrintColored(word[n], "\033[33m");
    } else {
      std::cout << word[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(pi.size()); ++n) {
    if (n == i) {
      PrintColored(pi[n], "\033[34m");
    } else if (n == pi_j) {
      PrintColored(pi[n], "\033[33m");
    } else {
      std::cout << pi[n];
    }
  }
  std::cout << '\n';
}

void DisplaySearch(const std::string& text, const std::string& word,
                   const std::vector<int>& pi, int i, int j, bool success) {
  std::string color = success ? "\033[32m" : "\033[31m";
  for (int n = 0; n < int(text.size()); ++n) {
    if (n == i) {
      PrintColored(text[n], color);
    } else {
      std::cout << text[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(word.size()); ++n) {
    if (n == j) {
      PrintColored(word[n], color);
    } else {
      std::cout << word[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(pi.size()); ++n) {
    if (n == j) {
      PrintColored(pi[n], "\033[34m");
    } else {
      std::cout << pi[n];
    }
  }
  std::cout << '\n';
}

void DisplaySearchTransition(const std::string& text, const std::string& word,
                             const std::vector<int>& pi, int i, int j,
                             int j_next, int pi_j) {
  for (int n = 0; n < int(text.size()); ++n) {
    if (n == i) {
      PrintColored(text[n], "\033[31m");
    } else {
      std::cout << text[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(word.size()); ++n) {
    if (n == j) {
      PrintColored(word[n], "\033[31m");
    } else if (n == j_next) {
      PrintColored(word[n], "\033[33m");
    } else {
      std::cout << word[n];
    }
  }
  std::cout << '\n';
  for (int n = 0; n < int(pi.size()); ++n) {
    if (n == j) {
      PrintColored(pi[n], "\033[34m");
    } else if (n == pi_j) {
      PrintColored(pi[n], "\033[33m");
    } else {
      std::cout << pi[n];
    }
  }
  std::cout << '\n';
}

void DisplaySearchFound(const std::string& text, const std::string& word,
                        const std::vector<int>& pi, int len, int i, int j_next,
                        int pi_j) {
  int pattern_start = i - len + 1;
  for (int n = 0; n < pattern_start; ++n) {
    std::cout << text[n];
  }
  for (int n = pattern_start; n < i + 1; ++n) {
    PrintColored(text[n], "\033[32m");
  }
  for (int n = i + 1; n < int(text.size()); ++n) {
    std::cout << text[n];
  }
  std::cout << '\n';
  for (int n = 0; n < j_next; ++n) {
    PrintColored(word[n], "\033[32m");
  }
  PrintColored(word[j_next], "\033[34m");
  for (int n = j_next + 1; n < int(word.size()); ++n) {
    PrintColored(word[n], "\033[32m");
  }
  std::cout << '\n';
  for (int n = 0; n < int(pi.size()); ++n) {
    if (n == pi_j) {
      PrintColored(pi[n], "\033[34m");
    } else if (n == j_next) {
      PrintColored(pi[n], "\033[34m");
    } else {
      std::cout << pi[n];
    }
  }
  std::cout << '\n';
}
