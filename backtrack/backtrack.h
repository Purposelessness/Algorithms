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
#include <memory>
#include <vector>

#include "debug.h"
#include "square.h"

#define MAX_PRIME_SQUARE_SIZE 0.9
#define MAX_SQUARE_SIZE 0.9

struct Answer {
  int size;
  std::unique_ptr<Square[]> data;
};

class Solution {
 public:
  Solution(int a, int b, bool print_answer = true)
      : printAnswer_(print_answer), a_(a), b_(b) {
    if (a_ < b_) {
      std::swap(a_, b_);
      swap_ = true;
    }
  }

  // Main solution function.
  int backtrack(Answer* answer = nullptr) {
    if (a_ == b_) {
      maxSquareCount_ = 6 * std::log2(3 * a_ - 1) - 9;
    } else {
      maxSquareCount_ = a_ * b_;
    }
    initAnswers();
    answer_ = new Square[maxSquareCount_]{};
    if (b_ <= 3) {
      // If height is less or equal 3.
      trivialCase();
      PRINT("Trivial.\n");
    } else if (a_ == b_) {
      // If square has size=(2^r - 1) it can simply be squared.
      if (isPrimeOfTwo(a_) != -1) {
        PRINT("Square of prime (2^r - 1) size.\n");
        primeTwoSearch();
      } else {
        maxSquareSize_ =
            isPrime(a_) ? MAX_PRIME_SQUARE_SIZE * a_ : MAX_SQUARE_SIZE * a_;
        PRINT("Square.\n")
        goto default_backtrack;
      }
    } else {
      PRINT("Rectangle.\n")
      maxSquareSize_ = MAX_SQUARE_SIZE * b_;
    default_backtrack:
      secondMaxSquareSize_ = std::min(int(0.5 * a_), maxSquareSize_);
      partialAnswer_ = new Square[maxSquareCount_]{};
      levels_ = new int[b_ + 1]{};
      levels_[0] = a_;
      // backtrack algorithm implementation.
      backtrackImpl(0, maxSquareSize_);
      delete[] levels_;
      delete[] partialAnswer_;

      // Finding PSs' coordinates.
      PRINT("\nFinding coordinates.\n\n")
      answerLeft_ = answerSize_;
      xMax_ = 40 - a_;
      // Rectangle's matrix used to find coords.
      rect_ = new std::bitset<40>[b_];
      int i = kAnswersSize;
      do {
        answer_ = popAnswer();
        --i;
        PRINT("Checking answer:\n");
        PRINT_ANSWER(answer_, answerSize_);
      } while (!findCoords(0) && i >= 0);
      delete[] rect_;
    }

    if (printAnswer_) {
      printAnswer();
    }

    if (answer != nullptr) {
      answer->size = answerSize_;
      answer->data = std::unique_ptr<Square[]>(new Square[answerSize_]);
      if (!swap_) {
        std::memcpy(answer->data.get(), answer_, answerSize_ * sizeof(Square));
      } else {
        for (int i = 0; i < answerSize_; ++i) {
          answer->data[i] =
              Square{answer_[i].y + 1, answer_[i].x - 1, answer_[i].size};
        }
      }
    }

    for (int i = 0; i < kAnswersSize; ++i) {
      delete[] answersCycle_[i];
    }
    delete[] answersCycle_;
    return answerSize_;
  }

