#include "PipeReader.h"

std::string PipeReader::read_and_find_substring(const std::string& substr) {
    bytes_read = 0ULL;
    output = "";

    reading_start_time = std::chrono::steady_clock::now();

    while (true) {
        if (read_timed_out()) {
            Logger::log("Timed out");
            break;
        }

        if (!is_there_any_data_on_read_pipe()) {
            delay(10);
            continue;
        }

        if (!ReadFile(*read_pipe, read_buffer, sizeof(read_buffer) - 1, &bytes_read, NULL) || bytes_read == 0) {
            Logger::log("Nothing to read");
            break;
        }

        read_buffer[bytes_read] = '\0';
        output += read_buffer; 
        
        std::string line = find_substring_in_output(substr);

        if (!line.empty()) {
            return line;
        }
    }
    
    return "";
}

bool PipeReader::read_timed_out() {
    auto current_time = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - reading_start_time).count();
        
    return elapsed > TIMEOUT_MS;
}

bool PipeReader::is_there_any_data_on_read_pipe() {
    DWORD bytes_available = 0;
    BOOL peek_result = PeekNamedPipe(*read_pipe, NULL, 0, NULL, &bytes_available, NULL);

    return peek_result && bytes_available != 0;
}

std::string PipeReader::find_substring_in_output(const std::string& substr) {
    std::size_t start_pos = output.find(substr);

    if (start_pos != std::string::npos) {
        std::size_t end_pos = output.find_first_of("\r\n", start_pos);

        if (end_pos == std::string::npos) {
            return "";
        }

        return output.substr(start_pos, end_pos - start_pos);
    }

    return "";
}