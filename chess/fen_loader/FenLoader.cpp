#include "FenLoader.h"

GameState* FenLoader::state;
std::string FenLoader::fen;
std::array<std::string, FenElement::N_FEN_ELEMENTS> FenLoader::fen_elements;

void FenLoader::load_fen(GameState* game_state, std::string fen_str) {
    state = game_state;
    fen = fen_str;

    state->board.fill(0ULL);
    state->colours_bitboard.fill(0ULL);
    state->board_squares.fill(PIECE_NONE);
    state->castling_rights.fill(false);
    state->en_passant = SQ_NONE;
    state->en_passant_capture = SQ_NONE;
    state->game_phase = 0;
    state->repetition_list.clear();

    fen_elements = get_split_fen();

    load_board();
    load_turn();
    load_castling_rights();
    load_en_passant();
    load_fifty_moves_rule_counter();

    MoveGen move_gen = MoveGen(state);
    MoveLegalizer move_legalizer = MoveLegalizer(state, move_gen);
    move_legalizer.calculate_pins_and_checks();
}

std::array<std::string, FenElement::N_FEN_ELEMENTS> FenLoader::get_split_fen() {
    std::array<std::string, FenElement::N_FEN_ELEMENTS> elements;

    std::stringstream str_stream(fen);
    std::string element;

    for (int i = 0; str_stream >> element && i < FenElement::N_FEN_ELEMENTS; i++) {
        elements[i] = element;
    }

    return elements;
}

void FenLoader::load_board() {
    Square fen_square = SQ_0;
    int squares_in_row = 0;
    std::array<bool, N_COLOURS> kings_in_board = {false, false};

    for (char ch : fen_elements[FenElement::BOARD]) {

        if (isdigit(ch)) {
            fen_square += char_to_int(ch);
            squares_in_row += char_to_int(ch);
            continue;
        }

        if (ch == '/') {
            if (squares_in_row != 8) {
                throw InvalidElementsInFenRowException();
            }
            squares_in_row = 0;
            continue;
        }   
        
        Piece piece = char_to_piece(ch);
        Square square = get_flipped_square(fen_square);

        state->place_piece_at(square, piece);

        if (piece == WHITE_KING) {
            kings_in_board[WHITE] = true;
        }
        if (piece == BLACK_KING) {
            kings_in_board[BLACK] = true;
        }

        fen_square++;
        squares_in_row++;
    }

    if (squares_in_row != 8) {
        throw InvalidElementsInFenRowException();
    }

    if (fen_square != 64) {
        throw InvalidElementsInFenBoardException();
    }
    
    if (!kings_in_board[WHITE] || !kings_in_board[BLACK]) {
        throw BothKingsNotFoundInFenException();
    }
}

void FenLoader::load_turn() {
    std::string fen_turn = fen_elements[FenElement::TURN];

    if (fen_turn == "w") {
        state->turn = WHITE;
    }
    else if (fen_turn == "b") {
        state->turn = BLACK;
    }
    else {
        throw InvalidFenTurnException();
    }
}

void FenLoader::load_castling_rights() {
    std::string fen_castling_rights = fen_elements[FenElement::CASTLING_RIGHTS];

    if (fen_castling_rights == "-") {
        return;
    }

    for (char ch : fen_castling_rights) {
        Castling castling = char_to_castling(ch);
        state->give_castling_right(castling);
    }
}

void FenLoader::load_en_passant() {
    std::string fen_en_passant = fen_elements[FenElement::EN_PASSANT];

    if (fen_en_passant == "-") {
        return;
    }

    state->en_passant = fen_en_passant_to_square(fen_en_passant);
    int en_passant_capture_int = state->is_white_turn() ?
        state->en_passant - PawnOffset::FRONT :
        state->en_passant + PawnOffset::FRONT;

    state->en_passant_capture = static_cast<Square>(en_passant_capture_int);
}

void FenLoader::load_fifty_moves_rule_counter() {
    std::string fen_fifty_moves_rule_counter = fen_elements[FenElement::FIFTY_MOVES_RULE_COUNTER];
    
    state->fifty_moves_rule_counter = string_to_int(fen_fifty_moves_rule_counter);
}

Piece FenLoader::char_to_piece(const char ch) {
    switch (ch) {
        case 'P': return WHITE_PAWN;
        case 'R': return WHITE_ROOK;
        case 'N': return WHITE_KNIGHT;
        case 'B': return WHITE_BISHOP;
        case 'Q': return WHITE_QUEEN;
        case 'K': return WHITE_KING;
        case 'p': return BLACK_PAWN;
        case 'r': return BLACK_ROOK;
        case 'n': return BLACK_KNIGHT;
        case 'b': return BLACK_BISHOP;
        case 'q': return BLACK_QUEEN;
        case 'k': return BLACK_KING;
        default: throw InvalidFenPieceCharException();
    }
}

Castling FenLoader::char_to_castling(const char ch) {
    switch (ch) {
        case 'K': return WHITE_SHORT_CASTLING;
        case 'Q': return WHITE_LONG_CASTLING;
        case 'k': return BLACK_SHORT_CASTLING;
        case 'q': return BLACK_LONG_CASTLING;
        default: throw InvalidFenCastlingTypeException();
    }
}


Square FenLoader::fen_en_passant_to_square(const std::string fen_en_passant) {
    if (fen_en_passant.length() != 2) {
        throw InvalidFenEnPassantException();
    }
    
    int column = fen_en_passant[0] - 'a';
    int row = char_to_int(fen_en_passant[1]) - 1;

    if (is_valid_en_passant_position(column, row)) {
        throw InvalidFenEnPassantException();
    }

    return static_cast<Square>( row * 8 + column );
}

bool FenLoader::is_valid_en_passant_position( const int column, const int row) {
    return !(row == 2 || row == 5) || column < 0 || column > 8;
}
