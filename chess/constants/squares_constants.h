#pragma once

#include <cstdint>

#include "colour_constants.h"
#include "piece_constants.h"

enum Square : uint8_t {
    SQ_0,  SQ_1,  SQ_2,  SQ_3,  SQ_4,  SQ_5,  SQ_6,  SQ_7,
    SQ_8,  SQ_9, SQ_10, SQ_11, SQ_12, SQ_13, SQ_14, SQ_15,
    SQ_16, SQ_17, SQ_18, SQ_19, SQ_20, SQ_21, SQ_22, SQ_23,
    SQ_24, SQ_25, SQ_26, SQ_27, SQ_28, SQ_29, SQ_30, SQ_31,
    SQ_32, SQ_33, SQ_34, SQ_35, SQ_36, SQ_37, SQ_38, SQ_39,
    SQ_40, SQ_41, SQ_42, SQ_43, SQ_44, SQ_45, SQ_46, SQ_47,
    SQ_48, SQ_49, SQ_50, SQ_51, SQ_52, SQ_53, SQ_54, SQ_55,
    SQ_56, SQ_57, SQ_58, SQ_59, SQ_60, SQ_61, SQ_62, SQ_63,
    
    N_SQUARES,
    SQ_NONE
};

constexpr Square operator++(Square& square, int) {
    Square old = square;
    square = static_cast<Square>(static_cast<int>(square) + 1);
    return old;
}

constexpr void operator+=(Square& square, const int num) {
    square = static_cast<Square>(static_cast<int>(square) + num);
}

namespace PawnOffset {
enum PawnOffset : uint8_t {
    FRONT = 8,
    DOUBLE_FRONT = 16,
    LEFT = 7,
    RIGHT = 9
};
}

enum Row : uint8_t {
    ROW_1,
    ROW_2,
    ROW_3,
    ROW_4,
    ROW_5,
    ROW_6,
    ROW_7,
    ROW_8,

    N_ROWS
};

enum RowBitboard : uint64_t {
    ROW_1_BITBOARD = 0xFF,
    ROW_2_BITBOARD = ROW_1_BITBOARD << 8,
    ROW_3_BITBOARD = ROW_1_BITBOARD << 16,
    ROW_4_BITBOARD = ROW_1_BITBOARD << 24,
    ROW_5_BITBOARD = ROW_1_BITBOARD << 32,
    ROW_6_BITBOARD = ROW_1_BITBOARD << 40,
    ROW_7_BITBOARD = ROW_1_BITBOARD << 48,
    ROW_8_BITBOARD = ROW_1_BITBOARD << 56
};

enum Column : uint8_t {
    COLUMN_1,
    COLUMN_2,
    COLUMN_3,
    COLUMN_4,
    COLUMN_5,
    COLUMN_6,
    COLUMN_7,
    COLUMN_8,

    N_COLUMNS
};

constexpr Column operator++(Column& column, int) {
    Column old = column;
    column = static_cast<Column>(static_cast<int>(column) + 1);
    return old;
}

enum ColumnBitboard : uint64_t {
    COLUMN_1_BITBOARD = 0x101010101010101,
    COLUMN_2_BITBOARD = COLUMN_1_BITBOARD << 1,
    COLUMN_3_BITBOARD = COLUMN_1_BITBOARD << 2,
    COLUMN_4_BITBOARD = COLUMN_1_BITBOARD << 3,
    COLUMN_5_BITBOARD = COLUMN_1_BITBOARD << 4,
    COLUMN_6_BITBOARD = COLUMN_1_BITBOARD << 5,
    COLUMN_7_BITBOARD = COLUMN_1_BITBOARD << 6,
    COLUMN_8_BITBOARD = COLUMN_1_BITBOARD << 7

};

constexpr Colour piece_to_color[N_PIECES] = {
    WHITE, WHITE, WHITE, WHITE, WHITE, WHITE,
    BLACK, BLACK, BLACK, BLACK, BLACK, BLACK
};

constexpr PieceType piece_to_piece_type[N_PIECES] = {
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING,
    PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING
};

constexpr Row square_to_row[64] = {
    ROW_1, ROW_1, ROW_1, ROW_1, ROW_1, ROW_1, ROW_1, ROW_1,
    ROW_2, ROW_2, ROW_2, ROW_2, ROW_2, ROW_2, ROW_2, ROW_2,
    ROW_3, ROW_3, ROW_3, ROW_3, ROW_3, ROW_3, ROW_3, ROW_3,
    ROW_4, ROW_4, ROW_4, ROW_4, ROW_4, ROW_4, ROW_4, ROW_4,
    ROW_5, ROW_5, ROW_5, ROW_5, ROW_5, ROW_5, ROW_5, ROW_5,
    ROW_6, ROW_6, ROW_6, ROW_6, ROW_6, ROW_6, ROW_6, ROW_6,
    ROW_7, ROW_7, ROW_7, ROW_7, ROW_7, ROW_7, ROW_7, ROW_7,
    ROW_8, ROW_8, ROW_8, ROW_8, ROW_8, ROW_8, ROW_8, ROW_8,
};

constexpr Column square_to_column[64] = {
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
    COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4, COLUMN_5, COLUMN_6, COLUMN_7, COLUMN_8,
};