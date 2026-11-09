#pragma once

#include <cstdint>
#include <string>

#include "colour_constants.h"

enum GameOverType : int8_t {
    CHECKMATE_TO_WHITE,
    CHECKMATE_TO_BLACK,

    WHITE_RESIGNATION,
    BLACK_RESIGNATION,

    TIMEOUT_FOR_WHITE,
    TIMEOUT_FOR_BLACK,

    STALEMATE,

    INSUFFICIENT_MATERIAL,

    FIFTY_MOVES_RULE,
    THREE_FOLD_REPETITION,
    DRAW_AGREEMENT,

    N_TYPES_OF_GAME_OVER,
    GAME_OVER_NONE
};

const std::string game_over_messages[N_TYPES_OF_GAME_OVER] = {
    "Black won! White got checkmated",
    "White won! Black got checkmated",
    "Black won! White resigned",
    "White won! Black resigned",
    "Black won! White ran out of time",
    "White won! Black ran out of time",
    "Draw! By stalemate",
    "Draw! By insufficient material",
    "Draw! By fifty moves rule",
    "Draw! By three fold repetition",
    "Draw! By agreement"
};

constexpr Colour game_over_type_to_winner_colour[N_TYPES_OF_GAME_OVER] = {
    BLACK, WHITE, BLACK, WHITE, BLACK, WHITE, COLOUR_NONE, COLOUR_NONE, COLOUR_NONE, COLOUR_NONE, COLOUR_NONE
};