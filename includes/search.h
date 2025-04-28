#include "state.h"
#include <chrono>

#ifndef SEARCH_ALGORITHMS_HEAD
#define SEARCH_ALGORITHMS_HEAD

using std::chrono::steady_clock;

extern int negamax(State& st, int depth, int alpha, int beta, Player agent_color);
extern std::unique_ptr<move> get_best_move(State& root, int depth, Player agent_color, steady_clock::time_point start_time, double time_limit);
extern std::unique_ptr<move> get_best_move_timed(State& root, int max_depth, double time_limit, Player agent_color);

#endif
