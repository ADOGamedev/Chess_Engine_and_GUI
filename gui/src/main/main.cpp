#include "main.h"

using namespace godot;

void Main::_bind_methods() {
    ClassDB::bind_method(D_METHOD("on_promotion_piece_selected", "piece"), &Main::on_promotion_piece_selected);
    
    ClassDB::bind_method(D_METHOD("on_play_button_pressed"), &Main::on_play_button_pressed);
    ClassDB::bind_method(D_METHOD("on_previous_move_button_pressed"), &Main::on_previous_move_button_pressed);
    ClassDB::bind_method(D_METHOD("on_next_move_button_pressed"), &Main::on_next_move_button_pressed);
    ClassDB::bind_method(D_METHOD("flip_board"), &Main::flip_board);
    ClassDB::bind_method(D_METHOD("on_black_button_pressed"), &Main::on_black_button_pressed);
    ClassDB::bind_method(D_METHOD("on_random_colour_button_pressed"), &Main::on_random_colour_button_pressed);
    ClassDB::bind_method(D_METHOD("on_white_button_pressed"), &Main::on_white_button_pressed);
    ClassDB::bind_method(D_METHOD("set_ai_mode", "play_against_ai"), &Main::set_ai_mode);
    ClassDB::bind_method(D_METHOD("quit_engine"), &Main::quit_engine);
    ClassDB::bind_method(D_METHOD("is_settings_menu_visible"), &Main::is_settings_menu_visible);
    ClassDB::bind_method(D_METHOD("show_settings_menu"), &Main::show_settings_menu);
    ClassDB::bind_method(D_METHOD("hide_settings_menu"), &Main::hide_settings_menu);
    ClassDB::bind_method(D_METHOD("hide_error_menu"), &Main::hide_error_menu);
    ClassDB::bind_method(D_METHOD("set_game_config", "engine_path", "start_fen"), &Main::set_game_config);
    ClassDB::bind_method(D_METHOD("set_timer_mode", "seconds", "increment"), &Main::set_timer_mode);
    ClassDB::bind_method(D_METHOD("apply_top_resignation"), &Main::apply_top_resignation);
    ClassDB::bind_method(D_METHOD("apply_bottom_resignation"), &Main::apply_bottom_resignation);
    ClassDB::bind_method(D_METHOD("hide_game_over_message"), &Main::hide_game_over_message);
}

void Main::_ready() {
    set_process_input(true);

    godot_logger = GodotLogger();
    Logger::set_logger(&godot_logger);

    Node2D* arrow_drawer = get_node<Node2D>("ArrowDrawer");
    board_renderer = BoardRenderer(&state, &move_gen, arrow_drawer);
    board_renderer.set_tilemaps(
        get_node<TileMapLayer>("pieces_tilemap"),
        get_node<TileMapLayer>("selections_tilemap"),
        get_node<TileMapLayer>("red_selections_tilemap"),
        get_node<TileMapLayer>("moves_tilemap")
    );

    Control* promotion_menu = get_node<Control>("promotion_menu");
    Control* settings_menu = get_node<Control>("settings");

    gui = GUI(  get_node<Control>("gui"),
                promotion_menu,
                settings_menu,
                get_node<Control>("game_over_message"),
                get_node<Control>("error_menu")
    );

    timer_manager = TimerManager(&state, &gui);
    sound_manager = SoundManager(
        get_node<AudioStreamPlayer>("move_sfx"),
        get_node<AudioStreamPlayer>("capture_sfx"),
        get_node<AudioStreamPlayer>("check_sfx"),
        get_node<AudioStreamPlayer>("castling_sfx"),
        get_node<AudioStreamPlayer>("game_over_sfx")
    );

    promotion_menu->connect("promotion_piece_selected", Callable(this, "on_promotion_piece_selected"));

    ZobristHasher::initialize_zobrist_values();
    setup_start_position();
    
    board_renderer.draw_board();
    
    timer_manager.update_timer_labels(board_renderer.board_flipped);

    PolyglotZobristTester::test_polyglot_zobrist();
}

void Main::setup_start_position() {
    move_manager.clear_moves_history();

    GameState prev_state = state;
    state = GameState();

    move_gen.reset(&state);    

    try {
        FenLoader::load_fen(&state, starting_fen);
    }
    catch (const FenException& e) {
        state = prev_state;

        gui.display_error(e);
        Logger::log(e.what());
    }

    curr_fen = starting_fen;
    gui.update_curr_fen_in_settings_menu();

    board_renderer.clear_all();
    board_renderer.draw_board();

    gui.hide_game_over_message();
    timer_manager.restart_timers();

    ZobristHasher::set_zobrist_key_of_state(state);
    state.add_curr_key_to_repetition_list();

    GameOverType game_over = GameOverChecker::check_game_over(state);
    if (game_over != GAME_OVER_NONE) {
        gui.show_game_over_message(game_over);
        game_started = false;
    }
}

