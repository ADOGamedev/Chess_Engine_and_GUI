#pragma once

#include <random>

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/timer.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/tile_map_layer.hpp>
#include <godot_cpp/variant/node_path.hpp>

#include <godot_cpp/variant/variant.hpp>
#include <godot_cpp/variant/string.hpp>

#include "../../chess/constants/constants.h"
#include "../../chess/utils/utils.h"

#include "../../chess/fen_loader/FenLoader.h"
#include "../../chess/game_state/GameState.h"

#include "../../chess/move_gen/MoveGen.h"

#include "../../chess/logger/Logger.h"
#include "../../engine/uci_engine_communicator/UCIEngineCommunicator.h"

#include "../../chess/tests/game_over_check_test/game_over_check_test.h"
#include "../../chess/tests/move_creation_test/move_creation_test.h"
#include "../../chess/tests/fen_loader_test/fen_loader_test.h"
#include "../../chess/tests/perft/perft.h"
#include "../../chess/tests/zobrist_collisions_test/ZobristCollisionsTester.h"
#include "../../chess/tests/polyglot_zobrist_test/PolyglotZobristTester.h"

#include "../board_renderer/BoardRenderer.h"
#include "../board_controller/BoardController.h"
#include "../timer_manager/TimerManager.h"
#include "../move_manager/MoveManager.h"

class GodotLogger : public Logger::Logger {
    void log(const std::string& message) override {
        godot::UtilityFunctions::print(message.c_str());
    }
};

namespace godot
{
    
class Main : public Node2D {
    GDCLASS(Main, Node2D)
        
protected:
    static void _bind_methods();

public:
    Main() : 
        move_gen(&state),
        move_legalizer(&state, move_gen, move_gen.curr_moves),
        timer_manager(&state, &gui),
        move_manager(&state, &move_gen, &board_renderer, &gui, &timer_manager, &uci_communicator, &sound_manager),
        board_controller(&state, &move_gen, &board_renderer, &move_manager, &timer_manager, &gui, &sound_manager) {}
        
    ~Main() = default;

    void _ready() override;
    void _process(double delta) override;
    virtual void _unhandled_input(const Ref<InputEvent> &event) override;
    bool should_handle_input();

    void setup_start_position();

    void on_play_button_pressed();
    
    void on_promotion_piece_selected(int piece);
    
    void on_previous_move_button_pressed();
    void on_next_move_button_pressed();
    
    void flip_board();
    void on_black_button_pressed();
    void on_random_colour_button_pressed();
    void on_white_button_pressed();
    
    void set_ai_mode(bool play_versus_ai);
    void quit_engine();
    
    bool is_settings_menu_visible();
    void show_settings_menu();
    void hide_settings_menu();

    void hide_error_menu();

    void hide_game_over_message();

    void set_game_config(const String engine_path, const String start_fen);

    void set_timer_mode(int seconds, int increment);

    void apply_bottom_resignation();
    void apply_top_resignation();
    void apply_resignation_to_colour(const Colour colour);

    GodotLogger godot_logger;


    GameState state;
    GUI gui;
    SoundManager sound_manager;
    UCIEngineCommunicator uci_communicator;

    MoveGen move_gen;
    MoveLegalizer move_legalizer;
    BoardRenderer board_renderer;
    TimerManager timer_manager;

    MoveManager move_manager;
    BoardController board_controller;

    Colour prev_turn = WHITE;
};
}