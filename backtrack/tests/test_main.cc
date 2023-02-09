#include <gtest/gtest.h>

#include "../backtrack.h"

class BacktrackTest : public ::testing::Test {
 public:
  BacktrackTest() = default;

 protected:
  void SetUp() override {
    // ...
  }

  void TearDown() override {
    // ...
  }
};

TEST_F(BacktrackTest, FirstTest) {
  for (int i = 2; i <= 20; ++i) {
    clock_t time = clock();
    Backtrack(i);
    std::cout << "Test time: " << clock() - time << '\n';
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
