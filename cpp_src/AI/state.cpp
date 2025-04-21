#include "state.h"
#include <memory>
#include <utility>
#include <vector>

#ifndef STATE_REPR_IMPL
#define STATE_REPR_IMPL

const position State::piece_moves[] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2},{1, 1}, {-1, 1}};

State::State(int width,int height){
    this->width = width;
    this->height = height;
    this->turn = WHITE;

    for(int i = 1; i < width + 1; i++){
        board[{i,1}] = WHITE;
        board[{i,2}] = WHITE;
        board[{i,this->height}] = BLACK;
        board[{i,this->height - 1}] = BLACK;
    }

    for(int x = 1; x < width; x++){
        for(int y = 3; y < this->height - 1; y++){
            board[{x,y}] = NONE;
        }
    }
}

State::State(State& other){
    if(this == &other)
        return;

    this->width = other.width;
    this->height = other.height;
    this->board = other.board;
}

bool State::is_valid_move(const move& mv){
    if(!((1 <= mv.from.x && mv.from.x <= this->width)&& (1 <= mv.from.y && mv.from.y <= this->height))) // mv.from out of bounds
        return false;

    if(!((1 <= mv.to.x && mv.to.x <= this->width)&&
         (1 <= mv.to.y && mv.to.y <= this->height))) // mv.to out of bounds
        return false;

    if(this->board[mv.from] == NONE)// No piece at mv.from
        return false;

    Player curr_piece = this->board[mv.from];
    if((curr_piece == WHITE && this->turn != WHITE) || 
        (curr_piece == BLACK && this->turn != BLACK)) // picking wrong piece
        return false;

    // Movement restriction, can only move like a knight
    // either one square forward and two squares to the left or right
    // or one square to the left or right and two squares forward
    if(!((mv.delta().x == 2 && mv.delta().y == 1) || (mv.delta().x == 1 && mv.delta().y == 2)))
        return false;

    if((curr_piece == WHITE && mv.to.y <= mv.from.y) || 
        (curr_piece == BLACK && mv.to.y >= mv.from.y)) // Pieces cannot move backward
       return false;

    if(this->board[mv.to] != NONE){
        if(mv.delta().x != 1 || mv.delta().y != 1) // Can only capture like a pawn
            return false;

        if(this->board[mv.to] == curr_piece) // Cannot capture your own color
            return false;
    }
    else if(mv.delta().x == 1 && mv.delta().y == 1) // Cannot move diagonal if not capturing
        return false;

    return true; // All other moves are valid
}

std::unique_ptr<State> State::apply_move(const move& mv){
    if(!this->is_valid_move(mv))
        return nullptr;

    std::unique_ptr<State> successor = std::make_unique<State>(*this);
    successor->board[mv.to] = successor->board[mv.from];
    successor->board[mv.from] = NONE;
    successor->turn = this->turn == WHITE ? BLACK : WHITE;
    return std::move(successor);
}

std::vector<move> State::get_legal_moves(){
    std::vector<move> moves;

    for(const auto& square: this->board){
        if(square.second == NONE)
            continue;
        if(this->turn == BLACK && square.second == WHITE)
            continue;
        if(this->turn == WHITE && square.second == BLACK)
            continue;

        // No need to seperate normal moves from capture moves since
        // is_valid_move checks for this
        for(auto delta: this->piece_moves){
            if(square.second == BLACK)
                delta.y = (-delta.y);
            
            move move_to_check = {square.first, {square.first.x + delta.x, square.first.y + delta.y}};

            if(this->is_valid_move(move_to_check)){
                moves.push_back(move_to_check);
            }
        }
    }

    return moves;
}

Outcome State::check_win(){
    // If either black reaches end or white reaches end, this is then a Terminal state
    for(const auto& square : this->board){
        if(square.first.y == this->height && square.second == WHITE)
            return WHITE_W;

        if(square.first.y == 1 && square.second == BLACK)
            return BLACK_W;
    }

    // If neither player has legal moves to play it is a Terminal state
    std::vector<move> legal_moves = this->get_legal_moves();

    if(legal_moves.empty()){
        this->turn = this->turn == WHITE ? BLACK : WHITE;// Temporarily change to opponentes turn
        std::vector<move> opponent_legal_moves = this->get_legal_moves();

        if(opponent_legal_moves.empty()){
            this->turn = this->turn == WHITE ? BLACK : WHITE;// Clean up turn switch
            return DRAW;
        }
        this->turn = this->turn == WHITE ? BLACK : WHITE;// Clean up turn switch
    }

    return NONE_W;
}

#endif
