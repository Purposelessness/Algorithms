#ifndef A_STAR__DEBUG_H_
#define A_STAR__DEBUG_H_

#include <vector>

#include "config.h"
#include "graph.h"

#ifdef DEBUG
#define PRINT(x) std::cout << x;
#else
#define PRINT(x)
#endif

#endif  // A_STAR__DEBUG_H_