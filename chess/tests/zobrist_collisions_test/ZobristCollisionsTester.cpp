#include "ZobristCollisionsTester.h"

int ZobristCollisionsTester::collisions_count = 0;
int ZobristCollisionsTester::positions_searched_count = 0;
std::unordered_set<uint64_t> ZobristCollisionsTester::zobrist_keys = {};

const int ZobristCollisionsTester::MAXIMUN_PIECES_COUNT[N_PIECES] = {8, 2, 2, 2, 1, 1, 8, 2, 2, 2, 1, 1};

void ZobristCollisionsTester::test_zobrist_collisions() {
    ZobristHasher::initialize_zobrist_values();

    for (int i = 0; i < POSITIONS_TO_SEARCH; i++) {
        GameState state = get_random_game_state();
        ZobristHasher::set_zobrist_key_of_state(state);

        if (value_already_in_zobrist_keys(state.key)) {
            collisions_count++;
        }
        Logger::log("Between " + std::to_string(positions_searched_count) + " positions there were " +
            std::to_string(collisions_count) + " collisions.");

        zobrist_keys.insert(state.key);
        positions_searched_count++;
    }

    Logger::log("Test finished! Results: ");
    Logger::log("Between " + std::to_string(positions_searched_count) + " positions there were " +
                std::to_string(collisions_count) + " collisions.");
    Logger::log("Error rate = " + std::to_string(100 * collisions_count/positions_searched_count) + "%");
}

bool ZobristCollisionsTester::value_already_in_zobrist_keys(uint64_t key) {
    return zobrist_keys.count(key) > 0;
}

GameState ZobristCollisionsTester::get_random_game_state() {
    GameState state = GameState();

    place_random_kings(state);

    for (Piece piece = WHITE_PAWN; piece < N_PIECES; piece++) {
        place_random_pieces(piece, state);
    }

    asign_random_turn(state);
    asign_random_en_passant(state);

    return state;
}

void ZobristCollisionsTester::place_random_kings(GameState& state) {
    Square white_king_square = static_cast<Square>(randi_range(SQ_0, SQ_63));
    Square black_king_square = static_cast<Square>(randi_range(SQ_0, SQ_63));

    while (black_king_square == white_king_square) {
        black_king_square = static_cast<Square>(randi_range(SQ_0, SQ_63));
    }

    state.place_piece_at(white_king_square, WHITE_KING);
    state.place_piece_at(black_king_square, BLACK_KING);
}

void ZobristCollisionsTester::place_random_pieces(const Piece piece, GameState& state) {
    int amount = randi_range(0, MAXIMUN_PIECES_COUNT[piece]);

    for (int i = 0; i < amount; i++) {
        Square random_square = static_cast<Square>(randi_range(SQ_0, SQ_63));
        while (state.is_occupied(random_square)) {
            random_square = static_cast<Square>(randi_range(SQ_0, SQ_63));
        }

        state.place_piece_at(random_square, piece);
    }
}

void ZobristCollisionsTester::asign_random_turn(GameState& state) {
    state.turn = randf() > 0.5 ? WHITE : BLACK;
}

void ZobristCollisionsTester::asign_random_en_passant(GameState& state) {
    if (randf() > 0.5) {
        state.en_passant = static_cast<Square>(randi_range(SQ_48, SQ_55));
    }
    else {
        state.en_passant = static_cast<Square>(randi_range(SQ_8, SQ_15));
    }
}