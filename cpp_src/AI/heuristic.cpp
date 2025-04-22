#include "state.h"
#include "heuristic.h"
#include <algorithm>

#ifndef STATE_EVALUATION
#define STATE_EVALUATION

int evaluate_state(State& st, Player agent_color, Outcome outcome){
    if(outcome == WHITE_W)
        return agent_color == BLACK ? -100 : 100;

    if(outcome == BLACK_W)
        return agent_color == WHITE ? -100 : 100;

    if(outcome == DRAW)
        return 0;

    int white_max = 0;
    int black_min = st.get_height();
    int piece_advantage = 0;

    for(const auto& square : st.get_board()){
        if(square.second == WHITE){
            white_max = std::max(white_max, square.first.y);
            if(agent_color == WHITE)
                piece_advantage += 1;
        }
        else if(square.second == BLACK){
            black_min = std::min(black_min, square.first.y);
            if(agent_color == BLACK)
                piece_advantage += 1;
        }
    }

    float heuristic_value = ((black_min - 1) - (st.get_height() - white_max)) + (piece_advantage * 10);
    return heuristic_value * agent_color; 
}

#endif
