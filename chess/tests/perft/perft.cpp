#include "perft.h"


std::vector<PerftTestCase> PerftTester::perft_test_cases = {
    // Start position
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
    6,
    119060324},

    // Kiwipete
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
    5,
    193690690},
    
    // Position 3
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
    6,
    11030083},

    // Position 4
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
    5,
    15833292},

    // Position 4 mirrored
    {"r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1",
    5,
    15833292},
    
    // Position 5
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
    5,
    89941194},
    
    // Position 6
    {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
    4,
    3894594}
};

void PerftTester::test_perft() {
    int test_cases_count = perft_test_cases.size();
    int passed_tests_count = 0;

    for (PerftTestCase test_case : perft_test_cases) {
        auto start_time = std::chrono::high_resolution_clock::now();

        bool test_passed = validate_case(test_case);

        auto end_time = std::chrono::high_resolution_clock::now();
        double duration = std::chrono::duration<double, std::milli>(end_time - start_time).count();
        
        if (test_passed) {
            passed_tests_count++;

            Logger::log("Total: " + std::to_string(test_case.expected_positions_count));
            
            std::string message = "Passed test! " + std::to_string(passed_tests_count) + "/" + std::to_string(test_cases_count);
            Logger::log(message);
        }

        std::cout << "Tested in " << duration << "ms" << "\n";
        Logger::log("-------------------------------------");
    }
}

bool PerftTester::validate_case(const PerftTestCase& test_case) {
    GameState state = GameState();

    bool test_passed = true;

    Logger::log("Fen: " + test_case.starting_fen);
    Logger::log("Depth: " + std::to_string(test_case.depth));

    try {
        FenLoader::load_fen(&state, test_case.starting_fen);
    }
    catch (const FenException& e) {
        Logger::log(e.what());
        return false;
    }

    int positions_count = perft(state, test_case.depth, test_case.depth);

    if (!assert_and_print("Positions found", positions_count, test_case.expected_positions_count)) {
        test_passed = false;
    }

    return test_passed;
}


int PerftTester::perft(GameState& state, const int depth, const int initial_depth) {
    if (depth == 0) {
        return 1;
    }

    MoveGen move_gen = MoveGen(&state);
    MoveLegalizer move_legalizer = MoveLegalizer(&state, move_gen);

    move_legalizer.calculate_pins_and_checks();
    move_gen.update_available_legal_moves();

    int move_count = move_gen.available_moves_count;
    int positions_count = 0;

    if (depth == 1) {
        return move_count;
    }

    for (int move_index = 0; move_index < move_count; move_index++) {
        Move move = move_gen.available_moves[move_index];

        MoveExecutor::do_move(move, &state);

        int count = perft(state, depth - 1, initial_depth);

        if (depth == initial_depth) {
            Logger::log(move_to_lan_notation(move) + ": " + std::to_string(count));
        }

        positions_count += count;

        MoveExecutor::undo_move(move, &state);
    }

    return positions_count;
}