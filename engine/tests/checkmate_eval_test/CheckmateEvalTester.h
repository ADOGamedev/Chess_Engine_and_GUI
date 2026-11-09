#pragma once

#include "../../constants/constants.h"
#include "../../../chess/tests/test_utils.h"

#include "../../engine/engine.h"

struct CheckmateEvalTestCase {
    std::string starting_fen;
    int depth;
    int expected_eval;
};

class CheckmateEvalTester {

public:
    static void test_checkmate_eval();

private:
    static bool validate_case(CheckmateEvalTestCase test_case);

    static std::vector<CheckmateEvalTestCase> checkmate_eval_test_cases;
    static Engine engine;
};