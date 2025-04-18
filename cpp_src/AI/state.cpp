#include "state.h"
#include <vector>

const position State::piece_moves[] = {{2, 1}, {1, 2}, {-2, 1}, {-1, 2},{1, 1}, {-1, 1}};

State::State(int width,int height){
    this->width = width;
    this->height = height;
    this->turn = "white";
    for(int i = 1; i < width + 1; i++){
        board[{i,1}] = "W";
        board[{i,2}] = "W";
        board[{i,this->height}] = "B";
        board[{i,this->height - 1}] = "B";
    }
    for(int x = 1; x < width; x++){
        for(int y = 3; y < this->height - 1; y++){
            board[{x,y}] = "";
        }
    }
}

bool State::is_valid_move(const move& mv){
    if(!((1 <= mv.from.x && mv.from.x <= this->width)&& (1 <= mv.from.y && mv.from.y <= this->height))) // mv.from out of bounds
        return false;

    if(!((1 <= mv.to.x && mv.to.x <= this->width)&&
         (1 <= mv.to.y && mv.to.y <= this->height))) // mv.to out of bounds
        return false;

    if(this->board[mv.from] == "")// No piece at mv.from
        return false;

    std::string curr_piece = this->board[mv.from];
    if((curr_piece == "W" && this->turn != "white") || 
        (curr_piece == "B" && this->turn != "black")) // picking wrong piece
        return false;

    // Movement restriction, can only move like a knight
    // either one square forward and two squares to the left or right
    // or one square to the left or right and two squares forward
    if(!((mv.delta().x == 2 && mv.delta().y == 1) || (mv.delta().x == 1 && mv.delta().y == 2)))
        return false;

    if((curr_piece == "W" && mv.to.y <= mv.from.y) || 
        (curr_piece == "B" && mv.to.y >= mv.from.y)) // Pieces cannot move backward
       return false;

    if(this->board[mv.to] != ""){
        if(mv.delta().x != 1 || mv.delta().y != 1) // Can only capture like a pawn
            return false;

        if(this->board[mv.to] == curr_piece) // Cannot capture your own color
            return false;
    }
    else if(mv.delta().x == 1 && mv.delta().y == 1) // Cannot move diagonal if not capturing
        return false;

    return true; // All other moves are valid
}

bool State::apply_move(const move& mv){
    if(!this->is_valid_move(mv))
        return false;

    this->board[mv.to] = this->board[mv.from];
    this->board[mv.from] = "";
    this->turn = this->turn == "white" ? "black":"white";
    return true;
}

std::vector<move> State::get_legal_moves(){
    std::vector<move> moves;

    for(const auto& square: this->board){
        if(square.second == "")
            continue;
        if(this->turn == "black" && square.second == "W")
            continue;
        if(this->turn == "white" && square.second == "B")
            continue;

        // No need to seperate normal moves from capture moves since
        // is_valid_move checks for this
        for(auto delta: this->piece_moves){
            if(square.second == "B")
                delta.y = (-delta.y);
            
            move move_to_check = {square.first, {square.first.x + delta.x, square.first.y + delta.y}};

            if(this->is_valid_move(move_to_check)){
                moves.push_back(move_to_check);
            }
        }
    }

    return moves;
}
