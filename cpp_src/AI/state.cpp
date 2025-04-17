#include "state.h"
#include <vector>

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

bool State::is_valid_move(position &curr_pos, position &next_pos){
    if(!((1 <= curr_pos.x && curr_pos.x <= this->width)&&
         (1 <= curr_pos.y && curr_pos.y <= this->height))) // curr_pos out of bounds
        return false;

    if(!((1 <= next_pos.x && next_pos.x <= this->width)&&
         (1 <= next_pos.y && next_pos.y <= this->height))) // next_pos out of bounds
        return false;

    if(this->board[curr_pos] == "")// No piece at curr_pos
        return false;

    std::string curr_piece = this->board[curr_pos];
    if((curr_piece == "W" && this->turn != "white") || 
        (curr_piece == "B" && this->turn != "black")) // picking wrong piece
        return false;

    int dx = std::abs(next_pos.x - curr_pos.x);
    int dy = std::abs(next_pos.y - curr_pos.y);

    // Movement restriction, can only move like a knight
    // either one square forward and two squares to the left or right
    // or one square to the left or right and two squares forward
    if(!((dx == 2 && dy == 1) || (dx == 1 && dy == 2)))
        return false;

    if((curr_piece == "W" && next_pos.y <= curr_pos.y) || 
        (curr_piece == "B" && next_pos.y >= curr_pos.y)) // Pieces cannot move backward
       return false;

    if(this->board[next_pos] != ""){
        if(dx != 1 || dy != 1) // Can only capture like a pawn
            return false;

        if(this->board[next_pos] == curr_piece) // Cannot capture your own color
            return false;
    }
    else if(dx == 1 && dy == 1) // Cannot move diagonal if not capturing
        return false;

    return true; // All other moves are valid
}

bool State::apply_move(position &curr_pos, position &next_pos){
    if(!this->is_valid_move(curr_pos, next_pos))
        return false;

    this->board[next_pos] = this->board[curr_pos];
    this->board[curr_pos] = "";
    this->turn = this->turn == "white" ? "black":"white";
    return true;
}

std::vector<position> State::get_legal_moves(){
}
