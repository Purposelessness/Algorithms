#include <algorithm>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

#include "blum_floyd_pratt_rivest_tarjan.h"

// 'k'-th element in 0-indexation
// range = [begin, end]
int Quickselect(std::vector<int>& vec, int k, int begin = -1, int end = -1) {
  if (begin == -1 || end == -1) {
    begin = 0;
    end = static_cast<int>(vec.size()) - 1;
  }

  auto idx = BlumFloydPrattRivestTarjan(vec, begin, end);
  // or
  // std::mt19937 rng(std::random_device{}());
  // std::uniform_int_distribution<std::mt19937::result_type> range(beg, end);
  // auto idx = range(rng);
  // or
  // auto idx = (end + begin) / 2;
  int pivot = vec[idx];

  std::swap(vec[idx], vec[end]);
  int l = begin;
  for (int i = l; i < end; ++i) {
    if (vec[i] < pivot) {
      std::swap(vec[l++], vec[i]);
    }
  }
  std::swap(vec[end], vec[l]);

  if (k == l) {
    return vec[l];
  }
  if (k < l) {
    return Quickselect(vec, k, begin, l - 1);
  }
  return Quickselect(vec, k, l + 1, end);
}

int main() {
  int k = 5;
  std::vector<int> vec{7, 6, 8, 4, 5, 2, 9, 1, 2, 5, 4};
  std::vector<int> ans = vec;
  std::sort(ans.begin(), ans.end());
  std::cout << Quickselect(vec, k) << '\n';

  return 0;
}
