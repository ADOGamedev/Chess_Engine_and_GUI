#pragma once

#include <cstdint>
#include <array>
#include <vector>

#include "colour_constants.h"
#include "squares_constants.h"

enum Castling : uint8_t {
    WHITE_SHORT_CASTLING,
    WHITE_LONG_CASTLING,
    BLACK_SHORT_CASTLING,
    BLACK_LONG_CASTLING,

    N_CASTLINGS,
    CASTLING_NONE
};

constexpr char castling_to_char[N_CASTLINGS] = {'K', 'Q', 'k', 'q'};

constexpr Castling operator++(Castling& castling, int) {
    Castling old = castling;
    castling = static_cast<Castling>(static_cast<int>(castling) + 1);
    return old;
}

enum CastlingType : uint8_t {
    SHORT_CASTLING,
    LONG_CASTLING,

    N_CASTLING_TYPES
};

constexpr Castling castling_type_to_castling[N_COLOURS][N_CASTLING_TYPES] = {
    {WHITE_SHORT_CASTLING, WHITE_SHORT_CASTLING},
    {BLACK_LONG_CASTLING, BLACK_LONG_CASTLING}
};

const std::array<std::vector<int>, N_CASTLING_TYPES> castling_offsets = {{
    {1, 2},
    {-1, -2, -3}
}};

enum InitialRooks : uint8_t {
    WHITE_RIGHT_ROOK,
    WHITE_LEFT_ROOK,
    BLACK_RIGHT_ROOK,
    BLACK_LEFT_ROOK,

    N_INITIAL_ROOKS
};

constexpr InitialRooks operator++(InitialRooks& initial_rook, int) {
    InitialRooks old = initial_rook;
    initial_rook = static_cast<InitialRooks>(static_cast<int>(initial_rook) + 1);
    return old;
}

constexpr Square initial_rooks_squares[N_INITIAL_ROOKS] = {
    SQ_7, SQ_0, SQ_63, SQ_56
};

constexpr int castling_path_offsets[N_CASTLING_TYPES][2] = {
    {1, 2},
    {-1, -2}
};

constexpr Castling king_destination_square_to_castling[N_SQUARES] = {
    CASTLING_NONE, CASTLING_NONE, WHITE_LONG_CASTLING, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, WHITE_SHORT_CASTLING, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE,
    CASTLING_NONE, CASTLING_NONE, BLACK_LONG_CASTLING, CASTLING_NONE, CASTLING_NONE, CASTLING_NONE, BLACK_SHORT_CASTLING, CASTLING_NONE
};

constexpr Castling initial_rook_to_castling[N_INITIAL_ROOKS] = {
    WHITE_SHORT_CASTLING,
    WHITE_LONG_CASTLING,
    BLACK_SHORT_CASTLING,
    BLACK_LONG_CASTLING
};