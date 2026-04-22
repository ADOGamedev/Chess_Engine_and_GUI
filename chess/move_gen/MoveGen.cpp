#include "MoveGen.h"

void MoveGen::update_available_legal_moves() {
    available_moves.count = 0;

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        if (state->is_enemy(square)) {
            continue;
        }

        update_available_moves_for_square(square);
    }
}

void MoveGen::update_available_moves_for_square(const Square square) {
    update_legal_moves_for_square(square);
    
    while (true) {
        int destination_square_index = count_trailing_zeros(curr_moves);
            
        // Equivalent to destination_square >= 64
        if (is_square_out_of_bounds(destination_square_index)) {
            break;
        }

        set_bit_to_0(curr_moves, destination_square_index);

        Square destination_square = static_cast<Square>(destination_square_index);

        if (is_square_out_of_bounds(square)) {
            continue;
        }
        
        Move move = create_move(square, destination_square);

        if (move.is_promotion()) {
            add_all_promotions_to_available_moves(move);
            continue;
        }

        available_moves.push(move);
    }
}

void MoveGen::add_all_promotions_to_available_moves(const Move& move) {
    PieceType promotion_piece_types[4] = {KNIGHT, BISHOP, ROOK, QUEEN};

    for (PieceType promotion_piece_type : promotion_piece_types) {
        Move promotion_move = move;
        promotion_move.promotion_piece_type = promotion_piece_type;

        available_moves.push(promotion_move);
    }
}

bool MoveGen::can_move_to_square(const Square square) const {
    return is_bit_1(curr_moves, square);
}

uint64_t MoveGen::get_curr_moves() const {
    return curr_moves;
}

void MoveGen::clear_moves() {
    curr_moves = 0ULL;
}

void MoveGen::update_legal_moves_for_square(const Square square) {
    update_pseudo_legal_moves_for_square(square);
    
    MoveLegalizer move_legalizer = MoveLegalizer(state, *this, curr_moves, square);
    move_legalizer.legalize_moves();
}

void MoveGen::update_pseudo_legal_moves_for_square(const Square square) {
    Piece piece = state->get_piece_at(square);

    if (piece == PIECE_NONE) {
        return;
    }

    PieceType piece_type = piece_to_piece_type[piece];

    switch (piece_type) {
        case PAWN:   curr_moves = generate_pawn_moves(square);
                break;
        case KNIGHT: curr_moves = knight_moves_lookup[square];
                break;
        case BISHOP: curr_moves = get_bishop_moves(square, state->get_pieces_bitboard());
                break;
        case ROOK:   curr_moves = get_rook_moves(square, state->get_pieces_bitboard());
                break;
        case QUEEN:  curr_moves = get_queen_moves(square);
                break;
        case KING:   
                curr_moves = king_moves_lookup[square];
                include_castling_if_possible();
                break;
    }

    exclude_friendly_pieces_from_moves();
}

void MoveGen::include_castling_if_possible() {
    Square king_square = state->get_king_square(state->turn);

    if (is_castling_path_empty(CastlingType::SHORT_CASTLING)) {
        set_bit_to_1(curr_moves, king_square + 2);
    }
    if (is_castling_path_empty(CastlingType::LONG_CASTLING)) {
        set_bit_to_1(curr_moves, king_square - 2);
    }
}

bool MoveGen::is_castling_path_empty(const CastlingType castling_type) const {
    Square king_square = state->get_king_square(state->turn);

    for (int castling_offset : castling_offsets[castling_type]) {
        Square castling_square = static_cast<Square>( static_cast<int>(king_square) + castling_offset );

        if (state->is_occupied(castling_square)) {
            return false;
        }
    }

    return true;
}

void MoveGen::exclude_friendly_pieces_from_moves() {
    curr_moves &= ~state->colours_bitboard[state->turn];
}

uint64_t MoveGen::generate_pawn_moves(const Square square) const {
    uint64_t moves = 0ULL;

    Square double_front = static_cast<Square>(square + PawnOffset::DOUBLE_FRONT);
    Square front = static_cast<Square>(square + PawnOffset::FRONT);

    if (!state->is_white_turn()) {
        double_front = static_cast<Square>(square - PawnOffset::DOUBLE_FRONT);
        front = static_cast<Square>(square - PawnOffset::FRONT);
    }

    if (state->is_empty(front)) {
        set_bit_to_1(moves, front);

        if (can_pawn_double_push(square)) {
            set_bit_to_1(moves, double_front);
        }
    }

    moves |= get_pawn_captures(square);

    return moves;
}

bool MoveGen::can_pawn_double_push(const Square square) const {
    Square double_front = static_cast<Square>( state->is_white_turn() ? square + PawnOffset::DOUBLE_FRONT : square - PawnOffset::DOUBLE_FRONT );
    Row starting_pawn_row = state->is_white_turn() ? ROW_2 : ROW_7;

    return state->is_empty(double_front) && square_to_row[square] == starting_pawn_row;
}

uint64_t MoveGen::get_pawn_captures(const Square square) const {
    Colour turn_colour = state->turn;

    uint64_t captures = state->get_enemy_bitboard() & pawn_attacks[turn_colour][square];

    if (state->en_passant != SQ_NONE) {
        captures |= (1ULL << state->en_passant) & pawn_attacks[turn_colour][square];
    }

    return captures;
}

uint64_t MoveGen::get_bishop_moves(const Square square, const uint64_t blockers) const {
    int blockers_index = get_blockers_magic_index(square, BISHOP, blockers);
    return bishop_moves_lookup[square][blockers_index];
}

uint64_t MoveGen::get_rook_moves(const Square square, const uint64_t blockers) const {
    int blockers_index = get_blockers_magic_index(square, ROOK, blockers);
    return rook_moves_lookup[square][blockers_index];
}

uint64_t MoveGen::get_queen_moves(const Square square) const {
    uint64_t blockers = state->get_pieces_bitboard();
    return get_bishop_moves(square, blockers) | get_rook_moves(square, blockers);
}

int MoveGen::get_blockers_magic_index(const Square square, const PieceType piece_type, const uint64_t blockers) const {
    bool is_rook = (piece_type == ROOK);

    uint64_t relevant_blockers = is_rook ?
        MovesMasks::rook_attack_masks[square] & blockers :
        MovesMasks::bishop_attack_masks[square] & blockers;

    uint64_t shift_amount = is_rook ? 
        64 - rook_occupancy_bits_count[square] :
        64 - bishop_occupancy_bits_count[square];
    
    uint64_t magic_number = is_rook ?
        MagicNumbers::rook_magics[square] :
        MagicNumbers::bishop_magics[square];

    return (magic_number * relevant_blockers) >> shift_amount;
}

Move MoveGen::create_move(const Square from, const Square to) const {
    Move move = Move(from, to);
    MoveFlagAssigner::assign_corresponding_flag_to(move, state);
    return move;
}

