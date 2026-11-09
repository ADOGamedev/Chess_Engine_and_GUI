#include "CheckmateEvalTester.h"

Engine CheckmateEvalTester::engine = Engine();

std::vector<CheckmateEvalTestCase> CheckmateEvalTester::checkmate_eval_test_cases = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
     1, 0},

    {"7k/8/6Q1/6K1/8/8/8/8 b - - 0 1",
     1, 0},

    {"7k/8/8/6K1/3Q4/8/8/8 b - - 100 1",
     1, 0},

    {"rnb1kbnr/pppp1ppp/8/4p3/6Pq/5P2/PPPPP2P/RNBQKBNR w KQkq - 1 3",
     1, -CHECKMATE_SCORE},

    {"r1bqkb1r/pppp1Qpp/2n2n2/4p3/2B1P3/8/PPPP1PPP/RNB1K1NR b KQkq - 0 4",
     1, -CHECKMATE_SCORE},

    {"3r2k1/5ppp/8/8/8/8/5PPP/3R2K1 w - - 0 1",
     1, CHECKMATE_SCORE},

    {"7k/8/5KQ1/8/8/8/8/8 w - - 0 1",
     1, CHECKMATE_SCORE},

    {"r6k/6pp/8/6N1/2Q5/1B6/5PPP/6K1 w - - 0 1",
     3, CHECKMATE_SCORE},

    {"5rk1/6pQ/6P1/8/8/8/8/6K1 w - - 0 1",
     3, CHECKMATE_SCORE},

    {"8/8/5R2/8/8/4K1k1/8/8 w - - 0 1",
     12, -CHECKMATE_SCORE}
};

void CheckmateEvalTester::test_checkmate_eval() {
    int test_cases_count = checkmate_eval_test_cases.size();
    int passed_tests_count = 0;

    for (CheckmateEvalTestCase test_case : checkmate_eval_test_cases) {
        bool test_passed = validate_case(test_case);

        if (test_passed) {
            passed_tests_count++;

            std::cout << "Passed test! " << passed_tests_count << "/" << test_cases_count << "\n";
        }

        std::cout << "-------------------------------------" << "\n";
    }
}

bool CheckmateEvalTester::validate_case(CheckmateEvalTestCase test_case) {
    bool test_passed = true;

    engine.set_up_position(test_case.starting_fen);

    std::cout << "Fen: " << test_case.starting_fen << "\n";
    std::cout << "Depth: " << test_case.depth << "\n";

    int score = engine.negamax(test_case.depth, INT_MIN, INT_MAX, 1);
    std::string best_move = engine.go_depth(test_case.depth);

    if (score <= test_case.expected_eval - test_case.depth || score >= test_case.expected_eval - test_case.depth) {
        test_passed = false;
        std::cout << "Eval score: " << score << " expected " << test_case.expected_eval << "\n";
    }
    
    return test_passed;
}