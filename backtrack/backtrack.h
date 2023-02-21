#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#pragma ide diagnostic ignored "google-readability-casting"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-goto"
#pragma ide diagnostic ignored "bugprone-narrowing-conversions"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-vararg"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-c-arrays"
#pragma ide diagnostic ignored "cppcoreguidelines-avoid-magic-numbers"
#pragma ide diagnostic ignored "cppcoreguidelines-owning-memory"
#pragma ide diagnostic ignored "cppcoreguidelines-pro-bounds-pointer-arithmetic"
#ifndef BACKTRACK__BACKTRACK_H_
#define BACKTRACK__BACKTRACK_H_

#include <algorithm>
#include <array>
#include <bitset>
#include <climits>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>

#include "debug.h"

#define MAX_PRIME_SQUARE_SIZE 0.9
#define MAX_SQUARE_SIZE 0.9

struct Square {
  Square() = default;
  Square(int y, int size) : y(y), size(size) {}

  int x = -1;
  int y = 0;
  int size = 0;
};

class Solution {
 public:
  Solution(int a, int b, bool print_answer = true)
      : print_answer_(print_answer), a_(a), b_(b) {
    if (a_ < b_) {
      std::swap(a_, b_);
      swap_ = true;
    }
  }

  int Backtrack() {
    max_square_count_ = 6 * std::log2(3 * a_ - 1) - 9;
    InitAnswers();
    answer_ = new Square[max_square_count_]{};
    if (a_ == b_) {
      if (IsPrimeOfTwo(a_) != -1) {
        PrimeTwoSearch();
      } else {
        max_square_size_ =
            IsPrime(a_) ? MAX_PRIME_SQUARE_SIZE * a_ : MAX_SQUARE_SIZE * a_;
        goto default_backtrack;
      }
    } else {
      max_square_size_ = b_;
    default_backtrack:
      second_max_square_size_ = 0.5 * a_;
      partial_answer_ = new Square[max_square_count_]{};
      levels_ = new int[b_ + 1]{};
      levels_[0] = a_;
      BacktrackImpl(0, max_square_size_);
      delete[] levels_;
      delete[] partial_answer_;
      // Finding coords
      answer_left_ = answer_size_;
      x_max_ = 40 - a_;
      rect_ = new std::bitset<40>[b_];
      int i = kAnswersSize;
      do {
        answer_ = PopAnswer();
        --i;
      } while (!FindCoords(0) && i >= 0);
      delete[] rect_;
    }

    if (print_answer_) {
      printf("%d\n", answer_size_);
      if (!swap_) {
        for (int i = 0; i < answer_size_; ++i) {
          printf("%d %d %d\n", answer_[i].x, answer_[i].y + 1, answer_[i].size);
        }
      } else {
        for (int i = 0; i < answer_size_; ++i) {
          printf("%d %d %d\n", answer_[i].y + 1, answer_[i].x, answer_[i].size);
        }
      }
    }

    for (int i = 0; i < kAnswersSize; ++i) {
      delete[] answers_cycle_[i];
    }
    delete[] answers_cycle_;
    return answer_size_;
  }

  bool FindCoords(int y) {
    if (y == b_) {
      return answer_left_ <= 0;
    }
    if (int(rect_[y].count()) == a_) {
      return FindCoords(y + 1);
    }
    for (int i = 0; i < answer_size_; ++i) {
      if (answer_[i].y > y) {
        break;
      }
      if (answer_[i].x != -1 || answer_[i].y != y) {
        continue;
      }
      auto size = answer_[i].size;
      // Set square
      std::bitset<40> row(std::pow(2, size) - 1);
      int x = 39;
      for (; x >= x_max_ && rect_[y][x]; --x) {
      }
      row <<= x - size + 1;
      for (int j = y; j < y + size; ++j) {
        if ((rect_[j] & row).any()) {
          goto next_square;
        }
      }
      for (int j = y; j < y + size; ++j) {
        rect_[j] |= row;
      }
      // ------
      answer_[i].x = 40 - x;
      --answer_left_;
      if (FindCoords(y)) {
        return true;
      }
      ++answer_left_;
      answer_[i].x = -1;
      // Clear square
      row.flip();
      for (int j = y; j < y + size; ++j) {
        rect_[j] &= row;
      }
    // ----
    next_square:
      continue;
    }
    return false;
  }

 private:
  void InitAnswers() {
    answers_cycle_ = new Square*[kAnswersSize];
    for (int i = 0; i < kAnswersSize; ++i) {
      answers_cycle_[i] = new Square[max_square_count_]{};
    }
  }

  void PushAnswer() {
    if (answers_pos_ >= kAnswersSize) {
      answers_pos_ = 0;
    }
    std::memcpy(answers_cycle_[answers_pos_++], partial_answer_,
                answer_size_ * sizeof(Square));
  }

