#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <chrono>
#include <thread>
#include <format>

#include "delay.h"
#include "PipeReader.h"
#include "../../chess/logger/Logger.h"

class UCIEngineCommunicator {
public:
    UCIEngineCommunicator() :
    pipe_reader(&out_read) {}
    
    void start_uci_engine(const std::string& path);
    void end_process();
    
    void send_command(const std::string& command);
    std::string find_best_move(const TimeStruct& time_struct);
    
    private:
    void create_pipes();
    void configure_startup_info();
    void start_process();
    
    std::string path_to_executable;
    
    PipeReader pipe_reader;
    
    HANDLE in_read, in_write;
    HANDLE out_read, out_write;
    
    char read_buffer[4096];
    
    SECURITY_ATTRIBUTES security_attributes{sizeof(security_attributes), NULL, TRUE};
    STARTUPINFOA startup_info{};
    PROCESS_INFORMATION process_information{};
};

class UCIEngineException : public std::exception {
    
    private:
    std::string message;
    
    public:
    UCIEngineException(const std::string msg) 
    : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class ProcessCreationException : public UCIEngineException {
    public:
    ProcessCreationException(const std::string& error)
    : UCIEngineException("An error ocurred while starting the AI (note: check in settings menu). Error code: " + error) {};
};

class MissingCommand : public UCIEngineException {
    
    public:
    MissingCommand(const std::string& missing_command)
    : UCIEngineException("Command \"" + missing_command + "\" is missing!") {};
};

class BestMoveNotFoundException : public UCIEngineException {
    
    public:
    BestMoveNotFoundException()
    : UCIEngineException("A best move could not be found!") {};
};

class EngineInitializationException : public UCIEngineException {
    
    public:
    EngineInitializationException()
    : UCIEngineException("Engine failed to response to \"uci\" or \"isready\" commands") {};
};
