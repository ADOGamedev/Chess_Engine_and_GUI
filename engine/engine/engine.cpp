#include "engine.h"


void Engine::set_up_position(const std::string& fen, const std::string moves) {
    try {
        FenLoader::load_fen(&state, fen);

        ZobristHasher::set_zobrist_key_of_state(state);
        state.add_curr_key_to_repetition_list();

        //MovesHistory::reset();

        if (!moves.empty()) {
            MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves_sequence = string_to_array_of_moves(moves);
            do_moves_sequence(moves_sequence);
        }
    }
    catch (const FenException& e) {
        std::cout << "error while setting up pos: " << e.what() << "\n";
    }       
}

MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> Engine::string_to_array_of_moves(const std::string& moves) const {
    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves_sequence = {};

    std::istringstream stream(moves);
    std::string lan_move;

    while (stream >> lan_move) {
        Move move = lan_notation_to_move(lan_move);
        moves_sequence.push(move);
    }

    return moves_sequence;
}

void Engine::do_moves_sequence(MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES>& sequence) {
    for (Move& move : sequence) {
        PieceType promotion_piece_type = move.promotion_piece_type;
        move = move_gen.create_move(move.from, move.to);
        move.promotion_piece_type = promotion_piece_type;

        MoveExecutor::do_move(move, &state);
    }
}

std::string Engine::go_depth(int depth) {
    nodes_searched = 0;
    max_time = INF;

    Move best_move = search_best_move(depth).best_move;
    return report_best_move(best_move);
}

std::string Engine::go_time_inc(const int& wtime, const int& btime, const int& winc, const int& binc) {
    int ms_left = state.is_white_turn() ? wtime : btime;
    int ms_inc = state.is_white_turn() ? winc : binc;

    int t = (ms_left / 20) + (ms_inc * 0.75);
    int search_time = std::min(1500, std::min(ms_left / 3, t));

    return go_movetime(search_time);
}

std::string Engine::go_movetime(int movetime) {
    nodes_searched = 0;

    max_time = movetime;
    start_time = std::chrono::high_resolution_clock::now();

    SearchResults results = search_best_move(1);
    Move best_move = results.best_move;
    print_info(1, results.best_score);

    for (int depth = 2; !has_search_timed_out(); depth++) {
        SearchResults results = search_best_move(depth);
        Move& candidate = results.best_move;
        int& score = results.best_score;

        print_info(depth, score);
        
        if (!has_search_timed_out()) {
            best_move = candidate;
        }
    }

    return report_best_move(best_move);
}

void Engine::print_info(const int& depth, const int& score) {
    std::cout << "info depth " << depth << " score cp " << score << " nodes " << nodes_searched << "\n";
}

SearchResults Engine::search_best_move(int depth) {
    HashEntry hash_entry = TranspositionTable::get(state.key);

    prepare_move_generation();

    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves = get_available_moves(hash_entry.move, 0);

    if (moves.count <= 0) {
        return {Move(), 0};
    }
    
    Move best_move = moves.array[0];
    int best_score = -INF;

    int alpha = -INF;
    int beta = INF;

    for (const Move& move : moves) {
        nodes_searched++;

        MoveExecutor::do_move(move, &state);
        int score = -negamax(depth - 1, -beta, -alpha, 1);
        MoveExecutor::undo_move(move, &state);
        
        if (score > best_score) {
            best_score = score;
            best_move = move;

            if (score > alpha) {
                alpha = score;
            }
        }

        if (has_search_timed_out()) {
            break;
        }

        if (alpha >= beta) {
            if (!move.is_capture()) {
                KillerMoves::insert_killer(move, 0);
            }
            break;
        }
    }

    return {best_move, best_score};
}

