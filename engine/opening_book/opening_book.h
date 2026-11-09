#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <climits>

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


bool is_position_in_book(const uint64_t& key);
std::string get_random_move(const uint64_t& key);
int binary_search(const uint64_t& key, const int begin, const int end);
uint16_t bswap16(uint16_t x);
void init();
BookPosition get_book_position_from_bin_positions(std::vector<BinBookPosition>& bin_positions);
std::string get_lan_move_from_binary(uint16_t& move);
void print_book_positions();


}