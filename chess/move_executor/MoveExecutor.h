#pragma once

#include "../game_state/GameState.h"
#include "../move_gen/MoveLegalizer.h"
#include "../zobrist_hasher/ZobristHasher.h"

class MoveExecutor {

public:
    MoveExecutor(const Move& move, GameState* state) : move(move), state(state) {}
    
    static void do_move(const Move& move, GameState* game_state);
    static void undo_move(const Move& move, GameState* game_state);

    static void do_null_move(GameState* game_state);
    static void undo_null_move(GameState* game_state, const Square& prev_en_passant, const Square& prev_en_passant_capture);
    
private:
    void move_piece() const;

    void handle_capture() const;

    void handle_castling() const;
    void update_castling_rights() const;
    bool try_revoke_castling_for_rook(const Square square) const;
    void revoke_all_castlings_for_colour(const Colour colour) const;
    void perform_castling_rook_move() const;

    void handle_promotion() const;
    void handle_en_passant() const;
    void apply_en_passant() const;
    void handle_fifty_moves_rule_counter() const;

    void switch_piece_position(const Move& move, Piece piece) const;

    void undo_move_piece() const;

    void handle_undo_en_passant() const;
    void handle_undo_castling() const;
    void handle_undo_capture() const;
    void handle_undo_fifty_moves_rule_counter() const;
    
    void undo_switch_piece_position(const Move& piece_move, Piece piece) const;

    bool is_pawn_double_push() const;
    bool is_move_en_passant() const;

    
    GameState* state;
    const Move move;
};