int Engine::negamax(int depth, int alpha, int beta, int ply) {
    prepare_move_generation();

    GameOverType game_over = GameOverChecker::check_game_over(state);
    if (game_over != GAME_OVER_NONE) {
        return get_game_over_score(game_over, ply);
    }

    if (depth > 0 && GameOverChecker::get_repetition_count(&state) > 1) {
        return 0;
    }

    if (depth <= 0) {
        return quiescence(alpha, beta);
    }

    int original_alpha = alpha;

    HashEntry hash_entry = TranspositionTable::get(state.key);
    bool should_read_hash_entry = hash_entry.is_valid() && state.key == hash_entry.zobrist && hash_entry.depth >= depth;
    
    if (should_read_hash_entry) {
        int hash_eval = hash_entry.eval;
        if (hash_eval > CHECKMATE_SCORE - 100)
            hash_eval -= ply;
        if (hash_eval < -CHECKMATE_SCORE + 100)
            hash_eval += ply;

        if (hash_entry.flag == EXACT)
            return hash_eval;

        else if (hash_entry.flag == LOWERBOUND)
            alpha = std::max(alpha, hash_eval);

        else if (hash_entry.flag == UPPERBOUND)
            beta = std::min(beta, hash_eval);
        
        if (alpha >= beta)
            return hash_eval;
    }

    int best_score = -INF;
    Move best_move;

    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves = get_available_moves(hash_entry.move, ply);
    for (const Move& move : moves) {
        nodes_searched++;

        MoveExecutor::do_move(move, &state);
        int score = -negamax(depth - 1, -beta, -alpha, ply + 1);
        MoveExecutor::undo_move(move, &state);
        
        if (score > best_score) {
            best_score = score;
            best_move = move;

            if (score > alpha) {
                alpha = score;
            }
        }
        
        if (has_search_timed_out()) {
            return best_score;
        }

        if (alpha >= beta) {
            if (!move.is_capture()) {
                KillerMoves::insert_killer(move, ply);
            }
            break;
        }
    }

    HashEntryFlag flag = EXACT;
    if (best_score <= original_alpha) {
        flag = UPPERBOUND;
    }
    else if (best_score >= beta) {
        flag = LOWERBOUND;
    }

    int score_to_store = best_score;
    if (best_score > CHECKMATE_SCORE - 100) 
        score_to_store += ply;
    if (best_score < -CHECKMATE_SCORE + 100) 
        score_to_store -= ply;

    bool should_add_new_entry = !hash_entry.is_valid() || depth > hash_entry.depth;
    if (should_add_new_entry) {
        HashEntry new_entry = HashEntry(
            state.key, depth, flag, score_to_store, 0, best_move
        );
        TranspositionTable::add(new_entry);
    }

    return best_score;
}

int Engine::get_game_over_score(GameOverType game_over, int ply) const {
    bool is_checkmate = (game_over == CHECKMATE_TO_BLACK || game_over == CHECKMATE_TO_WHITE);
    return is_checkmate ? (-CHECKMATE_SCORE + ply) : 0;
}

int Engine::quiescence(int alpha, int beta) {
    prepare_move_generation();

    int eval = evaluate();
    if (eval >= beta) {
        return beta;
    }
    alpha = std::max(alpha, eval);

    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> captures = get_available_captures();

    for (Move move : captures) {
        MoveExecutor::do_move(move, &state);
        int score = -quiescence(-beta, -alpha);
        MoveExecutor::undo_move(move, &state);

        if (score >= beta) {
            return beta;
        }
        alpha = std::max(alpha, score);
    }

    return alpha;
}

