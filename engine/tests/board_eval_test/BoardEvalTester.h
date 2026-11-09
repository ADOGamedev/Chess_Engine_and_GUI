#pragma once

#include "../../constants/constants.h"
#include "../../../chess/tests/test_utils.h"

#include "../../engine/engine.h"

struct BoardEvalTestCase {
    std::string fen;
    int expected_eval;
    int expected_game_phase;
};

class BoardEvalTester {

public:
    static void test_board_eval();

private:
    static bool validate_case(BoardEvalTestCase test_case);

    static std::vector<BoardEvalTestCase> board_eval_test_cases;
    static Engine engine;
};