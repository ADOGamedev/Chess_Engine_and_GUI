#pragma once

#include <exception>

#include "Move.h"
#include "../game_state/GameState.h"
#include "MoveLegalizer.h"
#include "MoveFlagAssigner.h"

#include "../pieces_moves/piece_attack_masks.h"
#include "../pieces_moves/rook_moves_lookup.h"
#include "../pieces_moves/bishop_moves_lookup.h"
#include "../pieces_moves/knight_moves_lookup.h"
#include "../pieces_moves/king_moves_lookup.h"

#include "../rays/rays_in_between.h"
#include "../magic_numbers/magics.h"

class MoveLegalizer;
class MovesLookupsGen;
class GameState;

struct RayAttack {
    uint64_t mask = 0ULL;
    Square last_square = SQ_NONE;
};

class MoveGen {
    
public:
    MoveGen(GameState* state) :
        state(state) {}
    
    void update_available_legal_moves();
    
    bool can_move_to_square(const Square square) const;
    
    uint64_t get_curr_moves() const;
    void clear_moves();
    
    void update_legal_moves_for_square(const Square square);
    void update_pseudo_legal_moves_for_square(const Square square);
    
private:
    void update_available_moves_for_square(const Square square);
    void add_all_promotions_to_available_moves(const Move& move);
    void add_move_to_available_moves(const Move& move);

    void include_castling_if_possible();
    bool is_castling_path_empty(const CastlingType castling_type) const;

    void exclude_friendly_pieces_from_moves();
    
public:
    uint64_t generate_pawn_moves(const Square square) const;
    bool can_pawn_double_push(const Square square) const;
    uint64_t get_pawn_captures(const Square square) const;

    uint64_t get_bishop_moves(const Square square, const uint64_t blockers) const;
    uint64_t get_rook_moves(const Square square, const uint64_t blockers) const;
    uint64_t get_queen_moves(const Square square) const;
    int get_blockers_magic_index(const Square square, const PieceType piece_type, const uint64_t blockers) const;  

    Move create_move(const Square from, const Square to) const;

    GameState* state;
    uint64_t curr_moves = 0ULL;
    std::array<Move, MAX_POSSIBLE_AVAILABLE_MOVES> available_moves;
    int available_moves_count = 0;

};