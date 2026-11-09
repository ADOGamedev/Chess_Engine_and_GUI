#pragma once

#include <unordered_set>

#include "../../game_state/GameState.h"
#include "../../fen_loader/FenLoader.h"
#include "../../utils/utils.h"
#include "../../zobrist_hasher/ZobristHasher.h"

class ZobristCollisionsTester {

public:
    static void test_zobrist_collisions();

    static GameState get_random_game_state();
private:
    static bool value_already_in_zobrist_keys(uint64_t value);

    static void place_random_kings(GameState& state);
    static void place_random_pieces(const Piece piece, GameState& state);
    static void asign_random_turn(GameState& state);
    static void asign_random_en_passant(GameState& state);

    static const int POSITIONS_TO_SEARCH = 100000;
    static const int MAXIMUN_PIECES_COUNT[N_PIECES];

    static int collisions_count;
    static int positions_searched_count;
    static std::unordered_set<uint64_t> zobrist_keys;
};