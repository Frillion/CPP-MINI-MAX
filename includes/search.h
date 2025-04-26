#include "state.h"

#ifndef SEARCH_ALGORITHMS_HEAD
#define SEARCH_ALGORITHMS_HEAD

extern int negamax(State& st, int depth, int alpha, int beta, Player agent_color);
extern std::unique_ptr<move> get_best_move(State& root, int depth, Player agent_color);
extern std::unique_ptr<move> get_best_move_timed(State& root, int max_depth, double time_limit, Player agent_color);

#endif
