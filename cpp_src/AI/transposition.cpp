#include "transposition.h"
#include <cmath>
#include <random>
#ifndef TRANSPOSITION_CPP
#define TRANSPOSITION_CPP

void TranspositionTable::zorbist_init(int width,int height){
    std::random_device rd;
    std::mt19937 gen(rd());

    this->hash_keys.resize(width*height);
    for(int i = 0; i < width*height; i++){
        this->hash_keys[i].push_back(gen());
        this->hash_keys[i].push_back(gen());
    }
    this->black_move = gen();
}

int TranspositionTable::zorbist_hash(const State& st){
    int zorbist = 0;
    if(st.get_turn() == BLACK)
        zorbist ^= this->black_move;

    const std::unordered_map<position,Player> board = st.get_board();
    for(const auto entry: board){
        int square_index = std::sqrt(std::pow(entry.first.x - 1, 2) + std::pow(entry.first.y - 1, 2));
        if(entry.second == WHITE){
            zorbist ^= this->hash_keys[square_index][1];
        }
        else if(entry.second == BLACK){
            zorbist ^= this->hash_keys[square_index][2];
        }
    }

    return zorbist;
}

TranspositionTable::TranspositionTable(){}

void TranspositionTable::LRU_Clock(){

}

#endif