  Square* PopAnswer() {
    --answers_pos_;
    if (answers_pos_ < 0) {
      answers_pos_ = kAnswersSize - 1;
    }
    return answers_cycle_[answers_pos_];
  }

  void BacktrackImpl(int y, int limit) {
    int flow = levels_[y];
    if (flow == 0) {
      if (y == 0) {
        second_max_square_size_ = partial_answer_[1].size;
      }
      BacktrackImpl(y + 1, second_max_square_size_);
      return;
    }
    if (y == b_) {
      if (square_count_ <= answer_size_) {
        answer_size_ = square_count_;
        PushAnswer();
      }
      return;
    }
    if (square_count_ >= answer_size_ || square_count_ >= max_square_count_) {
      return;
    }
    int size = std::min({flow, b_ - y, limit});
    for (; size >= 1; --size) {
      levels_[y] -= size;
      levels_[y + size] += size;
      partial_answer_[square_count_].y = y;
      partial_answer_[square_count_].size = size;
      ++square_count_;
      BacktrackImpl(y, size);
      --square_count_;
      levels_[y + size] -= size;
      levels_[y] += size;
    }
  }

  static inline int IsPrimeOfTwo(int size) {
    std::pair<int, int> primes[] = {{3, 2}, {7, 3}, {31, 5}};
    for (auto& prime : primes) {
      if (size == prime.first) {
        return prime.second;
      }
    }
    return -1;
  }

  static inline bool IsPrime(int c) {
    int primes[] = {3, 5, 7, 11, 13, 17, 23, 29, 31, 37};
    for (int num : primes) {
      if (c == num) {
        return true;
      }
    }
    return false;
  }

  void PrimeTwoSearch() {
    answer_[0].x = 1;
    answer_[0].y = 0;
    int size = (a_ + 1) / 2;
    answer_[0].size = size;
    answer_[1].size = answer_[2].size = size - 1;
    answer_[1].x = size + 1;
    answer_[2].x = 1;
    answer_[2].y = size;
    answer_size_ = 3;
    int t = a_;
    while (size > 1) {
      size /= 2;
      answer_[answer_size_].size = answer_[answer_size_ + 1].size =
          answer_[answer_size_ + 2].size = size;
      t -= size;
      answer_[answer_size_].x = answer_[answer_size_].y = t + 1;
      answer_[answer_size_ + 1].x = t + 1;
      answer_[answer_size_ + 1].y = t - size;
      answer_[answer_size_ + 2].x = t - size + 1;
      answer_[answer_size_ + 2].y = t;
      answer_size_ += 3;
    }
  }

  bool print_answer_ = true;

  int a_ = 2;
  int b_ = 2;
  int max_square_size_ = INT_MAX;
  int second_max_square_size_ = INT_MAX;

  bool swap_ = false;

  int square_count_ = 0;
  int max_square_count_ = INT_MAX;
  int answer_size_ = INT_MAX;

  int* levels_ = nullptr;
  Square* partial_answer_ = nullptr;
  Square* answer_ = nullptr;

  static constexpr int kAnswersSize = 1000;
  Square** answers_cycle_ = nullptr;
  int answers_pos_ = 0;

  // Variables for finding coords
  std::bitset<40>* rect_{};
  int answer_left_ = 0;
  int x_max_ = 0;
};

int a = 2;
int b = 2;
int max_square_size = INT_MAX;
int second_max_square_size = INT_MAX;

int square_count = 0;
int max_square_count = INT_MAX;
int answer_size = INT_MAX;

int* levels;
Square* partial_answer;
Square* answer;

static int answers_size = 1000;
Square** answers;
int answers_pos = 0;

void InitAnswers() {
  answers = new Square*[answers_size];
  for (int i = 0; i < answers_size; ++i) {
    answers[i] = new Square[max_square_count]{};
  }
}

void PushAnswer() {
  if (answers_pos >= answers_size) {
    answers_pos = 0;
  }
  std::memcpy(answers[answers_pos++], partial_answer,
              answer_size * sizeof(Square));
}

Square* PopAnswer() {
  --answers_pos;
  if (answers_pos < 0) {
    answers_pos = answers_size - 1;
  }
  return answers[answers_pos];
}

void BacktrackImpl(int y, int limit) {
  int f = levels[y];
  if (f == 0) {
    if (y == 0) {
      second_max_square_size = partial_answer[1].size;
    }
    BacktrackImpl(y + 1, second_max_square_size);
    return;
  }
  if (y == b) {
    if (square_count <= answer_size) {
      answer_size = square_count;
      PushAnswer();
    }
    return;
  }
  if (square_count >= answer_size || square_count >= max_square_count) {
    return;
  }
  int size = std::min({f, b - y, limit});
  for (; size >= 1; --size) {
    levels[y] -= size;
    levels[y + size] += size;
    partial_answer[square_count].y = y;
    partial_answer[square_count].size = size;
    ++square_count;
    BacktrackImpl(y, size);
    --square_count;
    levels[y + size] -= size;
    levels[y] += size;
  }
}

