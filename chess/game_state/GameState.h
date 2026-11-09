#pragma once

#include <cstdint>
#include <array>
#include <unordered_map>
#include <string>

#include "../constants/constants.h"
#include "../move_gen/Move.h"
#include "../utils/utils.h"
#include "../fen_loader/FenLoader.h"


class GameState {

public:
    GameState();

    void switch_turns();
    bool is_white_turn() const;

    Piece get_piece_at(const Square square) const;

    void place_piece_at(const Square square, const Piece piece);
    void remove_piece_from(const Square square);

    void remove_castling_right(const Castling castling);
    void give_castling_right(const Castling castling);
    bool has_castling_right(const Castling castling);

    void add_curr_key_to_repetition_list();
    void remove_last_key_from_repetition_list();

    void add_move_to_history(const Move& move);
    std::string convert_moves_history_to_str();
    
    inline bool is_empty(const Square square) const {
        return (1ULL << square) & get_empty_bitboard();
    }

    inline bool is_occupied(const Square square) const {
        return (1ULL << square) & get_pieces_bitboard();
    }

    inline bool is_friendly(const Square square) const {
        return (1ULL << square) & get_friendly_bitboard();
    }

    inline bool is_enemy(const Square square) const {
        return (1ULL << square) & get_enemy_bitboard();
    }

    inline uint64_t get_empty_bitboard() const {
        return ~get_pieces_bitboard();
    }

    inline uint64_t get_enemy_bitboard() const {
        return get_pieces_bitboard(!turn);
    }

    inline uint64_t get_friendly_bitboard() const {
        return get_pieces_bitboard(turn);
    }

    inline uint64_t get_pieces_bitboard() const {
        return get_pieces_bitboard(WHITE) | get_pieces_bitboard(BLACK);
    }

    inline uint64_t get_pieces_bitboard(const Colour colour) const {
        return colours_bitboard[colour];
    }

    inline uint64_t get_pieces_bitboard(const PieceType piece_type) const {
        return board[piece_type] | board[piece_type + N_PIECES_TYPES];
    }

    inline uint64_t get_enemy_pieces(const PieceType piece_type) const {
        return board[piece_type + (!turn) * N_PIECES_TYPES];
    }

    inline uint64_t get_friendly_pieces(const PieceType piece_type) const {
        return get_pieces_bitboard(piece_type) & get_friendly_bitboard();
    }

    inline uint64_t get_pieces_bitboard(const Piece piece) const {
        return board[piece];
    }

    Square get_king_square(const Colour colour) const;
    
    std::array<uint64_t, N_PIECES> board = {};
    std::array<uint64_t, N_COLOURS> colours_bitboard = {};
    std::array<Piece, 64> board_squares = {};

    Colour turn = WHITE;

    int game_phase = MAX_GAME_PHASE;
    
    Square en_passant = SQ_NONE;
    Square en_passant_capture = SQ_NONE;
    int fifty_moves_rule_counter = 0;

    std::vector<Move> moves_history = {};
    std::vector<uint64_t> repetition_list = {};
    uint64_t key = 0ULL;

    std::array<bool, N_CASTLINGS> castling_rights = {};

    std::array<uint64_t, N_SQUARES> pins = {};
    uint64_t check_mask = UINT64_MAX;
};