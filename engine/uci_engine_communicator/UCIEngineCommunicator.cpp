#include "UCIEngineCommunicator.h"
    
void UCIEngineCommunicator::start_uci_engine(const std::string& path) {
    end_process();

    path_to_executable = path;

    create_pipes();
    configure_startup_info();
    start_process();

    send_command("uci");
    if (pipe_reader.read_and_find_substring("uciok").empty()) {
        throw EngineInitializationException();
    }
    
    send_command("isready");
    if (pipe_reader.read_and_find_substring("readyok").empty()) {
        throw EngineInitializationException();
    }
}

void UCIEngineCommunicator::end_process() {
    send_command("quit");

    CloseHandle(in_write);
    CloseHandle(out_read);
    CloseHandle(process_information.hProcess);
    CloseHandle(process_information.hThread);
}

std::string UCIEngineCommunicator::find_best_move(const TimeStruct& time_struct) {
    send_command(std::format(
        "go wtime {} btime {} winc {} binc {}", 
        time_struct.wtime, time_struct.btime, time_struct.winc, time_struct.binc
    ));

    std::jthread stop_thread([this](std::stop_token st) {
        if (!st.stop_requested()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        }
        if (!st.stop_requested()) {
            send_command("stop");
        }
    });

    std::string best_move = pipe_reader.read_and_find_substring("bestmove");

    stop_thread.request_stop();
    
    if (best_move.empty()) {
        throw BestMoveNotFoundException();
    }

    std::size_t move_start = std::string("bestmove ").size();

    if (best_move.size() < move_start + 4) {
        throw BestMoveNotFoundException();
    }
    
    std::size_t move_end = best_move[move_start + 4] == ' ' ? 4 : 5;

    return best_move.substr(move_start, move_end);
}

void UCIEngineCommunicator::send_command(const std::string& command) {
    try {
        DWORD written;
        std::string cmd = command + "\n";
        WriteFile(in_write, cmd.c_str(), cmd.size(), &written, NULL);
    } catch (std::exception& e) {
        throw MissingCommand(command);
    }
}

void UCIEngineCommunicator::create_pipes() {
    CreatePipe(&out_read, &out_write, &security_attributes, 0);
    CreatePipe(&in_read, &in_write, &security_attributes, 0);
}

void UCIEngineCommunicator::configure_startup_info() {
    startup_info.cb = sizeof(startup_info);
    startup_info.dwFlags = STARTF_USESTDHANDLES;
    startup_info.hStdInput = in_read;
    startup_info.hStdOutput = out_write;
    startup_info.hStdError = out_write;
}

void UCIEngineCommunicator::start_process() {
    if (!CreateProcessA(
        NULL,
        (LPSTR)path_to_executable.c_str(),
        NULL, NULL,
        TRUE,
        CREATE_NO_WINDOW,
        NULL, NULL,
        &startup_info, &process_information
    )) {
        DWORD err = GetLastError();
        throw ProcessCreationException(std::to_string(err));
    }

    CloseHandle(out_write);
    CloseHandle(in_read);
}




