#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-readability-casting"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef BACKTRACK__DEBUG_H_
#define BACKTRACK__DEBUG_H_

// #define DEBUG_EX
// #define DEBUG_RECT
// #define DEBUG
// #define FDEBUG

#include <bitset>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "square.h"

class Debug {
 public:
  Debug() {
#ifdef FDEBUG
    outFile_ = std::freopen("out.txt", "w", stdout);
#endif
  }

  ~Debug() {
    if (outFile_ != nullptr) {
      std::fclose(stdout);
    }
  }

 private:
  FILE* outFile_ = nullptr;
};

#ifdef DEBUG_EX
#define DEBUG_EXPR(x) x;
#else
#define DEBUG_EXPR(x)
#endif

#ifdef DEBUG
#define PRINT(x) std::cout << x;
#else
#define PRINT(x)
#endif

void printRect(int w, int h, int answer_size, Square* answer) {
  std::vector<std::vector<int>> matrix(h);
  for (int i = 0; i < h; ++i) {
    matrix[i].resize(w);
  }
  for (int i = 0; i < answer_size; ++i) {
    auto rect = answer[i];
    for (int x = rect.x - 1; x < rect.x - 1 + rect.size; ++x) {
      for (int y = rect.y; y < rect.y + rect.size; ++y) {
        matrix[y][x] = rect.size;
      }
    }
  }
  std::cout << "\nSquared rect:\n";
  for (const auto& i : matrix) {
    std::copy(i.cbegin(), i.cend(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
  }
  std::cout << '\n';
}

#ifdef DEBUG_RECT
#define PRINT_RECT(w, h, x, y) printRect(w, h, x, y)
#else
#define PRINT_RECT(w, h, x, y)
#endif

void printFlow(int* levels, int levels_cnt, int a, int b) {
  for (int i = 0; i < levels_cnt; ++i) {
    if (i == a) {
      if (b == -1) {
        std::cout << "\033[33m" << levels[i] << "\033[0m-";
      } else {
        std::cout << "\033[31m" << levels[i] << "\033[0m-";
      }
    } else if (i == b) {
      std::cout << "\033[32m" << levels[i] << "\033[0m-";
    } else {
      std::cout << levels[i] << '-';
    }
  }
}

#ifdef DEBUG
#define PRINT_FLOW(x, y, w, z) printFlow(x, y, w, z)
#else
#define PRINT_FLOW(x, y, w, z)
#endif

void printMatrix(std::bitset<40>* matrix, int a, int b) {
  for (int i = 0; i < b; ++i) {
    auto s = matrix[i].to_string();
    s.resize(a);
    std::cout << s << '\n';
  }
}

#ifdef DEBUG
#define PRINT_MATRIX(x, y, z) printMatrix(x, y, z)
#else
#define PRINT_MATRIX(x, y, z)
#endif

void printPartialAnswer(Square* answer, int size) {
  std::cout << "(h, s): ";
  for (int i = 0; i < size; ++i) {
    printf("(%d %d), ", answer[i].y + 1, answer[i].size);
  }
  std::cout << "\n\n";
}

#ifdef DEBUG
#define PRINT_ANSWER(x, y) printPartialAnswer(x, y)
#else
#define PRINT_ANSWER(x, y)
#endif

#endif  // BACKTRACK__DEBUG_H_
#pragma clang diagnostic pop
