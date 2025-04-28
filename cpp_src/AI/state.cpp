#include "state.h"
#include <iostream>
#include <cstdlib>
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
}

State::State(State& other){
    if(this == &other)
        return;

    this->width = other.width;
    this->height = other.height;
    this->board = other.board;
}

bool State::is_valid_move(const move& mv){
    if(mv.from.x < 1 || mv.from.x > this->width) // Boundary checking x for current pos
       return false;

    if(mv.from.y < 1 || mv.from.y > this->height)// Boundary checking y for current pos
        return false;

    if(mv.to.x < 1 || mv.to.x > this->width)// Boundary checking x for next pos
        return false;

    if(mv.to.y < 1 || mv.to.y > this->height)// Boundary checking y for next pos
        return false;

    if(this->board.find(mv.from) == this->board.end())// Piece you're trying to move doesn't exist
        return false;

    if(this->board[mv.from] != this->turn)
        return false;

    position delta = mv.delta();
    if(this->board.find(mv.to) != this->board.end()){// Square we are moving to is occupied
        if(this->board[mv.to] == this->turn)// Can never move over own piece
            return false;

        if(std::abs(delta.x) != 1 && std::abs(delta.y) != 1)// Capture Like Pawn Rule
            return false;

        return true;
    }
    
    if(!((std::abs(delta.x) == 1 && std::abs(delta.y) == 2) ||
        (std::abs(delta.x) == 2 && std::abs(delta.y) == 1)))
        return false;

    return true;
}

std::unique_ptr<State> State::apply_move(const move& mv){
    if(!this->is_valid_move(mv))
        return nullptr;

    std::unique_ptr<State> successor = std::make_unique<State>(*this);
    successor->board[mv.to] = successor->board[mv.from];
    successor->board.erase(mv.from);
    successor->turn = this->turn == WHITE ? BLACK : WHITE;
    return std::move(successor);
}

std::vector<move> State::get_legal_moves(){
    std::vector<move> moves;

    for(const auto& square: this->board){
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
