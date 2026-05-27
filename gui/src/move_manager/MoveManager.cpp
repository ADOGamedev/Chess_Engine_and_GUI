#include "MoveManager.h"

void MoveManager::ask_what_piece_promote_to() const {
    gui->show_promotion_menu(state->turn);
}

void MoveManager::on_promotion_piece_selected(int piece) {
    gui->hide_promotion_menu();

    pending_promotion_move.promotion_piece_type = piece_to_piece_type[piece];

    timer_manager->add_time_increment();
    do_move_and_update_graphics(pending_promotion_move);
    state->add_move_to_history(pending_promotion_move);

    if (game_started && playing_against_ai) {
        make_ai_move();
    }
}

void MoveManager::do_move_and_update_graphics(const Move& move) {
    curr_move_index++;

    MoveExecutor::do_move(move, state);
    board_renderer->update_graphics();
    board_renderer->hightlight_move(move);

    curr_fen = StateToFenConverter::game_state_to_fen(state);
    gui->update_curr_fen_in_settings_menu();

    move_legalizer.calculate_pins_and_checks();

    GameOverType game_over = GameOverChecker::check_game_over(*state);

    if (game_over != GAME_OVER_NONE) {
        sound_manager->play_game_over_sfx();
        gui->show_game_over_message(game_over);
        game_started = false;
    }
}

void MoveManager::make_ai_move() {
    std::thread([this]() {
        try {
            
        std::string str_moves_history = state->convert_moves_history_to_str();
        std::string uci_command = "position fen " + starting_fen;

        if (!str_moves_history.empty()) {
            uci_command += " moves " + str_moves_history;
        }
        
        uci_communicator->send_command(uci_command);

        int wtime = timer_manager->white_seconds_left * 1000;
        int btime = timer_manager->black_seconds_left * 1000;
        
        int milliseconds_increment = increment_seconds * 1000;

        TimeStruct time_struct(wtime, btime, milliseconds_increment, milliseconds_increment);
        
        std::string best_lan_move = uci_communicator->find_best_move(time_struct);

        Move best_move = lan_notation_to_move(best_lan_move);
        PieceType promotion_piece_type = best_move.promotion_piece_type;
        
        best_move = move_gen->create_move(best_move.from, best_move.to);
        best_move.promotion_piece_type = promotion_piece_type;

        if (state->turn != player_colour) {
            do_move_and_update_graphics(best_move);
            state->add_move_to_history(best_move);
        }

        } catch (const UCIEngineException& e) {
            gui->display_error(e);
            Logger::log(e.what());
        }

    }).detach();

}

void MoveManager::undo_move_and_update_graphics(const Move& move) {
    if (curr_move_index >= 0) {
        curr_move_index--;
    }

    MoveExecutor::undo_move(move, state);
    board_renderer->update_graphics();

    move_legalizer.calculate_pins_and_checks();
}

void MoveManager::go_back_one_move() {
    if (can_go_back_one_move()) {
        Colour prev_turn = state->turn;
        Move& move = state->moves_history[curr_move_index];
        undo_move_and_update_graphics(move);
        state->turn = prev_turn;
        board_renderer->hightlight_move(move);
    }
}

bool MoveManager::can_go_back_one_move() {
    return curr_move_index >= 0;
}

void MoveManager::advance_one_move() {
    if (can_advance_one_move()) {
        Colour prev_turn = state->turn;
        Move& move = state->moves_history[curr_move_index + 1];
        do_move_and_update_graphics(move);
        state->turn = prev_turn;
        board_renderer->hightlight_move(move);
    }
}

bool MoveManager::can_advance_one_move() {
    return curr_move_index < (state->moves_history.size() - 1) || curr_move_index == -1 && state->moves_history.size() > 0;
}

bool MoveManager::not_in_actual_move() {
    return (curr_move_index < (state->moves_history.size() - 1) ||
            curr_move_index == -1 && state->moves_history.size() > 0);
}

void MoveManager::clear_moves_history() {
    curr_move_index = -1;
    state->moves_history.clear();
}

