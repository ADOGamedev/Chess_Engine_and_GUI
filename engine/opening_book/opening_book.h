#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "../../chess/utils/utils.h"
#include "../../chess/move_gen/Move.h"

struct BookPosition {
    uint64_t key;
    std::vector<std::string> lan_moves;
    std::vector<int> weights;

};

struct BinBookPosition {
    uint64_t key;
    uint16_t move;
    uint16_t weight;
    uint32_t learn;
};

namespace OpeningBook {

std::vector<BookPosition> book_positions = {};

void print_book_positions();
BookPosition get_book_position_from_bin_positions(std::vector<BinBookPosition>& bin_positions);
std::string get_lan_move_from_binary(uint16_t& move);

uint64_t bswap64(uint64_t x) {
    return ((x & 0xFF00000000000000ULL) >> 56) |
           ((x & 0x00FF000000000000ULL) >> 40) |
           ((x & 0x0000FF0000000000ULL) >> 24) |
           ((x & 0x000000FF00000000ULL) >>  8) |
           ((x & 0x00000000FF000000ULL) <<  8) |
           ((x & 0x0000000000FF0000ULL) << 24) |
           ((x & 0x000000000000FF00ULL) << 40) |
           ((x & 0x00000000000000FFULL) << 56);
}

uint16_t bswap16(uint16_t x) {
    return (x >> 8) | (x << 8);
}

void init() {
    std::vector<BinBookPosition> bin_positions;
    BinBookPosition bin_book_pos;
    uint64_t last_key = 0ULL;

    std::ifstream file("D:\\My Projects\\Chess Engine and GUI\\engine\\opening_book\\gm2001.bin", std::ios::binary);

    while (file.read((char*)&bin_book_pos, sizeof(bin_book_pos))) {
        bin_book_pos.key    = bswap64(bin_book_pos.key);
        bin_book_pos.move   = bswap16(bin_book_pos.move);
        bin_book_pos.weight = bswap16(bin_book_pos.weight);

        if (bin_book_pos.key != last_key && last_key != 0ULL) {
            last_key = bin_book_pos.key;
            book_positions.push_back(get_book_position_from_bin_positions(bin_positions));
            bin_positions.clear();
        }
        
        last_key = bin_book_pos.key;
        bin_positions.push_back(bin_book_pos);
    }

    if (!bin_positions.empty()) {
        book_positions.push_back(get_book_position_from_bin_positions(bin_positions));
    }

    file.close();

    print_book_positions();
}

BookPosition get_book_position_from_bin_positions(std::vector<BinBookPosition>& bin_positions) {
    BookPosition new_book_pos;
    new_book_pos.key = bin_positions[0].key;

    for (BinBookPosition& bin_pos : bin_positions) {
        new_book_pos.lan_moves.push_back(get_lan_move_from_binary(bin_pos.move));
        new_book_pos.weights.push_back((int)bin_pos.weight);
    }

    return new_book_pos;
}

std::string get_lan_move_from_binary(uint16_t& move) {
    int from_column = (move >> 6) & 0x7;
    int from_row = (move >> 9) & 0x7;
    int to_column = (move >> 0) & 0x7;
    int to_row = (move >> 3) & 0x7;
    int promotion_piece = (move >> 13) & 0x7;

    std::string lan_move;
    lan_move += from_column + 'a';
    lan_move += from_row + '1';
    lan_move += to_column + 'a';
    lan_move += to_row + '1';

    const char* promotion_chars = "\0nbrq";
    if (promotion_piece) {
        lan_move += promotion_chars[promotion_piece];
    }

    return lan_move;
}

void print_book_positions() {
    std::cout << "{\n";

    for (int i = 0; i < book_positions.size(); i++) {
        BookPosition book_pos = book_positions[i];
        
        std::cout << "{\n";
        std::cout << "Key: " << book_pos.key << "\n";

        std::cout << "Moves: {";
        for (std::string move : book_pos.lan_moves)
            std::cout << move << ",";
        std::cout << "}\n";


        std::cout << "Weights: {";
        for (uint16_t weight : book_pos.weights)
            std::cout << weight << ",";
        std::cout << "}\n";

        std::cout << "},\n";

        std::cout << "}\n";
    }
}   

}