#include "transposition.h"
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
    for(int i = 0; i < st.get_width()*st.get_height(); i++){
    }

    return zorbist;
}

#endif
