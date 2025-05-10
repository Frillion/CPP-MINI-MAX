#include "state.h"
#include <unordered_map>
#include <vector>
#ifndef TRANSPOSITION_HEAD
#define TRANSPOSITION_HEAD

enum evalType{
    UPPERBOUND = 1,
    LOWERBOUND = 2,
    EXACT = 3
};

struct ttEntry{
    int depth;
    int value;
    evalType flag;
    char access;
};

class TranspositionTable{
    TranspositionTable();
    int zorbist_hash(const State& st);

    void LRU_Clock();

    static TranspositionTable instance;
    std::vector<std::vector<int>> hash_keys;
    std::unordered_map<int, ttEntry> ttable;
    int black_move;

public:
    TranspositionTable(const TranspositionTable&) = delete;
    TranspositionTable& operator=(const TranspositionTable&) = delete;
    static TranspositionTable& getInstance(){return instance;}
    void insert(State& st, ttEntry& entry);
    void zorbist_init(int width, int height);
    const ttEntry& operator[](State& st);
};
#endif
