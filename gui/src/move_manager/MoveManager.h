#pragma once

#include <vector>

#include <godot_cpp/classes/control.hpp>

#include "../../chess/move_gen/Move.h"
#include "../../chess/move_gen/MoveGen.h"
#include "../../chess/move_executor/MoveExecutor.h"
#include "../../chess/fen_loader/StateToFenConverter.h"

#include "../../chess/game_over_checker/GameOverChecker.h"

#include "../configs/game_config.h"

#include "../board_renderer/BoardRenderer.h"
#include "../../engine/uci_engine_communicator/UCIEngineCommunicator.h"
#include "../gui/GUI.h"
#include "../timer_manager/TimerManager.h"
#include "../sound_manager/SoundManager.h"

using godot::BoardRenderer, godot::GUI, godot::TimerManager;

class MoveManager {

public:
    MoveManager(GameState* state, MoveGen* move_gen, BoardRenderer* board_renderer, GUI* gui, TimerManager* timer_manager, UCIEngineCommunicator* uci_communicator, SoundManager* sound_manager) :
        state(state), move_gen(move_gen), board_renderer(board_renderer), timer_manager(timer_manager), uci_communicator(uci_communicator),
        move_legalizer(MoveLegalizer(state, *move_gen, move_gen->curr_moves)), gui(gui), sound_manager(sound_manager) {
    }

    void ask_what_piece_promote_to() const;
    void on_promotion_piece_selected(int piece);

    void do_move_and_update_graphics(const Move& move);
    void undo_move_and_update_graphics(const Move& move);

    void make_ai_move();

    void go_back_one_move();
    bool can_go_back_one_move();

    void advance_one_move();
    bool can_advance_one_move();

    bool not_in_actual_move();
    void clear_moves_history();

    Move pending_promotion_move;

private:
    GameState* state;
    MoveGen* move_gen;
    MoveLegalizer move_legalizer;
    BoardRenderer* board_renderer;
    TimerManager* timer_manager;
    UCIEngineCommunicator* uci_communicator;
    GUI* gui;
    SoundManager* sound_manager;

public:
    int curr_move_index = -1;
};