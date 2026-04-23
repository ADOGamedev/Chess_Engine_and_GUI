#include "MoveExecutor.h"


void MoveExecutor::do_move(const Move& move, GameState* state) {
    MoveExecutor move_executor(move, state);
    
    state->en_passant = SQ_NONE;
    state->en_passant_capture = SQ_NONE;
    move_executor.move_piece();
    state->switch_turns();

    ZobristHasher::set_zobrist_key_of_state(*state);
    state->add_curr_key_to_repetition_list();

    state->add_move_to_history(move);
}

void MoveExecutor::undo_move(const Move& move, GameState* state) {
    MoveExecutor move_executor(move, state);

    state->switch_turns();
    move_executor.undo_move_piece();
    
    ZobristHasher::set_zobrist_key_of_state(*state);
    state->remove_last_key_from_repetition_list();

    state->remove_last_move_from_history();
}

void MoveExecutor::do_null_move(GameState* state) {
    state->en_passant = SQ_NONE;
    state->en_passant_capture = SQ_NONE;

    state->switch_turns();

    ZobristHasher::set_zobrist_key_of_state(*state);
    state->add_curr_key_to_repetition_list();
}

void MoveExecutor::undo_null_move(GameState* state) {
    state->switch_turns();
    
    ZobristHasher::set_zobrist_key_of_state(*state);
    state->remove_last_key_from_repetition_list();

    state->remove_last_move_from_history();
}


void MoveExecutor::move_piece() const {
    handle_capture();
    handle_castling();
    
    switch_piece_position(move, move.moved_piece);
    
    handle_promotion();
    handle_en_passant();
    handle_fifty_moves_rule_counter();
}

void MoveExecutor::handle_capture() const {
    if (move.is_capture()) {
        state->remove_piece_from(move.to);
    }
}

void MoveExecutor::handle_castling() const {
    update_castling_rights();

    PieceType moved_piece_type = piece_to_piece_type[move.moved_piece];

    if (move.is_castle() && moved_piece_type == KING) {
        perform_castling_rook_move();
    }   
}

void MoveExecutor::update_castling_rights() const {
    PieceType moved_piece_type = piece_to_piece_type[move.moved_piece];
    PieceType captured_piece_type = piece_to_piece_type[move.captured_piece];

    if (moved_piece_type == ROOK) {
        try_revoke_castling_for_rook(move.from);
    }

    if (captured_piece_type == ROOK) {
        try_revoke_castling_for_rook(move.to);
    }

    if (moved_piece_type == KING) {
        revoke_all_castlings_for_colour(state->turn);
    }
}

bool MoveExecutor::try_revoke_castling_for_rook(const Square square) const {
    switch (square) {
        case initial_rooks_squares[WHITE_LEFT_ROOK]: 
            state->remove_castling_right(WHITE_LONG_CASTLING);
            return true;

        case initial_rooks_squares[WHITE_RIGHT_ROOK]: 
            state->remove_castling_right(WHITE_SHORT_CASTLING);
            return true;

        case initial_rooks_squares[BLACK_LEFT_ROOK]: 
            state->remove_castling_right(BLACK_LONG_CASTLING);
            return true;

        case initial_rooks_squares[BLACK_RIGHT_ROOK]: 
            state->remove_castling_right(BLACK_SHORT_CASTLING);
            return true;
        
        default:
            return false;
    }
}

void MoveExecutor::revoke_all_castlings_for_colour(const Colour colour) const {
    if (colour == WHITE) {
        state->remove_castling_right(WHITE_SHORT_CASTLING);
        state->remove_castling_right(WHITE_LONG_CASTLING);
    }
    else {
        state->remove_castling_right(BLACK_SHORT_CASTLING);
        state->remove_castling_right(BLACK_LONG_CASTLING);
    }
}   

void MoveExecutor::perform_castling_rook_move() const {
    Castling castling = king_destination_square_to_castling[move.to];
    Colour turn_colour = state->turn;

    Move rook_move = castling_moves_for_rook[castling];
    Piece piece_to_move = turn_colour == WHITE ? WHITE_ROOK : BLACK_ROOK;
    rook_move.moved_piece = piece_to_move;

    switch_piece_position(rook_move, piece_to_move);
}

void MoveExecutor::handle_promotion() const {
    Piece promotion_piece = piece_type_to_piece[state->turn][move.promotion_piece_type];

    if (move.is_promotion()) {
        state->remove_piece_from(move.to);
        state->place_piece_at(move.to, promotion_piece);
    }
}

void MoveExecutor::handle_en_passant() const {
    if (is_pawn_double_push()) {
        state->en_passant = static_cast<Square>( static_cast<int>(move.from + move.to) / 2 );
        state->en_passant_capture = move.to;
    }
    
    if (is_move_en_passant()) {
        apply_en_passant();
    }
}

void MoveExecutor::apply_en_passant() const {
    state->remove_piece_from(move.prev_en_passant_capture);
}

void MoveExecutor::handle_fifty_moves_rule_counter() const {
    state->fifty_moves_rule_counter++;
    
    bool is_pawn_move = piece_to_piece_type[move.moved_piece] == PAWN;
    
    if (is_pawn_move || move.is_capture()) {
        state->fifty_moves_rule_counter = 0;

        // Clear the repetition list because after a pawn move or capture
        // you can not repeat a position that already occured
        state->repetition_list.clear();
    }
}

void MoveExecutor::switch_piece_position(const Move& piece_move, Piece piece) const {
    state->remove_piece_from(piece_move.from);
    state->place_piece_at(piece_move.to, piece);
}

void MoveExecutor::undo_move_piece() const {
    handle_undo_en_passant();
    
    undo_switch_piece_position(move, move.moved_piece);
    
    handle_undo_castling();
    handle_undo_capture();
    handle_undo_fifty_moves_rule_counter();
}

void MoveExecutor::handle_undo_en_passant() const {
    if (move.is_en_passant()) {
        state->place_piece_at(move.prev_en_passant_capture, move.captured_piece);
    }

    state->en_passant = move.prev_en_passant;
    state->en_passant_capture = move.prev_en_passant_capture;
}

void MoveExecutor::handle_undo_castling() const {
    state->castling_rights = move.prev_castling_rights;
    if (!move.is_castle()) {
        return;
    }

    Castling castling = king_destination_square_to_castling[move.to];
    Colour turn_colour = state->turn;

    Move rook_move = castling_moves_for_rook[castling];
    Piece piece_to_move = turn_colour == WHITE ? WHITE_ROOK : BLACK_ROOK;

    undo_switch_piece_position(rook_move, piece_to_move);
}

void MoveExecutor::handle_undo_capture() const {
    if (move.is_capture()) {
        state->place_piece_at(move.to, move.captured_piece);
    }
}

void MoveExecutor::handle_undo_fifty_moves_rule_counter() const {
    state->fifty_moves_rule_counter = move.prev_fifty_moves_rule_counter;
}

void MoveExecutor::undo_switch_piece_position(const Move& piece_move, Piece piece) const {
    state->remove_piece_from(piece_move.to);
    state->place_piece_at(piece_move.from, piece);
}

bool MoveExecutor::is_pawn_double_push() const {
    return move.flag == MoveFlag::DOUBLE_PAWN_PUSH;
}

bool MoveExecutor::is_move_en_passant() const {
    return move.flag == MoveFlag::EN_PASSANT;
}
