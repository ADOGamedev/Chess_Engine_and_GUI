#include "ZobristHasher.h"

uint64_t ZobristHasher::piece_square_zobrist_values[N_PIECES][N_SQUARES] = {};
uint64_t ZobristHasher::turn_zobrist_value = 0ULL;
uint64_t ZobristHasher::castling_rights_zobrist_values[N_CASTLINGS] = {};
uint64_t ZobristHasher::en_passant_zobrist_values[N_COLUMNS] = {};

void ZobristHasher::initialize_zobrist_values() {
    initialize_pieces_squares_zobrist_values();
    initialize_castling_rights_zobrist_values();
    initialize_en_passants_zobrist_values();
    initialize_turn_zobrist_value();
}

void ZobristHasher::initialize_pieces_squares_zobrist_values() {
    for (Square square = SQ_0; square < N_SQUARES; square++) {
        for (Piece piece = WHITE_PAWN; piece < N_PIECES; piece++) {
            int polyglot_piece_index = piece_to_polyglot_piece_index[piece];
            int polyglot_index = PolyglotIndexes::PIECES_SQUARES + polyglot_piece_index * N_SQUARES + square;

            uint64_t value = Random64[polyglot_index];
            piece_square_zobrist_values[piece][square] = value;
        }
    }
}

void ZobristHasher::initialize_castling_rights_zobrist_values() {
    memcpy(castling_rights_zobrist_values, &Random64[PolyglotIndexes::CASTLING], N_CASTLINGS * sizeof(uint64_t));
}

void ZobristHasher::initialize_en_passants_zobrist_values() {
    memcpy(en_passant_zobrist_values, &Random64[PolyglotIndexes::EN_PASSANT_INDEX], N_COLUMNS * sizeof(uint64_t));
}

void ZobristHasher::initialize_turn_zobrist_value() {
    turn_zobrist_value = Random64[PolyglotIndexes::TURN];
}

void ZobristHasher::set_zobrist_key_of_state(GameState& state) {
    state.key = get_pieces_square_zobrist_key(state) ^
                get_turn_zobrist_key(state.turn) ^
                get_castlings_zobrist_key(state.castling_rights) ^
                get_en_passant_zobrist_key(state.en_passant, state);
}

uint64_t ZobristHasher::get_pieces_square_zobrist_key(GameState& state) {
    uint64_t key = 0ULL;

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Piece piece = state.get_piece_at(square);

        if (piece != PIECE_NONE) {
            key ^= piece_square_zobrist_values[piece][square];
        }
    }

    return key;
}

uint64_t ZobristHasher::get_turn_zobrist_key(const Colour turn) {
    return turn == WHITE ? turn_zobrist_value : 0ULL;
}

uint64_t ZobristHasher::get_castlings_zobrist_key(const std::array<bool, N_CASTLINGS>& castling_rights) {
    uint64_t key = 0ULL;

    for (Castling castling = WHITE_SHORT_CASTLING; castling < N_CASTLINGS; castling++) {
        if (castling_rights[castling]) {
            key ^= castling_rights_zobrist_values[castling];
        }
    }

    return key;
}

uint64_t ZobristHasher::get_en_passant_zobrist_key(const Square en_passant_square, const GameState& state) {
    if (en_passant_square == SQ_NONE) {
        return 0ULL;
    }

    uint64_t friendly_pawns = state.get_pieces_bitboard(PAWN) & state.get_friendly_bitboard();
    int row_offset = state.is_white_turn() ? -8 : 8;
    bool pawn_at_left = is_bit_1(friendly_pawns, en_passant_square + row_offset - 1);
    bool pawn_at_right = is_bit_1(friendly_pawns, en_passant_square + row_offset + 1);

    if (!pawn_at_left && !pawn_at_right) {
        return 0ULL;
    }

    Column en_passant_column = square_to_column[en_passant_square];
    return en_passant_zobrist_values[en_passant_column];
}