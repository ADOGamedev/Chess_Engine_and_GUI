#pragma once

#include "../constants/constants.h"

#include "../move_gen/MoveGen.h"
#include "../game_state/GameState.h"
#include "../logger/Logger.h"


class GameOverChecker {

public:
	static GameOverType check_game_over(GameState& state);
	static int get_repetition_count(GameState* state);

private:
	GameOverChecker(GameState* state) :
		state(state), move_gen(state), move_legalizer(state, move_gen) {}

	bool is_there_any_move_availale();
	bool is_three_fold_repetition();
	bool is_fifty_moves_rule();

	bool has_enough_material();

	bool more_than_two_pieces_of_the_same_colour();
	bool pawns_queens_or_rooks_in_board();
	bool exactly_one_bishop_in_each_side();
	bool more_than_three_pieces_on_the_board();

	bool are_bishops_on_same_square_colour();

	Colour get_square_colour(const Square square) const;
	
	GameState* state;
	MoveGen move_gen;
	MoveLegalizer move_legalizer;
};