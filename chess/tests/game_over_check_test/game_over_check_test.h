#pragma once

#include "../test_utils.h"

#include "../../game_state/GameState.h"
#include "../../fen_loader/FenLoader.h"
#include "../../game_over_checker/GameOverChecker.h"

struct GameOverCheckTestCase {
    std::string starting_fen;
    GameOverType expected_game_over_type;
};

class GameOverCheckTester {

public:
    static void test_game_over_check();

private:
    static bool validate_case(GameOverCheckTestCase test_case);

    static std::vector<GameOverCheckTestCase> game_over_check_test_cases;
};