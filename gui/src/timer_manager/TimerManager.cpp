#include "TimerManager.h"

using namespace godot;

void TimerManager::restart_timers() {
    white_seconds_left = initial_seconds_left;
    black_seconds_left = initial_seconds_left;
}

void TimerManager::update_timers(double delta) {
    if (state->is_white_turn()) { 
        white_seconds_left -= delta;
    }
    else { 
        black_seconds_left -= delta;
    }

    if (white_seconds_left < 0) {
        white_seconds_left = 0;
    }
    if (black_seconds_left < 0) {
        black_seconds_left = 0;
    }
}   

void TimerManager::add_time_increment() {
    if (state->is_white_turn()) { 
        white_seconds_left += increment_seconds;
    }
    else { 
        black_seconds_left += increment_seconds;
    }
}

void TimerManager::set_timer_mode(int seconds, int increment) {
    white_seconds_left = seconds;
    black_seconds_left = seconds;
    initial_seconds_left = seconds;
    increment_seconds = increment;
}

void TimerManager::update_timer_labels(const bool board_flipped) {
    int bottom_seconds = board_flipped ? black_seconds_left : white_seconds_left;
    int top_seconds = board_flipped ? white_seconds_left : black_seconds_left;

    if (bottom_seconds < 0) {
        bottom_seconds = 0;
    }
    if (top_seconds < 0) {
        top_seconds = 0;
    }

    gui->set_top_timer_label(top_seconds);
    gui->set_bottom_timer_label(bottom_seconds);
}

GameOverType TimerManager::check_timeout() {
    if (white_seconds_left <= 0) {
        return TIMEOUT_FOR_WHITE;
    }
    if (black_seconds_left <= 0) {
        return TIMEOUT_FOR_BLACK;
    }

    return GAME_OVER_NONE;
}