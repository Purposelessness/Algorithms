#include <iostream>

#include "backtrack.h"

int main() {
  scanf("%d %d", &a, &b);
  clock_t time = clock();
  Backtrack();
  printf("Test time: %lf\n", double(clock() - time) / CLOCKS_PER_SEC);
  return 0;
}
