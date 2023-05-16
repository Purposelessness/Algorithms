#ifndef ALGORITHMS_QUICKSELECT_H_
#define ALGORITHMS_QUICKSELECT_H_

#include <random>
#include <vector>

#include "blum_floyd_pratt_rivest_tarjan.h"

/// returns k-th minimum element in
/// range = [begin, end], where
/// begin <= a <= k - 1 => vec[a] < vec[k],
/// k + 1 <= b <= end => vec[b] >= vec[k]
int Quickselect(std::vector<int>& vec, int k, int begin = -1, int end = -1) {
  if (begin == -1 || end == -1) {
    begin = 0;
    end = static_cast<int>(vec.size()) - 1;
  }

  const int kBFPRTLen = 100;
  size_t idx = 0;
  if (vec.size() > kBFPRTLen) {
    idx = BlumFloydPrattRivestTarjan(vec, begin, end);
  } else {
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<std::mt19937::result_type> range(begin, end);
    idx = range(rng);
  }
  // or
  // auto idx = (end + begin) / 2;

  int pivot = vec[idx];

  std::swap(vec[idx], vec[end]);
  int l = begin;
  for (int i = begin; i < end; ++i) {
    if (vec[i] < pivot) {
      std::swap(vec[l++], vec[i]);
    }
  }
  std::swap(vec[end], vec[l]);

  if (k == l) {
    return vec[k];
  }
  if (k < l) {
    return Quickselect(vec, k, begin, l - 1);
  }
  return Quickselect(vec, k, l + 1, end);
}

#endif  // ALGORITHMS_QUICKSELECT_H_
