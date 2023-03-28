#include <gtest/gtest.h>

#include "../solution.h"

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

TEST_F(MainTest, ReductionTest) {
  int d = 0;
  Graph graph = {.data{{infty, 25, 40, 31, 27},
                       {5, infty, 17, 30, 25},
                       {19, 15, infty, 6, 1},
                       {9, 50, 24, infty, 6},
                       {22, 8, 7, 10, infty}}};
  Graph reduced_graph = {.data{{infty, 0, 15, 3, 2},
                               {0, infty, 12, 22, 20},
                               {18, 14, infty, 2, 0},
                               {3, 44, 18, infty, 0},
                               {15, 1, 0, 0, infty}}};
  auto null_vec = Solution::Reduction(graph, d);
  PointSet correct_null_vec = {{0, 1}, {1, 0}, {2, 4}, {3, 4}, {4, 2}, {4, 3}};
  EXPECT_EQ(d, 47);
  EXPECT_EQ(graph == reduced_graph, true);
  EXPECT_EQ(null_vec == correct_null_vec, true);
}

TEST(SingleTest, BranchingTest) {
  Graph graph = {.data{{infty, 0, 15, 3, 2},
                       {0, infty, 12, 22, 20},
                       {18, 14, infty, 2, 0},
                       {3, 44, 18, infty, 0},
                       {15, 1, 0, 0, infty}}};
  PointSet null_vec = {{0, 1}, {1, 0}, {2, 4}, {3, 4}, {4, 2}, {4, 3}};
  auto branching = Solution::Branching(graph, null_vec);
  std::vector<Point> correct_branching = {{1, 0}, {1, 2}, {3, 0}};
  EXPECT_EQ(branching == correct_branching, true);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
