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
        throw std::runtime_error("Cannot open file " + filename);
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

TEST_F(BacktrackTest, PrimeSquareTests) {
  auto td = test_data[0];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
                  << "PS Test: width=" << test.w << ", height=" << test.h
                  << ", test_ans=" << test_ans;
  }
}

TEST_F(BacktrackTest, LightTests) {
  for (int i = 1; i <= 4; ++i) {
    auto td = test_data[i];
    for (auto test : td) {
      Solution s(test.w, test.h, false);
      auto test_ans = s.Backtrack();
      ASSERT_EQ(test.cnt, test_ans)
          << "Test №" << i << ": width=" << test.w << ", height=" << test.h
          << ", test_ans=" << test_ans;
    }
  }
}

TEST_F(BacktrackTest, FifthTest) {
  auto td = test_data[5];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
        << "Test №" << 5 << ": width=" << test.w << ", height=" << test.h
        << ", test_ans=" << test_ans;
  }
}

TEST_F(BacktrackTest, SixthTest) {
  auto td = test_data[6];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
                  << "Test №" << 6 << ": width=" << test.w << ", height=" << test.h
                  << ", test_ans=" << test_ans;
  }
}

TEST_F(BacktrackTest, SeventhTest) {
  auto td = test_data[7];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
                  << "Test №" << 7 << ": width=" << test.w << ", height=" << test.h
                  << ", test_ans=" << test_ans;
  }
}


TEST_F(BacktrackTest, EighthTest) {
  auto td = test_data[8];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
                  << "Test №" << 8 << ": width=" << test.w << ", height=" << test.h
                  << ", test_ans=" << test_ans;
  }
}

TEST_F(BacktrackTest, NinthTest) {
  auto td = test_data[9];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
                  << "Test №" << 9 << ": width=" << test.w << ", height=" << test.h
                  << ", test_ans=" << test_ans;
  }
}

TEST_F(BacktrackTest, TenthTest) {
  auto td = test_data[10];
  for (auto test : td) {
    Solution s(test.w, test.h, false);
    auto test_ans = s.Backtrack();
    ASSERT_EQ(test.cnt, test_ans)
        << "Test №" << 10 << ": width=" << test.w << ", height=" << test.h
        << ", test_ans=" << test_ans;
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

#pragma clang diagnostic pop