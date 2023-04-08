#ifndef KNUTH_MORRIS_PRATT__DEBUG_H_
#define KNUTH_MORRIS_PRATT__DEBUG_H_

#include <iostream>
#include <string>
#include <vector>

#define DEBUG

#ifdef DEBUG
#define print(x) std::cout << x
#else
#define print(x)
#endif

#ifdef DEBUG
#define debug(x) x
#else
#define debug(x)
#endif

#ifdef DEBUG
#define display_pi(word, pi, i, j, success) DisplayPi(word, pi, i, j, success)
#define display_pi_transition(word, pi, i, j, j_next, pi_j) \
  DisplayPiTransition(word, pi, i, j, j_next, pi_j)
#else
#define display_pi(word, pi, i, j, success)
#define display_pi_transition(word, pi, i, j, j_next, pi_j)
#endif

#ifdef DEBUG
#define display_search(text, word, pi, i, j, success) \
  DisplaySearch(text, word, pi, i, j, success)
#define display_search_transition(text, word, pi, i, j, j_next, pi_j) \
  DisplaySearchTransition(text, word, pi, i, j, j_next, pi_j)
#define display_search_found(text, word, pi, len, i, j_next, pi_j) \
  DisplaySearchFound(text, word, pi, len, i, j_next, pi_j)
#else
#define display_search(text, word, pi, i, j, success)
#define display_search_transition(text, word, pi, i, j, j_next, pi_j)
#define display_search_found(text, word, pi, len, i, j_next, pi_j)
#endif

void DisplayPi(const std::string& word, const std::vector<int>& pi, int i,
               int j, bool success);

void DisplayPiTransition(const std::string& word, const std::vector<int>& pi,
                         int i, int j, int j_next, int pi_j);

void DisplaySearch(const std::string& text, const std::string& word,
                   const std::vector<int>& pi, int i, int j, bool success);

void DisplaySearchTransition(const std::string& text, const std::string& word,
                             const std::vector<int>& pi, int i, int j,
                             int j_next, int pi_j);

void DisplaySearchFound(const std::string& text, const std::string& word,
                        const std::vector<int>& pi, int len, int i, int j_next,
                        int pi_j);

#endif  // KNUTH_MORRIS_PRATT__DEBUG_H_