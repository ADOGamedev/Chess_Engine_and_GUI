#pragma once

#include <string>

#include "../game_state/GameState.h"

class StateToFenConverter {
public:
    static std::string game_state_to_fen(GameState* state);

private:
    static std::string get_board_fen_string(GameState* state);
    static std::string get_turn_fen_string(GameState* state);
    static std::string get_castling_rights_fen_string(GameState* state);
    static std::string get_en_passant_fen_string(GameState* state);
    static std::string get_fifty_moves_rule_counter_fen_string(GameState* state);
};