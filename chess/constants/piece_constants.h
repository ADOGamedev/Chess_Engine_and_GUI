#pragma once

#include <cstdint>

#include "colour_constants.h"

enum Piece : uint8_t {
    WHITE_PAWN,
    WHITE_KNIGHT,
    WHITE_BISHOP,
    WHITE_ROOK,
    WHITE_QUEEN,
    WHITE_KING,

    BLACK_PAWN,
    BLACK_KNIGHT,
    BLACK_BISHOP,
    BLACK_ROOK,
    BLACK_QUEEN,
    BLACK_KING,

    N_PIECES,
    PIECE_NONE
};

constexpr char piece_to_char[N_PIECES] = {
    'P', 'N', 'B', 'R', 'Q', 'K', 
    'p', 'n', 'b', 'r', 'q', 'k'  
};

constexpr Piece operator++(Piece& piece, int) {
    Piece old = piece;
    piece = static_cast<Piece>(static_cast<int>(piece) + 1);
    return old;
}

constexpr void operator+=(Piece& piece, const int num) {
    piece = static_cast<Piece>(static_cast<int>(piece) + num);
}

enum PieceType : uint8_t {
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,

    N_PIECES_TYPES,
    PIECE_TYPE_NONE
};

constexpr PieceType operator++(PieceType& piece_type, int) {
    PieceType old = piece_type;
    piece_type = static_cast<PieceType>(static_cast<int>(piece_type) + 1);
    return old;
}

constexpr char piece_type_to_char[N_PIECES_TYPES] = {
    'p', 'n', 'b', 'r', 'q', 'k'
};

constexpr Piece piece_type_to_piece[N_COLOURS][N_PIECES_TYPES] = {
    {WHITE_PAWN, WHITE_KNIGHT, WHITE_BISHOP, WHITE_ROOK, WHITE_QUEEN, WHITE_KING},
    {BLACK_PAWN, BLACK_KNIGHT, BLACK_BISHOP, BLACK_ROOK, BLACK_QUEEN, BLACK_KING}
};