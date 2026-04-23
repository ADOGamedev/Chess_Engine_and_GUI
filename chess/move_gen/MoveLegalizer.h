#pragma once

#include "MoveGen.h"
#include "../constants/constants.h"

#include "../game_state/GameState.h"

#include "../rays/rays_in_between.h"
#include "../pieces_moves/piece_attack_masks.h"

class MoveGen;
class GameState;

class MoveLegalizer {

public:
    MoveLegalizer() = default;
    
    MoveLegalizer(GameState* state, MoveGen& move_gen, uint64_t& moves, const Square square = SQ_NONE)
        : state(state), move_gen(move_gen), moves(&moves), square(square)  {}
    
    MoveLegalizer(GameState* state, MoveGen& move_gen)
        : state(state), move_gen(move_gen)  {}

    void calculate_pins_and_checks();
    void legalize_moves();

    bool is_in_check();

private:
    void calculate_pins();
    
    bool could_en_passant_expose_king();
    void remove_en_passant_captured_piece();
    void calculate_pin_for_atacker(const Square attacker_square);
    uint64_t get_overlapping_pieces_between_king_and_attacker(const Square square);
    void add_en_passant_captured_piece();
    void exclude_en_passant_move_if_needed();

    void calculate_checks();

    bool piece_can_attack_diagonally(const Piece piece);
    bool piece_can_attack_straight(const Piece piece);

    uint64_t get_ray_in_between_squares(const Square from_square, const Square to_square);

    void exclude_moves_if_pinned();
    void exclude_moves_if_king_in_check();
    void exclude_self_checks();

    void exclude_castling_if_needed();
    
public:
    bool is_castling_attacked(const CastlingType castling_type);

    bool is_king_in_check(const Colour colour);
    bool is_king_in_double_check(const Colour colour); 
    bool is_square_under_attack(const Square square);
    uint64_t attackers_for_square(const Square square);

private:
    uint64_t rook_moves_from_king = 0ULL;
    uint64_t bishop_moves_from_king = 0ULL;

    GameState* state;
    MoveGen& move_gen;
    uint64_t* moves;
    Square square;
};