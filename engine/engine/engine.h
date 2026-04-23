#pragma once

#include <iostream>
#include <chrono>

#include "../../chess/fen_loader/FenLoader.h"
#include "../../chess/fen_loader/StateToFenConverter.h"
#include "../../chess/game_over_checker/GameOverChecker.h"
#include "../../chess/game_state/GameState.h"
#include "../../chess/move_executor/MoveExecutor.h"
#include "../../chess/move_gen/MoveGen.h"
#include "../../chess/move_gen/MoveLegalizer.h"
#include "../../chess/utils/utils.h"
#include "../../chess/constants/constants.h"

#include "../constants/constants.h"
#include "../transposition_table/transposition_table.h"
#include "../killer_moves/killer_moves.h"
#include "../moves_history/moves_history.h"

//typedef std::chrono::high_resolution_clock::time_point time_point;

struct MoveList {
    std::array<Move, MAX_POSSIBLE_AVAILABLE_MOVES> moves;
    int count = 0;
};

struct SearchResults {
    Move best_move;
    int best_score;
};


class Engine  {

public:
    Engine() {
        ZobristHasher::initialize_zobrist_values();
    }
    
    void set_up_position(const std::string& fen, const std::string moves = "");

    std::vector<Move> string_to_vector_of_moves(const std::string& moves) const;
    void do_moves_sequence(std::vector<Move>& sequence);

    std::string go_depth(int depth);
    std::string go_time_inc(const int& wtime, const int& btime, const int& winc, const int& binc);
    std::string go_movetime(int movetime);

    void print_info(const int& depth, const int& score);
    
    int negamax(int depth, int alpha, int beta, int ply);
    
private:
    SearchResults search_best_move(int depth);
    void prepare_move_generation();
    std::string report_best_move(const Move& move) const;
    int get_game_over_score(const GameOverType game_over, const int depth) const;

public:
    int quiescence(int alpha, int beta);
    int evaluate();

	bool has_search_timed_out() const;

	MoveList get_vector_of_available_moves(const Move& hash_move, const int& ply);
    void insert_killers_if_possible(MoveList& moves, std::array<Move, KILLERS_PER_PLY>& killers);
    MoveList get_vector_of_available_captures() const;

    void sort_captures(std::vector<Move>& captures) const;
    void sort_captures_array(std::array<Move, MAX_POSSIBLE_AVAILABLE_MOVES>& captures, int count) const;
    void sort_quiets(std::vector<Move>& quiets) const;
    void sort_quiets_array(std::array<Move, MAX_POSSIBLE_AVAILABLE_MOVES>& quiets, int count) const;
    
    GameState state = GameState();
    
private:
    MoveGen move_gen = MoveGen(&state);
    MoveLegalizer move_legalizer = MoveLegalizer(&state, move_gen);

    int nodes_searched = 0;

	std::chrono::high_resolution_clock::time_point start_time = std::chrono::high_resolution_clock::now();
    int max_time = INT_MAX;
};