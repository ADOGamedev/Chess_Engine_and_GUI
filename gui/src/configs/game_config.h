#pragma once

#include <string>

inline std::string uci_engine_path = "";

inline std::string starting_fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"; 
inline std::string curr_fen = "";


inline double initial_seconds_left = 600;
inline int increment_seconds = 0;

inline bool playing_against_ai = false;
inline bool player_colour = WHITE;

inline bool game_started = false;