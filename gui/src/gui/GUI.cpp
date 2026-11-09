#include "GUI.h"

using namespace godot;

void GUI::show_promotion_menu(Colour colour) {
    promotion_menu->call_deferred("show_promotion_menu");

    if (colour == WHITE) {
        promotion_menu->call_deferred("show_buttons_of_white_pieces");
    }
    else {
        promotion_menu->call_deferred("show_buttons_of_black_pieces");
    }
}

void GUI::hide_promotion_menu() {
    promotion_menu->call_deferred("hide_promotion_menu");
}

void GUI::show_game_over_message(const GameOverType game_over_type) {
    godot::String message = game_over_messages[game_over_type].c_str();

    game_over_message->call("display_message", message);
}

void GUI::hide_game_over_message() {
    game_over_message->call_deferred("hide_message");
}

void GUI::set_bottom_timer_label(double seconds) {
    gui->call_deferred("set_bottom_timer_label", seconds);
}

void GUI::set_top_timer_label(double seconds) {
    gui->call_deferred("set_top_timer_label", seconds);
}

bool GUI::is_settings_menu_visible() {
    return settings_menu->is_visible();
}

void GUI::show_settings_menu() {
    settings_menu->call_deferred("show");
}

void GUI::hide_settings_menu() {
    settings_menu->call_deferred("hide");
}

void GUI::update_curr_fen_in_settings_menu() {
    settings_menu->call_deferred("set_curr_fen", curr_fen.c_str());
}

void GUI::display_error(const std::exception& e) {
    error_menu->call_deferred("display_error_message", e.what());
    game_started = false;
}

void GUI::hide_error_menu() {
    error_menu->call_deferred("hide");
}