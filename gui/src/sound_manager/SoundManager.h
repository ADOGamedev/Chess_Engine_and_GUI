#pragma once


#include <godot_cpp/classes/audio_stream_player.hpp>

#include "../../chess/logger/Logger.h"

using godot::AudioStreamPlayer;

class SoundManager {

public:
    SoundManager() = default;
    SoundManager(AudioStreamPlayer* move, AudioStreamPlayer* capture, AudioStreamPlayer* check, AudioStreamPlayer* castling, AudioStreamPlayer* game_over) :
        move(move), capture(capture), check(check), castling(castling), game_over(game_over)
    {}

    void play_move_sfx();
    void play_capture_sfx();
    void play_check_sfx();
    void play_castling_sfx();
    void play_game_over_sfx();

private:
    AudioStreamPlayer* move;
    AudioStreamPlayer* capture;
    AudioStreamPlayer* check;
    AudioStreamPlayer* castling;
    AudioStreamPlayer* game_over;

};