#include "uci.h"

void UCIReader::read_input() {
    std::string input;

    while (std::getline(std::cin, input)) {
        if (input == "uci") {
            std::cout << "id name ADOCE\n";
            std::cout << "id author ADOGamedev\n";
            std::cout << "uciok\n";
        }
        else if (input == "isready") {
            std::cout << "readyok\n";
        }
        else if (input == "quit") {
            break;
        }
        else if (input.substr(0, 8) == "position") {
            read_command_position(input);
        }
        else if (input.substr(0, 2) == "go") {
            read_command_go(input);
        }
        else {
            std::cout << "invalid command\n";
        }
    }
}

void UCIReader::read_command_go(std::string command) {
    std::istringstream str_stream(command);
    std::string token;
    int depth = -1;
    int movetime = -1;
    TimeStruct time_struct = TimeStruct();

    // TODO: make this more readable
    while (str_stream >> token) {
        if (token == "depth") {
            if (str_stream >> depth && depth > 0) {
                std::cout << "searching at depth " << depth << "\n";
                engine->go_depth(depth);
                return;
            }
            else {
                std::cout << "invalid command\n";
                return;
            }
        }
        else if (token == "movetime") {
            if (str_stream >> movetime && movetime > 0) {
                std::cout << "searching for " << movetime << " ms\n";
                engine->go_movetime(movetime);
                return;
            }
            else {
                std::cout << "invalid command\n";
                return;
            }
        }
        else if (token == "wtime") {
            if (str_stream >> time_struct.wtime && time_struct.wtime >= 0) {
            }
            else {
                std::cout << "invalid command\n";
                return;
            }
        }
        else if (token == "btime") {
            if (str_stream >> time_struct.btime && time_struct.btime >= 0) {}
            else {
                std::cout << "invalid command\n";
                return;
            }
        }
        else if (token == "winc") {
            if (str_stream >> time_struct.winc && time_struct.winc >= 0) {}
            else {
                std::cout << "invalid command\n";
                return;
            }
        }
        else if (token == "binc") {
            if (str_stream >> time_struct.binc && time_struct.binc >= 0) {}
            else {
                std::cout << "invalid command\n";
                return;
            }
        }

    }

    if (time_struct.is_valid()) {
        engine->go_time_inc(time_struct.wtime, time_struct.btime, time_struct.winc, time_struct.binc);
    }
}

void UCIReader::read_command_position(std::string command) {
    std::istringstream str_stream(command);
    std::string token;
    std::string fen;
    std::string moves;

    bool reading_fen = false;
    bool reading_moves = false;

    while (str_stream >> token) {
        if (token == "startpos") {
            fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0";
        }
        else if (token == "fen") {
            reading_fen = true;
            reading_moves = false;
            continue;
        }
        else if (token == "moves") {
            reading_fen = false;
            reading_moves = true;
            continue;
        }

        if (reading_fen) {
            fen += token + " ";
        }
        if (reading_moves) {
            moves += token + " ";
        }
    }

    engine->set_up_position(fen, moves);
}