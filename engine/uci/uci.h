#pragma once

#include <iostream>
#include <sstream>
#include <string>

#include "../engine/engine.h"

class UCIReader {

public:
    UCIReader(Engine* engine) :
        engine(engine) {}

    void read_input();

private:
    void read_command_go(std::string command);
    void read_command_position(std::string command);

    Engine* engine;
};