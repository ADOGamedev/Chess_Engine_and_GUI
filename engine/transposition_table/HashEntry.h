#pragma once

#include <stdint.h>
#include "../../chess/move_gen/Move.h"

struct HashEntry {
    HashEntry() :
        zobrist(0),
        depth(-1),
        flag(0),
        eval(0),
        ancient(0),
        move()
    {}

    HashEntry(uint64_t zobrist, int depth, int flag,
              int eval, int ancient, Move move)
        : zobrist(zobrist),
          depth(depth),
          flag(flag),
          eval(eval),
          ancient(ancient),
          move(move)
    {}

    bool is_valid() {
        return depth > 0;
    }

    uint64_t zobrist;
    int depth;
    int flag;
    int eval;
    int ancient;
    Move move;

};