#pragma once

#include <exception>

#include <godot_cpp/classes/control.hpp>

#include "../../chess/constants/constants.h"
#include "../configs/game_config.h"

#include "../../chess/logger/Logger.h"

namespace godot {

class GUI {

public:
    GUI() = default;
    GUI(Control* gui, Control* promotion_menu, Control* settings_menu, Control* game_over_message, Control* error_menu) :
        gui(gui), promotion_menu(promotion_menu), settings_menu(settings_menu), game_over_message(game_over_message), error_menu(error_menu) {}

    void show_promotion_menu(Colour colour);
    void hide_promotion_menu();

    void show_game_over_message(const GameOverType game_over_type);
    void hide_game_over_message();

    void set_bottom_timer_label(double seconds);
    void set_top_timer_label(double seconds);

    bool is_settings_menu_visible();
    void show_settings_menu();
    void hide_settings_menu();

    void update_curr_fen_in_settings_menu();

    void display_error(const std::exception& e);
    void hide_error_menu();

private:
    Control* gui;

    Control* promotion_menu;
    Control* settings_menu;
    Control* game_over_message;
    Control* error_menu;
};

}