 private:
  // Trivial case if height <= 3.
  void trivialCase() {
    answerSize_ = 0;
    if (b_ == 2) {
      for (int i = 0; i < a_; ++i) {
        answer_[answerSize_++] = Square{i + 1, 0, 1};
        answer_[answerSize_++] = Square{i + 1, 1, 1};
      }
      return;
    }
    // Case: b_ == 3
    if (a_ % 2 == 0) {
      for (int i = 0; i < a_; i += 2) {
        answer_[answerSize_++] = Square{i + 1, 0, 2};
        answer_[answerSize_++] = Square{i + 1, 2, 1};
        answer_[answerSize_++] = Square{i + 2, 2, 1};
      }
    } else {
      for (int i = 0; i < a_; i += 2) {
        if (i == a_ - 1) {
          answer_[answerSize_++] = Square{i + 1, 0, 1};
          answer_[answerSize_++] = Square{i + 1, 1, 1};
          answer_[answerSize_++] = Square{i + 1, 2, 1};
          break;
        }
        answer_[answerSize_++] = Square{i + 1, 0, 2};
        answer_[answerSize_++] = Square{i + 1, 2, 1};
        answer_[answerSize_++] = Square{i + 2, 2, 1};
      }
    }
  }

  void printAnswer() {
    PRINT("\nAnswer:\n");
    printf("%d\n", answerSize_);
    if (!swap_) {
      for (int i = 0; i < answerSize_; ++i) {
        printf("%d %d %d\n", answer_[i].x, answer_[i].y + 1, answer_[i].size);
      }
    } else {
      for (int i = 0; i < answerSize_; ++i) {
        printf("%d %d %d\n", answer_[i].y + 1, answer_[i].x, answer_[i].size);
      }
    }
    PRINT_RECT(a_, b_, answerSize_, answer_);
  }

  // Trying to place PSs in original rect.
  // Returns true if placement is successful.
  bool findCoords(int y) {
    // If current height equals to rect's height then placement is successful
    // if all squares are placed.
    if (y == b_) {
      PRINT_MATRIX(rect_, a_, b_);
      PRINT("\n");
      return answerLeft_ <= 0;
    }
    // If current level is fully covered then go to next level.
    if (int(rect_[y].count()) == a_) {
      return findCoords(y + 1);
    }
    // Bits in bitset are set from right to left but algorithm
    // places PSs from left to right. Finding first free x position.
    int x = 39;
    for (; x >= xMax_ && rect_[y][x]; --x) {
    }
    PRINT_MATRIX(rect_, a_, b_);
    PRINT("\n");
    // Trying to place each square.
    for (int i = 0; i < answerSize_; ++i) {
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
      --answerLeft_;
      // If all other PSs can be placed then it is the answer.
      PRINT("Placing square: x=" << answer_[i].x << ", y=" << answer_[i].y + 1
                                 << ", size=" << answer_[i].size << '\n');
      if (findCoords(y)) {
        return true;
      }
      PRINT("Answer is wrong: x=" << answer_[i].x << ", y=" << answer_[i].y + 1
                                  << ", size=" << answer_[i].size << ".\n")
      // Else clearing answer and matrix.
      ++answerLeft_;
      answer_[i].x = -1;
      // Clearing matrix.
      row.flip();
      for (int j = y; j < y + size; ++j) {
        rect_[j] &= row;
      }
      PRINT_MATRIX(rect_, a_, b_);
      PRINT("\n");
      // -----------------------
    next_square:
      continue;
    }
    // Cannot place PSs given.
    return false;
  }

  void initAnswers() {
    answersCycle_ = new Square*[kAnswersSize];
    for (int i = 0; i < kAnswersSize; ++i) {
      answersCycle_[i] = new Square[maxSquareCount_]{};
    }
  }

  // Places answer to cycled array.
  void pushAnswer() {
    if (answersPos_ >= kAnswersSize) {
      answersPos_ = 0;
    }
    std::memcpy(answersCycle_[answersPos_++], partialAnswer_,
                answerSize_ * sizeof(Square));
  }

  // Gets last answer from cycled array.
  Square* popAnswer() {
    --answersPos_;
    if (answersPos_ < 0) {
      answersPos_ = kAnswersSize - 1;
    }
    return answersCycle_[answersPos_];
  }

