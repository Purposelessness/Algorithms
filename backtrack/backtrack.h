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
#include "square.h"

#define MAX_PRIME_SQUARE_SIZE 0.9
#define MAX_SQUARE_SIZE 0.9

class Solution {
 public:
  Solution(int a, int b, bool print_answer = true)
      : print_answer_(print_answer), a_(a), b_(b) {
    if (a_ < b_) {
      std::swap(a_, b_);
      swap_ = true;
    }
  }

  // Main solution function.
  int Backtrack() {
    max_square_count_ = 6 * std::log2(3 * a_ - 1) - 9;
    InitAnswers();
    answer_ = new Square[max_square_count_]{};
    if (a_ == b_) {
      // If square has size=(2^r - 1) it can simply be squared.
      if (IsPrimeOfTwo(a_) != -1) {
        PRINT("Square of prime (2^r - 1) size.\n");
        PrimeTwoSearch();
      } else {
        max_square_size_ =
            IsPrime(a_) ? MAX_PRIME_SQUARE_SIZE * a_ : MAX_SQUARE_SIZE * a_;
        PRINT("Square.\n")
        goto default_backtrack;
      }
    } else {
      max_square_size_ = b_;
    default_backtrack:
      second_max_square_size_ = 0.5 * a_;
      partial_answer_ = new Square[max_square_count_]{};
      levels_ = new int[b_ + 1]{};
      levels_[0] = a_;
      // Backtrack algorithm implementation.
      BacktrackImpl(0, max_square_size_);
      delete[] levels_;
      delete[] partial_answer_;

      // Finding PSs' coordinates.
      PRINT("\nFinding coordinates.\n\n")
      answer_left_ = answer_size_;
      x_max_ = 40 - a_;
      // Rectangle's matrix used to find coords.
      rect_ = new std::bitset<40>[b_];
      int i = kAnswersSize;
      do {
        answer_ = PopAnswer();
        --i;
        PRINT("Checking answer:\n");
        PRINT_ANSWER(answer_, answer_size_);
      } while (!FindCoords(0) && i >= 0);
      delete[] rect_;
    }

    if (print_answer_) {
      PrintAnswer();
    }

    for (int i = 0; i < kAnswersSize; ++i) {
      delete[] answers_cycle_[i];
    }
    delete[] answers_cycle_;
    return answer_size_;
  }

 private:
  void PrintAnswer() {
    PRINT("\nAnswer:\n");
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
    PRINT_RECT(a_, b_, answer_size_, answer_);
  }

  // Trying to place PSs in original rect.
  // Returns true if placement is successful.
  bool FindCoords(int y) {
    // If current height equals to rect's height then placement is successful
    // if all squares are placed.
    if (y == b_) {
      return answer_left_ <= 0;
    }
    // If current level is fully covered then go to next level.
    if (int(rect_[y].count()) == a_) {
      return FindCoords(y + 1);
    }
    PRINT_MATRIX(rect_, a_, b_);
    PRINT("\n");
    // Bits in bitset are set from right to left but algorithm
    // places PSs from left to right. Finding first free x position.
    int x = 39;
    for (; x >= x_max_ && rect_[y][x]; --x) {
    }
    // Trying to place each square.
    for (int i = 0; i < answer_size_; ++i) {
      // If PS's y is greater than current height then there is no correct
      // placement (in the 'answer_' the heights of all PSs increase).
      if (answer_[i].y > y) {
        break;
      }
      // If PS's x is not '-1' then it is already placed.
      // If Ps's y is not equal current height then it cannot be placed here.
      if (answer_[i].x != -1 || answer_[i].y != y) {
        continue;
      }
      auto size = answer_[i].size;
      // Trying to place selected PS.
      std::bitset<40> row(std::pow(2, size) - 1);
      row <<= x - size + 1;
      // Check if PS intersects with any other square.
      for (int j = y; j < y + size; ++j) {
        if ((rect_[j] & row).any()) {
          // If intersects then trying to place another PS.
          goto next_square;
        }
      }
      // If not then place here.
      for (int j = y; j < y + size; ++j) {
        rect_[j] |= row;
      }
      // -----------------------
      // Saving coordinate of possible answer.
      answer_[i].x = 40 - x;
      --answer_left_;
      // If all other PSs can be placed then it is the answer.
      if (FindCoords(y)) {
        return true;
      }
      // Else clearing answer and matrix.
      ++answer_left_;
      answer_[i].x = -1;
      // Clearing matrix.
      row.flip();
      for (int j = y; j < y + size; ++j) {
        rect_[j] &= row;
      }
      // -----------------------
    next_square:
      continue;
    }
    // Cannot place PSs given.
    return false;
  }

