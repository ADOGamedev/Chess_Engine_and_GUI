#pragma once

#include "../../chess/move_gen/Move.h"
#include "../constants/constants.h"

namespace KillerMoves {
    

inline std::array<Move, KILLERS_PER_PLY> moves[PLY_COUNT];

inline void insert_killer(const Move& killer, const int& ply) {
    if (killer == moves[ply][0]) {
        return;
    }
    moves[ply][1] = moves[ply][0];
    moves[ply][0] = killer;
}

inline std::array<Move, KILLERS_PER_PLY> get_killers(const int& ply) {
    return moves[ply];
}


}