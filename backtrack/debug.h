#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#ifndef BACKTRACK__DEBUG_H_
#define BACKTRACK__DEBUG_H_

#define DEBUG
//#define FDEBUG

#include <iostream>
#include <memory>
#include <string>

class Debug {
 public:
  Debug() {
#ifdef FDEBUG
    out_file_ = std::freopen("out.txt", "w", stdout);
#endif
  }

  ~Debug() {
    if (out_file_ != nullptr) {
      std::fclose(stdout);
    }
  }

 private:
  FILE* out_file_ = nullptr;
};

#ifdef DEBUG
#define PRINT(x) \
  std::cout << x << '\n';
#else
#define PRINT(x)
#endif

#endif  // BACKTRACK__DEBUG_H_
#pragma clang diagnostic pop