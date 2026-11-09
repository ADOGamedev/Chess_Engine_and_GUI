#include "Logger.h"

namespace Logger {

Logger* active_logger = nullptr;

void set_logger(Logger* logger) {
    active_logger = logger;
} 

void log(const std::string& message) {
    if (active_logger) {
        active_logger->log(message);
    }
}

void log(const char ch) {
    if (active_logger) {
        active_logger->log(ch);
    }
}

}