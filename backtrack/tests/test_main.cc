#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-readability-casting"
#include <gtest/gtest.h>

#include <fstream>

#include "../backtrack.h"
#include "../debug.h"

class BacktrackTest : public ::testing::Test {
 protected:
  void SetUp() override {
    test_data.resize(11);
    for (int i = 0; i <= 10; ++i) {
      std::string filename = std::to_string(i) + ".txt";
      std::ifstream f(filename);
      if (!f.good()) {
        //        throw std::runtime_error("Cannot open file " + filename);
        return;
      }
      std::string line;
      while (std::getline(f, line), !line.empty()) {
        char* ptr = nullptr;
        int w = strtol(line.data(), &ptr, 10);
        int h = strtol(ptr, &ptr, 10);
        std::getline(f, line);
        int ans = strtol(line.data(), &ptr, 10);
        test_data[i].emplace_back(w, h, ans);
      }
      f.close();
    }
  }

  struct TestData {
    TestData(int w, int h, int cnt) : w(w), h(h), cnt(cnt) {}
    int w, h, cnt;
  };
  std::vector<std::vector<TestData>> test_data;
};

bool TryPlace(int w, int h, const Answer& answer) {
  std::vector<std::vector<bool>> rect(h);
  for (int i = 0; i < h; ++i) {
    rect[i].resize(w);
  }
  for (int i = 0; i < answer.size; ++i) {
    auto square = answer.data[i];
    for (int x = square.x - 1; x < square.x - 1 + square.size; ++x) {
      for (int y = square.y; y < square.y + square.size; ++y) {
        if (rect[y][x]) {
          return false;
        }
        rect[y][x] = true;
      }
    }
  }
  return std::all_of(rect.cbegin(), rect.cend(), [](const auto& v) {
    return std::all_of(v.cbegin(), v.cend(), [](bool f) { return f; });
  });
}

TEST_F(BacktrackTest, PrimeSquareTests) {
  auto td = test_data[0];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    Answer ans{};
    auto test_ans = s.backtrack(&ans);
    ASSERT_EQ(test.cnt, test_ans)
        << "PS Test: width=" << test.w << ", height=" << test.h
        << ", test_ans=" << test_ans;
    ASSERT_EQ(TryPlace(test.w, test.h, ans), true);
  }
}

TEST_F(BacktrackTest, EvenSquareTests) {
  for (int i = 2; i <= 40; i += 2) {
    Solution s(i, i, false);
    Answer ans{};
    auto test_ans = s.backtrack(&ans);
    ASSERT_EQ(4, test_ans) << "PS Test: width=" << i << ", height=" << i
                           << ", test_ans=" << test_ans;
    ASSERT_EQ(TryPlace(i, i, ans), true);
  }
}

TEST_F(BacktrackTest, PlacementTests) {
  for (int i = 2; i <= 10; ++i) {
    for (int j = i; j <= 2 * i; ++j) {
      Solution s(i, j, false);
      Answer ans{};
      s.backtrack(&ans);
      EXPECT_EQ(TryPlace(i, j, ans), true)
          << "Test: width=" << i << ", height=" << j;
    }
  }
}

TEST_F(BacktrackTest, PlacementTests2) {
  for (int i = 11; i <= 20; ++i) {
    for (int j = i; j <= 30; ++j) {
      Solution s(i, j, false);
      Answer ans{};
      s.backtrack(&ans);
      EXPECT_EQ(TryPlace(i, j, ans), true)
          << "Test: width=" << i << ", height=" << j;
    }
  }
}

TEST_F(BacktrackTest, PlacementTests3) {
  for (int i = 21; i <= 30; ++i) {
    for (int j = i; j <= 40; ++j) {
      Solution s(i, j, false);
      Answer ans{};
      s.backtrack(&ans);
      EXPECT_EQ(TryPlace(i, j, ans), true)
          << "Test: width=" << i << ", height=" << j;
    }
  }
}

TEST_F(BacktrackTest, PlacementTests4) {
  for (int i = 31; i <= 40; ++i) {
    for (int j = i; j <= 40; ++j) {
      Solution s(i, j, false);
      Answer ans{};
      s.backtrack(&ans);
      EXPECT_EQ(TryPlace(i, j, ans), true)
          << "Test: width=" << i << ", height=" << j;
    }
  }
}

// TEST_F(BacktrackTest, RectTests) {
//   for (int i = 1; i <= 10; ++i) {
//     auto td = test_data[i];
//     for (auto test : td) {
//       Solution s(test.w, test.h, false);
//       auto test_ans = s.backtrack();
//       ASSERT_EQ(test.cnt, test_ans)
//           << "Test №" << i << ": width=" << test.w << ", height=" << test.h
//           << ", test_ans=" << test_ans;
//     }
//   }
// }

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop