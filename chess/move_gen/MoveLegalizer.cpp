#include "MoveLegalizer.h"

void MoveLegalizer::calculate_pins_and_checks() {
    state->pins.fill(UINT64_MAX);
    state->check_mask = UINT64_MAX;

    calculate_pins();
    calculate_checks();
}

void MoveLegalizer::calculate_pins() {
    Square king_square = state->get_king_square(state->turn);
    
    uint64_t sliding_pieces = state->get_enemy_pieces(QUEEN) | state->get_enemy_pieces(ROOK) | state->get_enemy_pieces(BISHOP);
    
    bool en_passant_could_expose_king = could_en_passant_expose_king();
    if (en_passant_could_expose_king) {
        remove_en_passant_captured_piece();
    }

    uint64_t blockers = state->get_pieces_bitboard();
    
    rook_moves_from_king = move_gen.get_rook_moves(king_square, blockers);
    bishop_moves_from_king = move_gen.get_bishop_moves(king_square, blockers);
    
    for (int piece_index = 0; sliding_pieces != 0ULL;) {
        piece_index = count_trailing_zeros(sliding_pieces);
        set_bit_to_0(sliding_pieces, piece_index);

        Square attacker_square = static_cast<Square>(piece_index);

        calculate_pin_for_atacker(attacker_square);
    }

    if (en_passant_could_expose_king) {
        exclude_en_passant_move_if_needed();
        add_en_passant_captured_piece();
    }
}

// There is a special edge case where a pawn appears to move legally, but capturing en passant would expose your king to check.
//     _______________________________       _______________________________
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//    |___|___|___|___|___|___|___|___|     |___|___|_P_|___|___|___|___|___|
//    |_K_|_P_|_p_|___|___|___|___|_r_| --> |_K_|___|___|___|___|___|___|_r_| <- notice how this enemy rook now attacks the king.
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//    |___|___|___|___|___|___|___|___|     |___|___|___|___|___|___|___|___|
//
// This function detects this scenario, and the following functions handle it.

bool MoveLegalizer::could_en_passant_expose_king() {
    if (state->en_passant == SQ_NONE) {
        return false;
    }

    Square king_square = state->get_king_square(state->turn);

    Square en_passant_adjacent_squares[2] = {  
        static_cast<Square>( state->en_passant_capture - 1 ),
        static_cast<Square>( state->en_passant_capture + 1 )
    };

    uint64_t friendly_pawns = state->get_friendly_pieces(PAWN);
    bool exactly_one_pawn_adjacent =    is_bit_1(friendly_pawns, en_passant_adjacent_squares[0]) ^
                                        is_bit_1(friendly_pawns, en_passant_adjacent_squares[1]);
        
    bool king_aligned_in_row = square_to_row[king_square] == square_to_row[state->en_passant_capture];      
        
    return exactly_one_pawn_adjacent && king_aligned_in_row;
}

void MoveLegalizer::remove_en_passant_captured_piece() {
    state->remove_piece_from(state->en_passant_capture);
}

void MoveLegalizer::calculate_pin_for_atacker(const Square attacker_square) {
    Square king_square = state->get_king_square(state->turn);

    uint64_t pin_ray = get_ray_in_between_squares(king_square, attacker_square);
    uint64_t overlaping_pieces = get_overlapping_pieces_between_king_and_attacker(attacker_square) & pin_ray;

    if (overlaping_pieces == 0ULL) {
        return;
    }

    Square overlaping_square = static_cast<Square>( count_trailing_zeros(overlaping_pieces) );

    state->pins[overlaping_square] = pin_ray;
}

uint64_t MoveLegalizer::get_overlapping_pieces_between_king_and_attacker(const Square square) {
    Piece piece = state->get_piece_at(square);

    uint64_t moves_from_king_square = 0ULL;
    uint64_t piece_moves = 0ULL;
    uint64_t blockers = state->get_pieces_bitboard();

    if (piece_can_attack_straight(piece)) {
        moves_from_king_square |= rook_moves_from_king;
        piece_moves |= move_gen.get_rook_moves(square, blockers);
    }
    if (piece_can_attack_diagonally(piece)) {
        moves_from_king_square |= bishop_moves_from_king;
        piece_moves |= move_gen.get_bishop_moves(square, blockers);
    }

    return moves_from_king_square & piece_moves & state->get_friendly_bitboard();
}

void MoveLegalizer::add_en_passant_captured_piece() {
    Piece enemy_pawn = state->is_white_turn() ? BLACK_PAWN : WHITE_PAWN;
    state->place_piece_at(state->en_passant_capture, enemy_pawn);
}

void MoveLegalizer::exclude_en_passant_move_if_needed() {
    Square en_passant_adjacent_squares[2] = {  
        static_cast<Square>( state->en_passant_capture - 1 ),
        static_cast<Square>( state->en_passant_capture + 1 )
    };

    for (Square adjacent_square : en_passant_adjacent_squares) {
        bool pawn_is_pinned = state->pins[adjacent_square] != UINT64_MAX;
        bool en_passant_capture_intersects_pin = is_bit_1(state->pins[adjacent_square], state->en_passant_capture);

        if (state->is_friendly(adjacent_square) &&
            pawn_is_pinned && en_passant_capture_intersects_pin) {

                state->pins[adjacent_square] = UINT64_MAX;
                set_bit_to_0(state->pins[adjacent_square], state->en_passant);
        }
    }
}

void MoveLegalizer::calculate_checks() {
    Square king_square = state->get_king_square(state->turn);

    uint64_t attackers = attackers_for_square(king_square);
    
    if (attackers == 0ULL) {
        return;
    }

    Square attacker_square = static_cast<Square>(count_trailing_zeros(attackers));

    state->check_mask = get_ray_in_between_squares(king_square, attacker_square);
    set_bit_to_1(state->check_mask, attacker_square);

    if (attacker_square == state->en_passant_capture) {
        set_bit_to_1(state->check_mask, state->en_passant);
    }
}

bool MoveLegalizer::piece_can_attack_diagonally(const Piece piece) {
    PieceType piece_type = piece_to_piece_type[piece];

    return piece_type == BISHOP || piece_type == QUEEN;
}

bool MoveLegalizer::piece_can_attack_straight(const Piece piece) {
    PieceType piece_type = piece_to_piece_type[piece];

    return piece_type == ROOK || piece_type == QUEEN;
}

uint64_t MoveLegalizer::get_ray_in_between_squares(const Square from_square, const Square to_square) {
    return rays_in_between[from_square][to_square];
}

void MoveLegalizer::legalize_moves() {
    Piece piece = state->get_piece_at(square);
    PieceType piece_type = piece_to_piece_type[piece];

    if (piece_type == KING) {
        exclude_self_checks();
        exclude_castling_if_needed();
    }
    else {
        if (is_king_in_double_check(state->turn)) {
            *moves = 0ULL;
            return;
        }

        exclude_moves_if_pinned();
        exclude_moves_if_king_in_check();
    }
}

void MoveLegalizer::exclude_moves_if_pinned() {
    *moves &= state->pins[square];
}

void MoveLegalizer::exclude_moves_if_king_in_check() {
    *moves &= state->check_mask;
}

void MoveLegalizer::exclude_self_checks() {
    Piece king = state->is_white_turn() ? WHITE_KING : BLACK_KING;
    Square king_square = state->get_king_square(state->turn);

    uint64_t curr_moves = *moves;

    state->remove_piece_from(king_square);

    while (curr_moves) {
        Square square = static_cast<Square>(count_trailing_zeros(curr_moves));
        set_bit_to_0(curr_moves, square);

        if (is_square_under_attack(square)) {
            set_bit_to_0(*moves, square);
        }
    }

    state->place_piece_at(king_square, king);
}

void MoveLegalizer::exclude_castling_if_needed() {
    Square king_square = state->get_king_square(state->turn);

    bool has_rights_for_short_castling = state->is_white_turn() ?
        state->has_castling_right(WHITE_SHORT_CASTLING) :
        state->has_castling_right(BLACK_SHORT_CASTLING);

    bool has_rights_for_long_castling = state->is_white_turn() ?
        state->has_castling_right(WHITE_LONG_CASTLING) :
        state->has_castling_right(BLACK_LONG_CASTLING);

    if (!has_rights_for_short_castling || is_castling_attacked(CastlingType::SHORT_CASTLING)) {
        set_bit_to_0(*moves, king_square + 2);
    }
    if (!has_rights_for_long_castling || is_castling_attacked(CastlingType::LONG_CASTLING)) {
        set_bit_to_0(*moves, king_square - 2);
    }
}

bool MoveLegalizer::is_castling_attacked(const CastlingType castling_type) {
    Square king_square = state->get_king_square(state->turn);

    for (int castling_offset : castling_path_offsets[castling_type]) {
        Square castling_square = static_cast<Square>( static_cast<int>(king_square) + castling_offset );

        if (is_king_in_check(state->turn) || is_square_under_attack(castling_square)) {
            return true;
        }
    }

    return false;
}

bool MoveLegalizer::is_king_in_check(const Colour colour) {
    Square king_square = state->get_king_square(colour);
    
    return is_square_under_attack(king_square);
}

bool MoveLegalizer::is_in_check() {
    return ~state->check_mask != 0ULL;
}

bool MoveLegalizer::is_square_under_attack(const Square square) {
    return attackers_for_square(square);
}

bool MoveLegalizer::is_king_in_double_check(const Colour colour) {
    Square king_square = state->get_king_square(colour);
    uint64_t attackers = attackers_for_square(king_square);
    int attackers_count = count_bits_in(attackers);

    return attackers_count > 1;
}

uint64_t MoveLegalizer::attackers_for_square(const Square square) {
    uint64_t enemy_bishops = state->get_enemy_pieces(BISHOP);
    uint64_t enemy_rooks = state->get_enemy_pieces(ROOK);
    uint64_t enemy_queens = state->get_enemy_pieces(QUEEN);
    uint64_t enemy_pawns = state->get_enemy_pieces(PAWN);
    uint64_t enemy_knights = state->get_enemy_pieces(KNIGHT);
    uint64_t enemy_king = state->get_enemy_pieces(KING);

    uint64_t blockers = state->get_pieces_bitboard();
    
    uint64_t diagonal_attacker_pieces = move_gen.get_bishop_moves(square, blockers) & (enemy_bishops | enemy_queens);
    uint64_t straight_attacker_pieces = move_gen.get_rook_moves(square, blockers) & (enemy_rooks | enemy_queens);
    uint64_t attacker_pawns = pawn_attacks[state->turn][square] & enemy_pawns;
    uint64_t attacker_knights = knight_moves_lookup[square] & enemy_knights;
    uint64_t attacker_king = king_moves_lookup[square] & enemy_king;

    return diagonal_attacker_pieces | straight_attacker_pieces | attacker_pawns | attacker_knights | attacker_king;
}

