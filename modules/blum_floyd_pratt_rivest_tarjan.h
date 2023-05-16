#ifndef ALGORITHMS_BLUM_FLOYD_PRATT_RIVES_TARJAN_H_
#define ALGORITHMS_BLUM_FLOYD_PRATT_RIVES_TARJAN_H_

#include <algorithm>
#include <vector>

int FindMedian(std::vector<int>& vec, int begin, int end);

/// returns index of 'median of median' in
/// range = [begin, end]
int BlumFloydPrattRivestTarjan(std::vector<int>& vec, int begin = -1,
                               int end = -1) {
  if (begin == -1 || end == -1) {
    begin = 0;
    end = static_cast<int>(vec.size()) - 1;
  }
  const int kLen = 5;
  if (end - begin + 1 <= kLen) {
    return FindMedian(vec, begin, end);
  }
  int l = begin;
  int i = begin;
  for (; i <= end - (kLen - 1); i += kLen) {
    std::swap(vec[l++], vec[FindMedian(vec, i, i + (kLen - 1))]);
  }
  if (i <= end) {
    std::swap(vec[l++], vec[FindMedian(vec, i, end)]);
  }
  return BlumFloydPrattRivestTarjan(vec, begin, l - 1);
}

int FindMedian(std::vector<int>& vec, int begin, int end) {
  std::sort(vec.begin() + begin, vec.begin() + end + 1);
  int n = end - begin + 1;
  return begin + n / 2;
}

inline int MedianOfMedian(std::vector<int>& vec, int begin = -1, int end = -1) {
  return BlumFloydPrattRivestTarjan(vec, begin, end);
}

#endif  // ALGORITHMS_BLUM_FLOYD_PRATT_RIVES_TARJAN_H_