int Engine::evaluate() {
    int mg[N_COLOURS] = {0, 0};
    int eg[N_COLOURS] = {0, 0};

    for (Square square = SQ_0; square < N_SQUARES; square++) {
        Piece piece = state.get_piece_at(square);
        if (piece == PIECE_NONE) {
            continue;
        }

        PieceType piece_type = piece_to_piece_type[piece];
        Colour piece_colour = piece_to_color[piece];

        Square sq = square;
        if (piece_colour == WHITE) {
            sq = get_flipped_square(square);
        }

        mg[piece_colour] += MIDDLE_GAME_PIECES_SCORES[piece_type] + PositionalScores::MIDDLE_GAME[piece_type][sq];
        eg[piece_colour] += END_GAME_PIECES_SCORES[piece_type] + PositionalScores::END_GAME[piece_type][sq];
    }

    int mg_score = mg[state.turn] - mg[!state.turn];
    int eg_score = eg[state.turn] - eg[!state.turn];
    
    int mg_phase = state.game_phase;
    int eg_phase = MAX_GAME_PHASE - state.game_phase;

    return (mg_score * mg_phase + eg_score * eg_phase) / MAX_GAME_PHASE;
}


void Engine::prepare_move_generation() {
    move_legalizer.calculate_pins_and_checks();
    move_gen.update_available_legal_moves();
}

std::string Engine::report_best_move(const Move& move) const {
    std::string lan = move_to_lan_notation(move);
    std::cout << "bestmove " << lan << "\n";
    return lan;
}

bool Engine::has_search_timed_out() const {
    time_point now = std::chrono::high_resolution_clock::now();
    int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count();

    return elapsed >= max_time;
}

MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> Engine::get_available_moves(const Move& hash_move, const int& ply) {
    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves;
    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> captures;
    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> quiets;
    MovesArray<KILLERS_PER_PLY> killers = KillerMoves::get_killers(ply);

    Move legal_hash_move;
    MovesArray<KILLERS_PER_PLY> legal_killer_moves;

    for (int i = 0; i < move_gen.available_moves.count; i++) {
        const Move& move = move_gen.available_moves.array[i];

        if (hash_move.is_valid() && move == hash_move) {
            legal_hash_move = move;
            continue;
        }

        if (move == killers.array[0]) {
            legal_killer_moves.array[0] = move;
            continue;
        }
        if (move == killers.array[1]) {
            legal_killer_moves.array[1] = move;
            continue;
        }

        if (move.is_capture()){
            captures.push(move);
            continue;
        }

        quiets.push(move);
    }
    if (legal_hash_move.is_valid()) {
        moves.push(legal_hash_move);
    }

    sort_captures(captures);
    moves.insert_back(captures.begin(), captures.end());
    insert_killers_if_possible(moves, legal_killer_moves);
    moves.insert_back(quiets.begin(), quiets.end());

    return moves;
}

void Engine::insert_killers_if_possible(MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES>& moves, MovesArray<KILLERS_PER_PLY>& killers) {
    if (killers.array[0].is_valid()) {
        moves.push(killers.array[0]);
    }
    if (killers.array[1].is_valid()) {
        moves.push(killers.array[1]);
    }
}

MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> Engine::get_available_captures() const {
    MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES> moves;

    for (int i = 0; i < move_gen.available_moves.count; i++) {
        if (move_gen.available_moves.array[i].is_capture()) {
            moves.push(move_gen.available_moves.array[i]);
        }
    }

    sort_captures(moves);

    return moves;
}

void Engine::sort_captures(MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES>& captures) const {
    std::sort(captures.begin(), captures.end(), [](const Move& a, const Move& b) {
        PieceType victim_a = piece_to_piece_type[a.captured_piece];
        PieceType attacker_a = piece_to_piece_type[a.moved_piece];
        PieceType victim_b = piece_to_piece_type[b.captured_piece];
        PieceType attacker_b = piece_to_piece_type[b.moved_piece];

        int score_a = MVV_LVA[victim_a][attacker_a];
        int score_b = MVV_LVA[victim_b][attacker_b];
        return score_a > score_b;
    });
}

void Engine::sort_quiets(MovesArray<MAX_POSSIBLE_AVAILABLE_MOVES>& quiets) const {
      std::sort(quiets.begin(), quiets.end(), [this](const Move& a, const Move& b) {
        int score_a = MovesHistory::get_score(state.turn, a);
        int score_b = MovesHistory::get_score(state.turn, b);

        return score_a > score_b;
    });  
}