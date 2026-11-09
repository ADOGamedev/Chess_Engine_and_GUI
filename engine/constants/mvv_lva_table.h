#pragma once

#include "../../chess/constants/piece_constants.h"

constexpr int MVV_LVA[N_PIECES_TYPES][N_PIECES_TYPES] = {
    { 15, 14, 13, 12, 11, 10 }, // victim: PAWN
    { 25, 24, 23, 22, 21, 20 }, // victim: KNIGHT
    { 35, 34, 33, 32, 31, 30 }, // victim: BISHOP
    { 45, 44, 43, 42, 41, 40 }, // victim: ROOK
    { 55, 54, 53, 52, 51, 50 }, // victim: QUEEN
    { 65, 64, 63, 62, 61, 60 }, // victim: KING
};