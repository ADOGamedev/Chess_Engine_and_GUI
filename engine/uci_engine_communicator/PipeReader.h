#pragma once

#include <chrono>
#include <string>
#include <windows.h>

#include "delay.h"
#include "../../chess/logger/Logger.h"

typedef std::chrono::steady_clock::time_point time_point;

class PipeReader {
    public: 
    PipeReader(HANDLE* pipe) :
    read_pipe(pipe) {}
    
    std::string read_and_find_substring(const std::string& substr);
    
    private:
    bool read_timed_out();
    bool is_there_any_data_on_read_pipe();
    std::string find_substring_in_output(const std::string& substr);
    
    HANDLE* read_pipe;
    
    char read_buffer[4096];
    
    DWORD bytes_read;
    std::string output;
    
    const int TIMEOUT_MS = 2500; 
    time_point reading_start_time;
};