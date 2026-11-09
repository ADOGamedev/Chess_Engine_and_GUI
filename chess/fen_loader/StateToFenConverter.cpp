#include "StateToFenConverter.h"

std::string StateToFenConverter::game_state_to_fen(GameState* state) {
    std::string game_state_fen = "";

    game_state_fen += get_board_fen_string(state) + " ";
    game_state_fen += get_turn_fen_string(state) + " ";
    game_state_fen += get_castling_rights_fen_string(state) + " ";
    game_state_fen += get_en_passant_fen_string(state) + " ";
    game_state_fen += get_fifty_moves_rule_counter_fen_string(state);

    return game_state_fen;
}

std::string StateToFenConverter::get_board_fen_string(GameState* state) {
    std::string board_fen = "";
    int empty_squares_counter = 0;

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Square fen_square = get_flipped_square(square);
        Piece piece = state->get_piece_at(fen_square);

        bool is_in_new_row = square != SQ_0 && fen_square % 8 == 0;

        if (is_in_new_row) {
            if (empty_squares_counter != 0) {
                board_fen += std::to_string(empty_squares_counter);
            }
            board_fen += "/";
            empty_squares_counter = 0;
        }

        if (piece == PIECE_NONE) {
            empty_squares_counter++;
            continue;
        }
        
        if (empty_squares_counter != 0) {
            board_fen += std::to_string(empty_squares_counter);
        }

        board_fen += piece_to_char[piece];
        empty_squares_counter = 0;
    }

    if (empty_squares_counter != 0) {
        board_fen += std::to_string(empty_squares_counter);
    }

    return board_fen;
}

std::string StateToFenConverter::get_turn_fen_string(GameState* state) {
    return state->is_white_turn() ? "w" : "b";
}

std::string StateToFenConverter::get_castling_rights_fen_string(GameState* state) {
    std::string castling_rights_str = "";

    for (Castling castling = WHITE_SHORT_CASTLING; castling < N_CASTLINGS; castling++) {
        if (state->has_castling_right(castling)) {
            castling_rights_str += castling_to_char[castling];
        }
    }

    if (castling_rights_str == "") {
        castling_rights_str = "-";
    }

    return castling_rights_str;
}

std::string StateToFenConverter::get_en_passant_fen_string(GameState* state) {
    std::string en_passant_square_str = "";

    if (state->en_passant == SQ_NONE) {
        en_passant_square_str = "-"; 
    }
    else {
        en_passant_square_str = square_to_chess_coords(state->en_passant);
    }

    return en_passant_square_str;
}

std::string StateToFenConverter::get_fifty_moves_rule_counter_fen_string(GameState* state) {
    return std::to_string(state->fifty_moves_rule_counter);
}


