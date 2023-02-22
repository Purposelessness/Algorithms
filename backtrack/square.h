#ifndef BACKTRACK__SQUARE_H_
#define BACKTRACK__SQUARE_H_

struct Square {
  Square() = default;
  Square(int y, int size) : y(y), size(size) {}

  int x = -1;
  int y = 0;
  int size = 0;
};

#endif  // BACKTRACK__SQUARE_H_