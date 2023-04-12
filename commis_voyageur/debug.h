#ifndef COMMIS_VOYAGEUR__DEBUG_H_
#define COMMIS_VOYAGEUR__DEBUG_H_

#include "graph.h"

//#define DEBUG

void PrintGraph(const Graph& graph);

#ifdef DEBUG
#define print_graph(m) PrintGraph(m)
#else
#define print_graph(m)
#endif

#ifdef DEBUG
#define print(x) std::cout << x
#else
#define print(x)
#endif

#endif  // COMMIS_VOYAGEUR__DEBUG_H_