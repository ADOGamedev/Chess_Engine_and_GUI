#include "GameOverChecker.h"

GameOverType GameOverChecker::check_game_over(GameState& state) {
    GameOverChecker game_over_checker = GameOverChecker(&state);
    bool is_king_in_check = game_over_checker.move_legalizer.is_king_in_check(state.turn);
    
    if (!game_over_checker.is_there_any_move_availale()) {
        if (is_king_in_check) {
            return state.is_white_turn() ? CHECKMATE_TO_WHITE : CHECKMATE_TO_BLACK;
        }
        
        return STALEMATE;
    }

    else if (game_over_checker.is_three_fold_repetition()){
        return THREE_FOLD_REPETITION;
    }
    
    else if (game_over_checker.is_fifty_moves_rule()) {
        return FIFTY_MOVES_RULE;
    }
    
    else if (!game_over_checker.has_enough_material()) {
        return INSUFFICIENT_MATERIAL;
    }
    
    return GAME_OVER_NONE;
}

bool GameOverChecker::is_there_any_move_availale() {
    move_gen.update_available_legal_moves();
    return move_gen.available_moves_count > 0;
}

bool GameOverChecker::is_three_fold_repetition() {
    return get_repetition_count(state) >= 3;
}

int GameOverChecker::get_repetition_count(GameState* state) {
    int repetitions_count = 0;

    for (uint64_t key : state->repetition_list) {
        if (key == state->key) {
            repetitions_count++;
        }
    }
    return repetitions_count;
}

bool GameOverChecker::is_fifty_moves_rule() {
    // "Fifty moves rule" refers to 50 full moves or 100 half moves
    return state->fifty_moves_rule_counter >= 100;
}

bool GameOverChecker::has_enough_material() {
    if (more_than_two_pieces_of_the_same_colour() ||
        pawns_queens_or_rooks_in_board()) {
            return true;
    }

    if (exactly_one_bishop_in_each_side()) {
        return !are_bishops_on_same_square_colour();
    }

    if (more_than_three_pieces_on_the_board()) {
        return true;
    }

    return false;
}

bool GameOverChecker::more_than_two_pieces_of_the_same_colour() {
    int count_of_white_pieces = count_bits_in(state->get_pieces_bitboard(WHITE));
    int count_of_black_pieces = count_bits_in(state->get_pieces_bitboard(BLACK));

    return count_of_white_pieces > 2 || count_of_black_pieces > 2;
}

bool GameOverChecker::pawns_queens_or_rooks_in_board() {
    bool are_there_pawns = state->get_pieces_bitboard(PAWN);
    bool are_there_rooks = state->get_pieces_bitboard(ROOK);
    bool are_there_queens = state->get_pieces_bitboard(QUEEN);

    return are_there_pawns || are_there_rooks || are_there_queens;
}

bool GameOverChecker::exactly_one_bishop_in_each_side() {
    int white_bishops_count = count_bits_in(state->get_pieces_bitboard(WHITE_BISHOP));
    int black_bishops_count = count_bits_in(state->get_pieces_bitboard(BLACK_BISHOP));

    return (white_bishops_count == 1) && (black_bishops_count == 1);
}

bool GameOverChecker::are_bishops_on_same_square_colour() {
    uint64_t white_bishops = state->get_pieces_bitboard(WHITE_BISHOP);
    uint64_t black_bishops = state->get_pieces_bitboard(BLACK_BISHOP);

    Square white_bishop_square = static_cast<Square>(count_trailing_zeros(white_bishops));
    Square black_bishop_square = static_cast<Square>(count_trailing_zeros(black_bishops));
    
    return get_square_colour(white_bishop_square) == get_square_colour(black_bishop_square);
}

bool GameOverChecker::more_than_three_pieces_on_the_board() {
    int count_of_white_pieces = count_bits_in(state->get_pieces_bitboard(WHITE));
    int count_of_black_pieces = count_bits_in(state->get_pieces_bitboard(BLACK));

    return (count_of_white_pieces + count_of_black_pieces) > 3;
}

Colour GameOverChecker::get_square_colour(const Square square) const {
    Row row = square_to_row[square];
    Column column = square_to_column[square];

    bool is_square_white = (static_cast<int>(row) + static_cast<int>(column)) % 2;

    return static_cast<Colour>(is_square_white);
}