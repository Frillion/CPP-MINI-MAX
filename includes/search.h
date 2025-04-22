#include "state.h"

#ifndef SEARCH_ALGORITHMS_HEAD
#define SEARCH_ALGORITHMS_HEAD

extern int negamax(State& st, int depth, int alpha, int beta);
extern std::unique_ptr<move> get_best_move(State& root, int depth);
extern std::unique_ptr<move> get_best_move_timed(State& root, int max_depth, double time_limit);

#endif
