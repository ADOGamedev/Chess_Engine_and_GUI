#include "BoardEvalTester.h"

Engine BoardEvalTester::engine = Engine();

std::vector<BoardEvalTestCase> BoardEvalTester::board_eval_test_cases = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    0, 24},
 
    {"4k3/8/8/8/8/8/8/4K3 w - - 0 1",
    0, 0},
 
    {"8/pppppppp/8/8/8/8/PPPPPPPP/8 w - - 0 1",
    0, 0},
 
    {"4k3/8/8/8/8/8/8/4KQ2 w - - 0 1",
    920, 4},
 
    {"4k3/8/8/8/8/8/8/R3K3 w - - 0 1",
    499, 2},
 
    {"4k3/8/8/8/8/8/8/4K2N w - - 0 1",
    221, 1},
 
    {"4k3/8/8/8/8/8/8/2B1K3 w - - 0 1",
    277, 1},
 
    {"4k3/8/8/8/8/8/4PP2/4K3 w - - 0 1",
    201, 0},
 
    {"8/P7/8/8/8/8/8/k3K3 w - - 0 1",
    318, 0},
 
    {"4k3/8/8/8/8/8/8/R1N1K3 w - - 0 1",
    758, 3},
 
    {"rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2",
    0, 24},
 
    {"rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq - 0 2",
    -15, 24},
 
    {"r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R w KQkq - 2 4",
    20, 24},
 
    {"r1bqk2r/pp2bppp/2n1pn2/3p4/2PP4/2NBPN2/PP3PPP/R1BQ1RK1 w kq - 0 1",
    134, 24},
 
    {"QQQQK3/8/8/8/8/8/8/3kqqqq w - - 0 1",
    -219, 24},
 
    {"8/8/8/8/8/7k/7p/7K w - - 0 1",
    -337, 0}
};


void BoardEvalTester::test_board_eval() {
    int test_cases_count = board_eval_test_cases.size();
    int passed_tests_count = 0;

    for (BoardEvalTestCase test_case : board_eval_test_cases) {
        bool test_passed = validate_case(test_case);

        if (test_passed) {
            passed_tests_count++;

            std::cout << "Passed test! " << passed_tests_count << "/" << test_cases_count << "\n";
        }

        std::cout << "-------------------------------------" << "\n";
    }
}

bool BoardEvalTester::validate_case(BoardEvalTestCase test_case) {
    bool test_passed = true;

    engine.set_up_position(test_case.fen);

    std::cout << "Fen: " << test_case.fen << "\n";

    int eval_score = engine.evaluate();
    int game_phase = engine.state.game_phase;

    if (eval_score != test_case.expected_eval) {
        test_passed = false;
        std::cout << "Eval score: " << eval_score << " expected " << test_case.expected_eval << "\n";
    }
    if (game_phase != test_case.expected_game_phase) {
        test_passed = false;
    }
    
    return test_passed;
}