#pragma once

#include <iostream>

#include "../utils/utils.h"

namespace Logger {

class Logger {
public:
    virtual ~Logger() = default;    virtual void log(const std::string& message) {
        std::cout << message << "\n";
    }
    virtual void log(const char message) {
        std::cout << message << "\n";
    }

};

void set_logger(Logger* logger);
void log(const std::string& message);
void log(const char ch);

}

