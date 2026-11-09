#include "SoundManager.h"

void SoundManager::play_move_sfx() {
    move->call_deferred("play");
}

void SoundManager::play_capture_sfx() {
    capture->call_deferred("play");
}

void SoundManager::play_check_sfx() {
    check->call_deferred("play");
}

void SoundManager::play_castling_sfx() {
    castling->call_deferred("play");
}

void SoundManager::play_game_over_sfx() {
    game_over->call_deferred("play");
}
