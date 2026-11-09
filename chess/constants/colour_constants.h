#pragma once

#include <cstdint>

enum Colour : uint8_t {
    WHITE,
    BLACK,

    N_COLOURS,
    COLOUR_NONE
};

constexpr Colour operator!(const Colour& colour) {
    return colour == WHITE ? BLACK : WHITE;
}