#pragma once

#include <array>

constexpr std::array<int, 4> BISHOP_DIRECTIONS = {7, 9, -7, -9};
constexpr std::array<int, 4> ROOK_DIRECTIONS   = {1, 8, -1, -8};
constexpr std::array<int, 8> QUEEN_DIRECTIONS  = {1, 7, 8, 9, -1, -7, -8, -9};