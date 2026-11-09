#pragma once

#include "../../chess/constants/constants.h"
#include "positional_piece_scores.h"
#include "mvv_lva_table.h"
#include "killer_moves.h"


constexpr int CHECKMATE_SCORE = 10000000;

constexpr int MIDDLE_GAME_PIECES_SCORES[N_PIECES_TYPES] = { 82, 337, 365, 477, 1025,  0};
constexpr int END_GAME_PIECES_SCORES[N_PIECES_TYPES] = { 94, 281, 297, 512,  936,  0};

constexpr int INF = 100000000;