  void InitAnswers() {
    answers_cycle_ = new Square*[kAnswersSize];
    for (int i = 0; i < kAnswersSize; ++i) {
      answers_cycle_[i] = new Square[max_square_count_]{};
    }
  }

  // Places answer to cycled array.
  void PushAnswer() {
    if (answers_pos_ >= kAnswersSize) {
      answers_pos_ = 0;
    }
    std::memcpy(answers_cycle_[answers_pos_++], partial_answer_,
                answer_size_ * sizeof(Square));
  }

  // Gets last answer from cycled array.
  Square* PopAnswer() {
    --answers_pos_;
    if (answers_pos_ < 0) {
      answers_pos_ = kAnswersSize - 1;
    }
    return answers_cycle_[answers_pos_];
  }

  // Searches for all partitioning options and saves them to 'answers_cycle_'.
  // Searching is performed by attempting to construct a Smith diagram by
  // transferring the flow (the width of PS) from the source
  // to the sink along the levels (the height in the original rect).
  // May find options that cannot be the final answer!
  void BacktrackImpl(int y, int limit) {
    int flow = levels_[y];
    PRINT(std::string(2 * y, '-') << "Level: " << y << " > flow=");
    PRINT_FLOW(levels_, b_ + 1, y, -1);
    PRINT('\n');
    if (flow == 0) {
      if (y == 0) {
        // If square then all PSs (except first and second)
        // are less than second-biggest partitioning square.
        if (a_ == b_) {
          second_max_square_size_ = partial_answer_[1].size;
        }
      }
      PRINT(std::string(2 * y, '-')
            << "Level: " << y << " > Moving to the next level\n");
      BacktrackImpl(y + 1, second_max_square_size_);
      return;
    }
    // If level equals to height of rect then partial solution is solution.
    if (y == b_) {
      // If current partitioning is less or equal than the answer's one
      // then save it to 'answers_cycle_'
      if (square_count_ <= answer_size_) {
        answer_size_ = square_count_;
        PRINT(std::string(2 * y, '-')
              << "Level: " << y << " > Saving current partitioning.\n");
        PushAnswer();
      }
      return;
    }
    // If current partitioning is already greater or equal than the answer's
    // one then leave this branch.
    if (square_count_ >= answer_size_ || square_count_ >= max_square_count_) {
      PRINT(std::string(2 * y, '-') << "Level: " << y << " > Not an answer.\n");
      return;
    }
    // Check all available extensions at the current level starting with
    // the biggest one.
    int size = std::min({flow, b_ - y, limit});
    for (; size >= 1; --size) {
      // The selected PS reduces flow at the current level and increases it at
      // the level corresponding to the lower face of the selected PS.
      levels_[y] -= size;
      levels_[y + size] += size;
      PRINT(std::string(2 * y, '-')
            << "Level: " << y << " > PS=" << size << ", flow=");
      PRINT_FLOW(levels_, b_ + 1, y, y + size);
      PRINT('\n');
      partial_answer_[square_count_].y = y;
      partial_answer_[square_count_].size = size;
      ++square_count_;
      BacktrackImpl(y, size);
      --square_count_;
      levels_[y + size] -= size;
      levels_[y] += size;
    }
    //    PRINT(std::string(2 * y, '-')
    //          << "Level: " << y
    //          << " > All extensions are checked (flow=" << levels_[y] <<
    //          ").\n");
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

  // If original square's size=(2^r - 1) then all PSs can be easily found
  // using this algorithm.
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
      answer_[answer_size_].x = t + 1;
      answer_[answer_size_].y = t;
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

  // Cycled array of answers.
  static constexpr int kAnswersSize = 1000;
  Square** answers_cycle_ = nullptr;
  int answers_pos_ = 0;

  // Variables for finding coords
  std::bitset<40>* rect_{};
  int answer_left_ = 0;
  int x_max_ = 0;
};

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop
