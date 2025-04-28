#include "state.h"
#include "heuristic.h"
#include "search.h"
#include <iostream>
#include <memory>
#include <vector>
#include <limits>
#include <chrono>

#ifndef SEARCH_ALGORITHMS
#define SEARCH_ALGORITHMS


int negamax(State& st, int depth, int alpha, int beta, Player agent_color){
    Outcome state_outcome = st.check_win();
    if(depth == 0 || state_outcome != NONE_W)
        return evaluate_state(st, agent_color, state_outcome);

    std::vector<move> all_legal_moves = st.get_legal_moves();

    int value = std::numeric_limits<int>::min();
    for(const move& mv: all_legal_moves){
        std::unique_ptr<State> successor = st.apply_move(mv);
        value = std::max(value,-negamax(*successor, depth - 1, -alpha, -beta, agent_color));
        alpha = std::max(alpha, value);
        if(alpha >= beta)
            break;
    }

    return value;
}

void output_board(State& st){
    for(int y = st.get_height(); y > 0; y--){
        for(int x = 1; x <= st.get_width(); x++){
            position pos = {x, y};
            std::unordered_map<position, Player> board = st.get_board();
            auto piece = board.find(pos);
            if(piece != board.end()){
                Player color = piece->second;
                if(color == WHITE)
                    std::cout << "| W |";
                if(color == BLACK)
                    std::cout << "| B |";
            }
            else{
                std::cout << "|   |";
            }
        }
        std::cout << std::endl;
    }
}

std::unique_ptr<move> get_best_move(State& root, int depth, Player agent_color){
    std::unique_ptr<move> best_move = nullptr;
    int best_value = std::numeric_limits<int>::min();

    output_board(root);
    for(const auto& mv : root.get_legal_moves()){
        //std::cout << "Checking Move: ( move " <<
        //    mv.from.x << " " << mv.from.y << " " <<
        //    mv.to.x << " " << mv.to.y << " )" << std::endl;
        std::unique_ptr<State> successor = root.apply_move(mv);
        //output_board(*successor);
        int eval = -negamax(*successor, depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), agent_color);
        if(eval > best_value)
            best_move = std::make_unique<move>(mv);

        best_value = std::max(best_value, eval);
    }
    return std::move(best_move);
}


using std::chrono::steady_clock;

std::unique_ptr<move> get_best_move_timed(State& root, int max_depth, double time_limit, Player agent_color){
    std::unique_ptr<move> best_move = nullptr;
    int depth = 1;
    std::chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    while(depth <= max_depth){
        std::cout << "SEARCHING TO DEPTH: " << depth << std::endl;
        steady_clock::time_point end_time = std::chrono::steady_clock::now();
        steady_clock::duration time_span = end_time - start_time;
        double seconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den; // Honestly wish there where a better way maybe there is
        std::cout << "ELAPSED TIME: " << seconds << std::endl;
        std::cout << "TIME LIMIT: " << time_limit << std::endl;
        if(seconds > time_limit)
            break;

        std::unique_ptr<move> mv = get_best_move(root, depth, agent_color);
        if(mv)
            best_move = std::move(mv);

        depth++;
    }

    return best_move;
}

#endif