std::bitset<40>* rect;
int answer_left = 0;
int x_max = 0;

bool FindCoords(int y) {
  if (y == b) {
    return answer_left <= 0;
  }
  if (int(rect[y].count()) == a) {
    return FindCoords(y + 1);
  }
  for (int i = 0; i < answer_size; ++i) {
    if (answer[i].y > y) {
      break;
    }
    if (answer[i].x != -1 || answer[i].y != y) {
      continue;
    }
    auto size = answer[i].size;
    // Set square
    std::bitset<40> row(std::pow(2, size) - 1);
    int x = 39;
    for (; x >= x_max && rect[y][x]; --x) {
    }
    row <<= x - size + 1;
    for (int j = y; j < y + size; ++j) {
      if ((rect[j] & row).any()) {
        goto next_square;
      }
    }
    for (int j = y; j < y + size; ++j) {
      rect[j] |= row;
    }
    // ------
    answer[i].x = 40 - x;
    --answer_left;
    if (FindCoords(y)) {
      return true;
    }
    ++answer_left;
    answer[i].x = -1;
    // Clear square
    row.flip();
    for (int j = y; j < y + size; ++j) {
      rect[j] &= row;
    }
    // ----
  next_square:
    continue;
  }
  return false;
}

int IsPrimeOfTwo(int size) {
  std::pair<int, int> primes[] = {{3, 2}, {7, 3}, {31, 5}};
  for (auto& prime : primes) {
    if (size == prime.first) {
      return prime.second;
    }
  }
  return -1;
}

bool IsPrime(int c) {
  int primes[] = {3, 5, 7, 11, 13, 17, 23, 29, 31, 37};
  for (int num : primes) {
    if (c == num) {
      return true;
    }
  }
  return false;
}

void PrimeTwoSearch() {
  answer[0].x = 1;
  answer[0].y = 0;
  int size = (a + 1) / 2;
  answer[0].size = size;
  answer[1].size = answer[2].size = size - 1;
  answer[1].x = size + 1;
  answer[2].x = 1;
  answer[2].y = size;
  answer_size = 3;
  int t = a;
  while (size > 1) {
    size /= 2;
    answer[answer_size].size = answer[answer_size + 1].size =
        answer[answer_size + 2].size = size;
    t -= size;
    answer[answer_size].x = answer[answer_size].y = t + 1;
    answer[answer_size + 1].x = t + 1;
    answer[answer_size + 1].y = t - size;
    answer[answer_size + 2].x = t - size + 1;
    answer[answer_size + 2].y = t;
    answer_size += 3;
  }
}

int Backtrack(int n_a, int n_b, bool print_answer = true) {
  a = n_a;
  b = n_b;
  bool swap = false;
  if (a < b) {
    std::swap(a, b);
    swap = true;
  }
  max_square_count = 6 * std::log2(3 * a - 1) - 9;
  InitAnswers();
  answer = new Square[max_square_count]{};

  if (a == b) {
    if (IsPrimeOfTwo(a) != -1) {
      PrimeTwoSearch();
    } else {
      max_square_size =
          IsPrime(a) ? MAX_PRIME_SQUARE_SIZE * a : MAX_SQUARE_SIZE * a;
      goto default_backtrack;
    }
  } else {
    max_square_size = b;
  default_backtrack:
    second_max_square_size = 0.5 * a;
    partial_answer = new Square[max_square_count]{};
    levels = new int[b + 1]{};
    levels[0] = a;
    BacktrackImpl(0, max_square_size);
    delete[] levels;
    delete[] partial_answer;
    // Finding coords
    answer_left = answer_size;
    x_max = 40 - a;
    rect = new std::bitset<40>[b];
    int i = answers_size;
    do {
      answer = PopAnswer();
      --i;
    } while (!FindCoords(0) && i >= 0);
    delete[] rect;
  }

  printf("%d\n", answer_size);
  if (!swap) {
    for (int i = 0; i < answer_size; ++i) {
      printf("%d %d %d\n", answer[i].x, answer[i].y + 1, answer[i].size);
    }
  } else {
    for (int i = 0; i < answer_size; ++i) {
      printf("%d %d %d\n", answer[i].y + 1, answer[i].x, answer[i].size);
    }
  }

  for (int i = 0; i < answers_size; ++i) {
    delete[] answers[i];
  }
  delete[] answers;
  return answer_size;
}

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop
