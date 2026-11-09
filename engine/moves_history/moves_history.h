#pragma once

#include <cstring>

#include "../../chess/constants/squares_constants.h"
#include "../../chess/move_gen/Move.h"


namespace MovesHistory {

static constexpr int32_t HISTORY_MAX = 16384;
inline int32_t history[N_COLOURS][N_SQUARES][N_SQUARES] = {0};

inline void update(const Colour& turn,   const Move& move, const int32_t& amount) {
    int32_t& entry = history[turn][move.from][move.to];
    int clamped_amount = std::clamp(amount, -HISTORY_MAX, HISTORY_MAX);

    entry += clamped_amount - entry * std::abs(clamped_amount) / HISTORY_MAX;
}


inline int32_t get_score(const Colour& turn, const Move& move) {
    return history[turn][move.from][move.to];
}

inline void reset() {
    memset(history, 0, sizeof(history));
}


};