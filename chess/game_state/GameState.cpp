#include "GameState.h"

GameState::GameState() {
    board.fill(0ULL);
    colours_bitboard.fill(0ULL);
    board_squares.fill(PIECE_NONE);
    castling_rights.fill(false);

    pins.fill(UINT64_MAX);
}

void GameState::switch_turns() { 
    turn = !turn;
}

bool GameState::is_white_turn() const{
    return turn == WHITE;
}

Piece GameState::get_piece_at(const Square square) const {
    return board_squares[square];
}

void GameState::place_piece_at(const Square square, const Piece piece) {
    if (piece == PIECE_NONE || is_square_out_of_bounds(square)) {
        return;
    }

    PieceType piece_type = piece_to_piece_type[piece];
    game_phase += GAME_PHASE_PIECE_VALUES[piece_type];
    game_phase = std::min(game_phase, MAX_GAME_PHASE);

    set_bit_to_1(board[piece], square);
    set_bit_to_1(colours_bitboard[piece_to_color[piece]], square);
    board_squares[square] = piece;
}

void GameState::remove_piece_from(const Square square) {
    if (is_square_out_of_bounds(square)) {
        return;
    }

    Piece piece = get_piece_at(square);
    if (piece == PIECE_NONE) { 
        return;
    }   

    PieceType piece_type = piece_to_piece_type[piece];
    game_phase -= GAME_PHASE_PIECE_VALUES[piece_type];
    game_phase = std::min(game_phase, MAX_GAME_PHASE);
    
    set_bit_to_0(board[piece], square);
    set_bit_to_0(colours_bitboard[piece_to_color[piece]], square);
    board_squares[square] = PIECE_NONE;
}

void GameState::give_castling_right(const Castling castling) {
    castling_rights[castling] = true;
}

void GameState::remove_castling_right(const Castling castling) {
    castling_rights[castling] = false;
}

bool GameState::has_castling_right(const Castling castling) {
    return castling_rights[castling];
}

void GameState::add_curr_key_to_repetition_list() {
    repetition_list.push_back(key);
}   

void GameState::remove_last_key_from_repetition_list() {
    if (repetition_list.size() > 0) {
        repetition_list.pop_back(); 
    }
}

void GameState::add_move_to_history(const Move& move) {
    moves_history.push_back(move);
}

std::string GameState::convert_moves_history_to_str() {
    std::string str = "";

    for (Move& move : moves_history) {
        str += move_to_lan_notation(move) + " ";
    }

    return str;
}

Square GameState::get_king_square(const Colour colour) const {
    Square white_king_square = static_cast<Square>(count_trailing_zeros(board[WHITE_KING]));
    Square black_king_square = static_cast<Square>(count_trailing_zeros(board[BLACK_KING]));

    return colour == WHITE ? white_king_square : black_king_square;
}
