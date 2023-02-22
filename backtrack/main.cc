#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err34-c"
#pragma ide diagnostic ignored "google-readability-casting"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-vararg"
#include <iostream>

#include "backtrack.h"
#include "debug.h"

int main() {
  DEBUG_EXPR(auto d = Debug());
  int a, b;
  scanf("%d %d", &a, &b);
  DEBUG_EXPR(clock_t time = clock());
  Solution s(a, b);
  s.Backtrack();
  DEBUG_EXPR(
      printf("Test time: %lf\n", double(clock() - time) / CLOCKS_PER_SEC));
  return 0;
}

#pragma clang diagnostic pop