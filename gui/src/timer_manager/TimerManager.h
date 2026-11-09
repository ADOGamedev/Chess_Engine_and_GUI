#pragma once

#include <godot_cpp/classes/control.hpp>

#include "../../chess/game_state/GameState.h"
#include "../configs/game_config.h"
#include "../gui/GUI.h"

namespace godot {

class TimerManager {

public:
    TimerManager(GameState* state, GUI* gui) :
        state(state), gui(gui) {}

    void restart_timers();
    void update_timer_labels(const bool board_flipped);
    void update_timers(double delta);
    void add_time_increment();
    void set_timer_mode(int seconds, int increment);

    GameOverType check_timeout();

    double white_seconds_left = 600;
    double black_seconds_left = 600;
    
private:
    GameState* state;
    GUI* gui;

};

}