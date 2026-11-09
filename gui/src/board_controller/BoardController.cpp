#include "BoardController.h"

using namespace godot;

void BoardController::handle_input(const Ref<InputEventMouseButton> &event) {
    Ref<InputEventMouseButton> mouse_event = event;
    
    if (!mouse_event.is_valid()) {
        return;
    }

    if (!game_started) {
        return;
    }
    
    Vector2 mouse_position = event->get_position();
    bool left_button_clicked = is_left_click(event);
    
    if (left_button_clicked) {
        board_renderer->clear_arrows_and_red_selections();
        gui->hide_error_menu();
    }

    if (is_out_of_board_bounds(mouse_position)) {
        return;
    }
    
    if (is_right_button(mouse_event)) {
        handle_right_click(mouse_event);
        return;
    }

    if (!left_button_clicked) {
        return;
    }
    
    if (is_ai_turn() || move_manager->not_in_actual_move()) {
        return;
    }
    
    Square clicked_square = screen_coords_to_board_square(mouse_position);
    
    if (board_renderer->board_flipped) {
        clicked_square = board_renderer->get_flipped_square(clicked_square);
    }
    
    handle_click_on_square(clicked_square);
}

bool BoardController::is_ai_turn() {
    return playing_against_ai && (state->turn != player_colour);
}

void BoardController::handle_right_click(const Ref<InputEventMouseButton> &mouse_event) {
    Vector2 mouse_position = mouse_event->get_position();
    Square clicked_square = screen_coords_to_board_square(mouse_position);

    if (mouse_event->is_pressed()) {
        right_clicked_square = clicked_square;
        return;
    }

    if (right_clicked_square == clicked_square) {
        board_renderer->toggle_red_selection(right_clicked_square);
    }
    else {
        board_renderer->draw_or_erase_arrow(right_clicked_square, clicked_square);
    }

    right_clicked_square = SQ_NONE;
}

void BoardController::handle_click_on_square(const Square clicked_square) {
    if (is_click_to_unselect(clicked_square)) {
        handle_unselection();
    }

    else if (is_click_to_select(clicked_square)) {
        handle_selection(clicked_square);
    }

    else if (is_click_to_move(clicked_square)) {
        handle_move(clicked_square);
    }
}

bool BoardController::is_click_to_unselect(const Square clicked_square) {
    return clicked_square == board_renderer->selected_square;
} 

bool BoardController::is_click_to_select(const Square clicked_square) {
    return state->is_friendly(clicked_square);
} 

bool BoardController::is_click_to_move(const Square clicked_square) {
    return board_renderer->selected_square != SQ_NONE && move_gen->can_move_to_square(clicked_square);
} 

void BoardController::handle_unselection() {
    board_renderer->unselect();
    move_gen->clear_moves();
} 

void BoardController::handle_selection(const Square clicked_square) {
    board_renderer->select(clicked_square);

    move_gen->update_legal_moves_for_square(clicked_square);

    board_renderer->draw_moves();
} 

void BoardController::handle_move(const Square clicked_square) {
    Move move = move_gen->create_move(board_renderer->selected_square, clicked_square);

    
    if (move.is_promotion()) {
        move_manager->pending_promotion_move = move;
        
        move_manager->ask_what_piece_promote_to();
        return;
    } 
    
    timer_manager->add_time_increment();
    
    move_manager->do_move_and_update_graphics(move);
    state->add_move_to_history(move);

    play_corresponding_sfx(move);

    if (game_started && playing_against_ai) {
       move_manager->make_ai_move();
    }
} 

void BoardController::play_corresponding_sfx(const Move& move) {
    if (!move.is_valid()) {
        return;
    }
    if (~state->check_mask) {
        sound_manager->play_check_sfx();
    }
    else if (move.is_capture() || move.is_en_passant()) {
        sound_manager->play_capture_sfx();
    }
    else if (move.is_castle()) {
        sound_manager->play_castling_sfx();
    }
    else {
        sound_manager->play_move_sfx();
    }
}

bool BoardController::is_left_click(const Ref<InputEventMouseButton> &mouse_event) {
    return  mouse_event->is_pressed() &&
            mouse_event->get_button_index() == MouseButton::MOUSE_BUTTON_LEFT;
}

bool BoardController::is_right_button(const Ref<InputEventMouseButton> &mouse_event) {
    return mouse_event->get_button_index() == MouseButton::MOUSE_BUTTON_RIGHT;
}

bool BoardController::is_out_of_board_bounds(const Vector2 mouse_position) const {
    int right_edge = SQUARE_PX_SIZE * N_COLUMNS;
    int bottom_edge = SQUARE_PX_SIZE * N_ROWS;

    return  mouse_position.x < 0 || mouse_position.x > right_edge ||
            mouse_position.y < 0 || mouse_position.y > bottom_edge;      
}

Square BoardController::screen_coords_to_board_square(const Vector2 screen_coords) const {
    Vector2 square_coords = Vector2(floor(screen_coords.x / SQUARE_PX_SIZE),
                                    floor(screen_coords.y / SQUARE_PX_SIZE));
    
    int flipped_y_coord = 7 - square_coords.y;

    return static_cast<Square>( 8 * flipped_y_coord + square_coords.x );
}