void Main::_process(double delta) {
    if (!game_started) {
        return;
    }

    if (state.turn != prev_turn && move_manager.curr_move_index >= 0) {
        Move& move = state.moves_history[move_manager.curr_move_index];
        board_controller.play_corresponding_sfx(move);
    }

    prev_turn = state.turn;

    timer_manager.update_timers(delta);
    timer_manager.update_timer_labels(board_renderer.board_flipped);

    GameOverType timeout_game_over = timer_manager.check_timeout();

    if (timeout_game_over != GAME_OVER_NONE) {
        sound_manager.play_game_over_sfx();
        gui.show_game_over_message(timeout_game_over);
        game_started = false;
    }
}

void Main::_unhandled_input(const Ref<InputEvent> &event) {
    board_controller.handle_input(event);
}

bool Main::should_handle_input() {
    return !game_started || !move_manager.not_in_actual_move();
}

void Main::on_play_button_pressed() {
    setup_start_position();
    game_started = true;

    if (board_controller.is_ai_turn()) {
        move_manager.make_ai_move();
    }
}

void Main::on_promotion_piece_selected(int piece) {
    move_manager.on_promotion_piece_selected(piece);
}

void Main::on_previous_move_button_pressed() {
    bool should_play_sound = move_manager.curr_move_index > -1;

    if (should_play_sound) {
        Move& move = state.moves_history[move_manager.curr_move_index];
        board_controller.play_corresponding_sfx(move);
    }

    move_manager.go_back_one_move(); 
}

void Main::on_next_move_button_pressed() {
    bool should_play_sound = move_manager.curr_move_index + 1 < state.moves_history.size();

    if (should_play_sound) {
        Move& move = state.moves_history[move_manager.curr_move_index + 1];
        board_controller.play_corresponding_sfx(move);
    }

    move_manager.advance_one_move();
}

void Main::flip_board() {
    board_renderer.flip_board();
}

void Main::on_black_button_pressed() {
    if (game_started) {
        return;
    }
    
    setup_start_position();
    
    player_colour = BLACK;

    if (!board_renderer.board_flipped) {
        board_renderer.flip_board();
    }
}

void Main::on_random_colour_button_pressed() {
    if (game_started) {
        return;
    }
    
    setup_start_position();
    
    double random_number = randf();
    
    if (random_number > 0.5) {
        player_colour = !player_colour;
        board_renderer.flip_board();
    }
}

void Main::on_white_button_pressed() {
    if (game_started) {
        return;
    }
    
    setup_start_position();

    player_colour = WHITE;
    
    if (board_renderer.board_flipped && !game_started) {
        board_renderer.flip_board();
    }
}

void Main::set_ai_mode(bool play_against_ai) {
    try {
        playing_against_ai = play_against_ai;
        
        uci_communicator.end_process();
        
        if (playing_against_ai) {
                uci_communicator.start_uci_engine(uci_engine_path);
        }
    } catch (const UCIEngineException& e) {
        gui.display_error(e);
        Logger::log(e.what());
    }
}

void Main::quit_engine() {
    uci_communicator.end_process();
}

bool Main::is_settings_menu_visible() {
    return gui.is_settings_menu_visible();
}

void Main::show_settings_menu() {
    gui.show_settings_menu();
}

void Main::hide_settings_menu() {
    gui.hide_settings_menu();
}

void Main::hide_error_menu() {
    gui.hide_error_menu();
}

void Main::hide_game_over_message() {
    gui.hide_game_over_message();
}

void Main::set_game_config(const String engine_path, const String start_fen) {
    starting_fen = start_fen.utf8().get_data();
    uci_engine_path = engine_path.utf8().get_data();
    
    if (!game_started) {
        setup_start_position();
    
        timer_manager.restart_timers();
        timer_manager.update_timer_labels(board_renderer.board_flipped);
    }
    
    set_ai_mode(playing_against_ai);
}

void Main::set_timer_mode(int seconds, int increment) {
    if (game_started) {
        return;
    }
    
    timer_manager.set_timer_mode(seconds, increment);
    timer_manager.update_timer_labels(board_renderer.board_flipped);
}

void Main::apply_bottom_resignation() {
    Colour bottom_colour = board_renderer.board_flipped ? BLACK : WHITE;
    apply_resignation_to_colour(bottom_colour);
    
    sound_manager.play_game_over_sfx();
}

void Main::apply_top_resignation() {
    Colour top_colour = board_renderer.board_flipped ? WHITE : BLACK;
    apply_resignation_to_colour(top_colour);

    sound_manager.play_game_over_sfx();
}

void Main::apply_resignation_to_colour(const Colour colour) {
    if (!game_started) {
        return;
    }

    game_started = false;

    GameOverType game_over = colour == WHITE ? WHITE_RESIGNATION : BLACK_RESIGNATION;
    gui.show_game_over_message(game_over);
}





