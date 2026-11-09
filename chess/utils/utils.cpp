#include "utils.h"

void set_bit_to_1(uint64_t& num, const int bit) {
    num |= 1ULL << bit;
}

void set_bit_to_0(uint64_t& num, const int bit) {
    num &= ~(1ULL << bit);
}

bool is_bit_1(const uint64_t num, const int bit) {
    return num & (1ULL << bit);
}

bool is_bit_0(const uint64_t num, const int bit) {
    return ~num & (1ULL << bit);
}

bool is_white(const Piece& piece) {
    return piece_to_color[piece] == WHITE;
}

int char_to_int(const char ch) {
    return ch - '0';
}

int string_to_int(const std::string str) {
    return std::stoi(str);
}

bool is_square_out_of_bounds(const Square& square) {
    return square & ~63;
}

bool is_square_out_of_bounds(const int& square) {
    return square & ~63;
}

std::string move_to_lan_notation(const Move& move) {
    std::string from_square_str = square_to_chess_coords(move.from);
    std::string to_square_str = square_to_chess_coords(move.to);

    std::string move_in_lan_notation = from_square_str + to_square_str;

    if (move.is_promotion()) {
        char promotion_piece_char = piece_type_to_char[move.promotion_piece_type];
        move_in_lan_notation += promotion_piece_char;
    }

    return move_in_lan_notation;
}

Move lan_notation_to_move(const std::string& lan_str) {
    Square from_square = chess_coords_to_square(lan_str.substr(0, 2));
    Square to_square = chess_coords_to_square(lan_str.substr(2, 2));

    Move move = Move(from_square, to_square);

    bool is_promotion = lan_str.length() > 4;
    
    if (is_promotion) {
        move.flag = MoveFlag::PROMOTION;

        char promotion_piece_char = lan_str[4];
        switch (promotion_piece_char) {
            case 'n': move.promotion_piece_type = KNIGHT;
                break;
            case 'b': move.promotion_piece_type = BISHOP;
                break;
            case 'r': move.promotion_piece_type = ROOK;
                break;
            case 'q': move.promotion_piece_type = QUEEN;
                break;
        }
    }
    
    return move;
}

std::string square_to_chess_coords(const Square square) {
    int square_int = static_cast<int>(square);

    char column = 'a' + square_int % 8;
    char row = '1' + floor(square_int / 8); 

    return {column, row};
}

Square chess_coords_to_square(const std::string chess_coords) {
    int column = chess_coords[0] - 'a'; 
    int row = chess_coords[1] - '1';

    return static_cast<Square>(row * 8 + column);
}

Square get_flipped_square(const Square square) {
    return static_cast<Square>(static_cast<int>(square) ^ 56);
}

std::string board_to_string(const GameState& state) {
    std::string board = "";
    std::string row = "";

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Square flipped_square = get_flipped_square(square);
        Piece piece = state.get_piece_at(flipped_square);
        
        if (piece == PIECE_NONE) {
            row += "_ ";
        }
        else {
            row += std::string(1, piece_to_char[piece]) + " ";
        }

        if (static_cast<int>(flipped_square) % 8 == 7) {
            board += row + "\n";
            row = "";
        }
    }

    return board;
}


void print_bitboard(const uint64_t& bb) {
    std::string row = "";

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Square flipped_square = static_cast<Square>(static_cast<int>(square_to_column[square]) + 8 * static_cast<int>(7 - square_to_row[square]));
        
        if (is_bit_1(bb, flipped_square)) {
            row += "1 ";
        }
        else {
            row += "0 ";
        }

        if (static_cast<int>(flipped_square) % 8 == 7) {
            Logger::log(row);
            row = "";
        }
    }
}

uint64_t rand_uint64(uint64_t seed) {
    std::mt19937_64 gen(seed);

    std::uniform_int_distribution<uint64_t> distribution(0, UINT64_MAX);

    return distribution(gen);
}

double randf() {
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_real_distribution<> distribution(0.0, 1.0);

    return distribution(gen);
}

int randi_range(int min, int max) {
    std::random_device random_device;
    std::mt19937 gen(random_device());

    std::uniform_int_distribution<> distribution(min, max);
    
    return distribution(gen);
}
