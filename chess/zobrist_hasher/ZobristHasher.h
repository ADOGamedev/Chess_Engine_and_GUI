#pragma once


#include "../game_state/GameState.h"
#include "../move_gen/MoveGen.h"
#include "../utils/utils.h"

#include "polyglot_randoms.h"

enum PolyglotIndexes : int {
    PIECES_SQUARES = 0,
    CASTLING = 768,
    EN_PASSANT_INDEX = 772,
    TURN = 780
};

class GameState;


constexpr int piece_to_polyglot_piece_index[N_PIECES] = {1, 3, 5, 7, 9, 11, 0, 2, 4, 6, 8, 10};


class ZobristHasher {

public:
    static void initialize_zobrist_values();
    static void set_zobrist_key_of_state(GameState& state);

private:
    static void initialize_pieces_squares_zobrist_values();
    static void initialize_turn_zobrist_value();
    static void initialize_castling_rights_zobrist_values();
    static void initialize_en_passants_zobrist_values();

    static uint64_t get_pieces_square_zobrist_key(GameState& state);
    static uint64_t get_turn_zobrist_key(const Colour turn);
    static uint64_t get_castlings_zobrist_key(const std::array<bool, N_CASTLINGS>& castling_rights);
    static uint64_t get_en_passant_zobrist_key(const Square en_passant_square, const GameState& state);

    static uint64_t piece_square_zobrist_values[N_PIECES][N_SQUARES];
    static uint64_t castling_rights_zobrist_values[N_CASTLINGS];
    static uint64_t en_passant_zobrist_values[N_COLUMNS];
    static uint64_t turn_zobrist_value;
};