  // Searches for all partitioning options and saves them to 'answersCycle_'.
  // Searching is performed by attempting to construct a Smith diagram by
  // transferring the flow (the width of PS) from the source
  // to the sink along the levels (the height in the original rect).
  // May find options that cannot be the final answer!
  void backtrackImpl(int y, int limit) {
    int flow = levels_[y];
    PRINT(std::string(2 * y, '-') << "Level: " << y << " > flow=");
    PRINT_FLOW(levels_, b_ + 1, y, -1);
    PRINT('\n');
    if (flow == 0) {
      if (y == 0) {
        // If square then all PSs (except first and second)
        // are less than second-biggest partitioning square.
        if (a_ == b_) {
          secondMaxSquareSize_ = partialAnswer_[1].size;
        }
      }
      PRINT(std::string(2 * y, '-')
            << "Level: " << y << " > Moving to the next level\n");
      backtrackImpl(y + 1, secondMaxSquareSize_);
      return;
    }
    // If level equals to height of rect then partial solution is solution.
    if (y == b_) {
      // If current partitioning is less or equal than the answer's one
      // then save it to 'answersCycle_'
      if (squareCount_ <= answerSize_) {
        answerSize_ = squareCount_;
        PRINT(std::string(2 * y, '-')
              << "Level: " << y << " > Saving current partitioning.\n");
        pushAnswer();
      }
      return;
    }
    // If current partitioning is already greater or equal than the answer's
    // one then leave this branch.
    if (squareCount_ >= answerSize_ || squareCount_ >= maxSquareCount_) {
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
      partialAnswer_[squareCount_].y = y;
      partialAnswer_[squareCount_].size = size;
      ++squareCount_;
      backtrackImpl(y, size);
      --squareCount_;
      levels_[y + size] -= size;
      levels_[y] += size;
    }
    //    PRINT(std::string(2 * y, '-')
    //          << "Level: " << y
    //          << " > All extensions are checked (flow=" << levels_[y] <<
    //          ").\n");
  }

  static inline int isPrimeOfTwo(int size) {
    std::pair<int, int> primes[] = {{3, 2}, {7, 3}, {31, 5}};
    for (auto& prime : primes) {
      if (size == prime.first) {
        return prime.second;
      }
    }
    return -1;
  }

  static inline bool isPrime(int c) {
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
  void primeTwoSearch() {
    answer_[0].x = 1;
    answer_[0].y = 0;
    int size = (a_ + 1) / 2;
    answer_[0].size = size;
    answer_[1].size = answer_[2].size = size - 1;
    answer_[1].x = size + 1;
    answer_[2].x = 1;
    answer_[2].y = size;
    answerSize_ = 3;
    int t = a_;
    while (size > 1) {
      size /= 2;
      answer_[answerSize_].size = answer_[answerSize_ + 1].size =
          answer_[answerSize_ + 2].size = size;
      t -= size;
      answer_[answerSize_].x = t + 1;
      answer_[answerSize_].y = t;
      answer_[answerSize_ + 1].x = t + 1;
      answer_[answerSize_ + 1].y = t - size;
      answer_[answerSize_ + 2].x = t - size + 1;
      answer_[answerSize_ + 2].y = t;
      answerSize_ += 3;
    }
  }

  bool printAnswer_ = true;

  int a_ = 2;
  int b_ = 2;
  int maxSquareSize_ = INT_MAX;
  int secondMaxSquareSize_ = INT_MAX;

  bool swap_ = false;

  int squareCount_ = 0;
  int maxSquareCount_ = INT_MAX;
  int answerSize_ = INT_MAX;

  int* levels_ = nullptr;
  Square* partialAnswer_ = nullptr;
  Square* answer_ = nullptr;

  // Cycled array of answers.
  static constexpr int kAnswersSize = 1000;
  Square** answersCycle_ = nullptr;
  int answersPos_ = 0;

  // Variables for finding coords
  std::bitset<40>* rect_{};
  int answerLeft_ = 0;
  int xMax_ = 0;
};

#endif  // BACKTRACK__BACKTRACK_H_

#pragma clang diagnostic pop
