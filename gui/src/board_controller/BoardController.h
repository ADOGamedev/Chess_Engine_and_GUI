#pragma once

#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/classes/input_event_mouse_button.hpp>

#include "../../chess/constants/constants.h"

#include "../../chess/move_executor/MoveExecutor.h"
#include "../../chess/game_over_checker/GameOverChecker.h"

#include "../board_renderer/BoardRenderer.h"
#include "../configs/game_config.h"
#include "../move_manager/MoveManager.h"
#include "../timer_manager/TimerManager.h"
#include "../sound_manager/SoundManager.h"
#include "../gui/GUI.h"

namespace godot {

class BoardController {

public:
    BoardController(GameState* state, MoveGen* move_gen, BoardRenderer* board_renderer, MoveManager* move_manager, TimerManager* timer_manager, GUI* gui, SoundManager* sound_manager) :
        state(state), move_gen(move_gen), board_renderer(board_renderer), move_manager(move_manager), timer_manager(timer_manager), gui(gui), sound_manager(sound_manager) {}

    void handle_input(const Ref<InputEventMouseButton> &event);
    bool is_ai_turn();

    void play_corresponding_sfx(const Move& move);

private:
    void handle_right_click(const Ref<InputEventMouseButton> &mouse_event);
    void handle_click_on_square(const Square clicked_square);

    bool is_click_to_unselect(const Square clicked_square);
    bool is_click_to_select(const Square clicked_square);
    bool is_click_to_move(const Square clicked_square);

    void handle_unselection();
    void handle_selection(const Square clicked_square);
    void handle_move(const Square clicked_square);

    bool is_left_click(const Ref<InputEventMouseButton> &mouse_event);
    bool is_right_button(const Ref<InputEventMouseButton> &mouse_event);


    bool is_out_of_board_bounds(const Vector2 mouse_position) const;
    Square screen_coords_to_board_square(const Vector2 screen_coords) const;

    GameState* state;
    MoveGen* move_gen;
    GUI* gui;
    BoardRenderer* board_renderer;
    MoveManager* move_manager;
    TimerManager* timer_manager;
    SoundManager* sound_manager;

    Square right_clicked_square = SQ_NONE;
};

}