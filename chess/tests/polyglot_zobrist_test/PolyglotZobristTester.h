#pragma once

#include "../test_utils.h"

#include "../../game_state/GameState.h"
#include "../../fen_loader/FenLoader.h"
#include "../../zobrist_hasher/ZobristHasher.h"


struct PolyglotZobristTestCase {
    std::string fen;
    uint64_t expected_key;
};

class PolyglotZobristTester {

public:
    static void test_polyglot_zobrist();

private:
    static bool validate_case(PolyglotZobristTestCase test_case);

    static std::vector<PolyglotZobristTestCase> polyglot_zobrist_test_cases;
};