#include <gtest/gtest.h>

class MainTest : public ::testing::Test {
 public:
  MainTest() = default;

 protected:
  void SetUp() override {
    // ...
  }

  void TearDown() override {
    // ...
  }
};

TEST_F(MainTest, FirstTest) {
  // ...
}

TEST(SingleTest, FirstTest) {
  // ...
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

