#include "game_over_check_test.h"

std::vector<GameOverCheckTestCase> GameOverCheckTester::game_over_check_test_cases = {
    {"r5k1/ppR2ppp/2p5/5b2/1PP2B2/1N6/P4PPP/3r2K1 w - - 0 1", CHECKMATE_TO_WHITE},
    {"8/8/8/8/8/1k6/2q5/2K5 w - - 0 1", CHECKMATE_TO_WHITE},
    {"r3r1k1/p4ppp/1p6/8/2P5/2B5/PP3nPP/6RK w - - 0 1", CHECKMATE_TO_WHITE},

    {"3R2k1/pp3ppp/2p4n/5b2/1PP2B2/1N6/Pr3PPP/6K1 b - - 0 1", CHECKMATE_TO_BLACK},
    {"2k5/2Q5/3K4/8/8/8/8/8 b - - 0 1", CHECKMATE_TO_BLACK},
    {"r5rk/p4Npp/1p6/8/2P5/2B2N2/PP3PPP/R5K1 b - - 0 1", CHECKMATE_TO_BLACK},

    {"8/3k4/8/8/8/6q1/8/7K w - - 0 1", STALEMATE},
    {"7k/8/6Q1/8/8/5K2/8/8 b - - 0 1", STALEMATE},
    {"8/5k2/7K/7P/4b3/5n2/8/8 w - - 0 1", STALEMATE},

    {"8/8/4k3/8/8/4K3/8/8 w - - 0 1", INSUFFICIENT_MATERIAL},
    {"8/4k3/8/8/6b1/1B2K3/8/8 w - - 0 1", INSUFFICIENT_MATERIAL},
    {"k7/8/8/2K5/8/4n3/8/8 w - - 0 1", INSUFFICIENT_MATERIAL},
    
    {"r2q1rk1/pbp1bppp/1pn1pn2/3p4/3P4/2PBPN2/PP1NQPPP/R1B2RK1 w - - 0 1", GAME_OVER_NONE},
    {"8/4k3/8/6n1/8/1B2K3/8/8 w - - 0 1", GAME_OVER_NONE},
    {"8/4k3/8/6b1/8/1B2K3/8/8 w - - 0 1", GAME_OVER_NONE},
    {"8/4k3/8/7P/8/4K3/8/8 w - - 0 1", GAME_OVER_NONE},
    {"8/4k3/8/7q/8/4K3/8/8 w - - 0 1", GAME_OVER_NONE},
    {"8/4k3/8/4r3/8/4K3/8/8 w - - 0 1", GAME_OVER_NONE},
    {"k7/8/8/2K2N2/8/2n5/8/8 w - - 0 1", GAME_OVER_NONE},
};

void GameOverCheckTester::test_game_over_check() {
    int test_cases_count = game_over_check_test_cases.size();
    int passed_tests_count = 0;

    for (GameOverCheckTestCase test_case : game_over_check_test_cases) {
        bool test_passed = validate_case(test_case);

        if (test_passed) {
            passed_tests_count++;

            std::string message = "Passed test! " + std::to_string(passed_tests_count) + "/" + std::to_string(test_cases_count);
            Logger::log(message);
        }

        Logger::log("-------------------------------------");
    }
}

bool GameOverCheckTester::validate_case(GameOverCheckTestCase test_case) {
    GameState state = GameState();

    bool test_passed = true;

    Logger::log("Fen: " + test_case.starting_fen);

    try {
        FenLoader::load_fen(&state, test_case.starting_fen);
    }
    catch (const FenException& e) {}

    GameOverType game_over_type = GameOverChecker::check_game_over(state);

    if (!assert_and_print("Game over type", game_over_type, test_case.expected_game_over_type)) {
        test_passed = false;
    }

    return test_passed;    
}