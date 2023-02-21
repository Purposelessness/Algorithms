#pragma clang diagnostic push
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

#define MAX_PRIME_SQUARE_SIZE 0.9
#define MAX_SQUARE_SIZE 0.9

struct Square {
  Square() = default;
  Square(int y, int size) : y(y), size(size) {}

  int x = -1;
  int y = 0;
  int size = 0;
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
    answers_pos = answer_size - 1;
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
  if (rect[y].count() == a) {
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

struct Pair {
  int num = 1;
  int degree = 0;
};

int IsPrimeOfTwo(int size) {
  Pair primes[] = {{3, 2}, {7, 3}, {31, 5}};
  for (auto& prime : primes) {
    if (size == prime.num) {
      return prime.degree;
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

void Backtrack() {
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
}

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop
