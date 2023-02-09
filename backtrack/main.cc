#include <iostream>

#include "new_backtrack.h"

int main() {
  std::cin >> n;
  clock_t time = clock();
  Backtrack(n);
  std::cout << "Test time: "
            << static_cast<double>(clock() - time) / CLOCKS_PER_SEC << '\n';
  return 0;
}
