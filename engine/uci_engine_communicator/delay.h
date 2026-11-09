#pragma once

#include <chrono>
#include <thread>

inline void delay(const int wait_time) {
    std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));
}