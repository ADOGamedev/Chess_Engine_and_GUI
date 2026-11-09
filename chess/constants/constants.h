#pragma once

#include "castling_constants.h"
#include "colour_constants.h"
#include "directions_constants.h"
#include "game_over_constants.h"
#include "occupancy_bits_count.h"
#include "piece_constants.h"
#include "squares_constants.h"
#include "time.h"

const int MAX_POSSIBLE_AVAILABLE_MOVES = 218;
const int MAX_NUMBER_OF_SIMULTANEOUS_CHECKS = 2;

constexpr int MAX_GAME_PHASE = 24;

constexpr int GAME_PHASE_PIECE_VALUES[N_PIECES_TYPES] = {
    0, 1, 1, 2, 4, 0
};