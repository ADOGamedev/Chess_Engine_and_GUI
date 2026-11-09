#pragma once

struct TimeStruct {
    TimeStruct() = default;
    TimeStruct(const int& wtime, const int& btime, const int& winc, const int& binc) :
        wtime(wtime), btime(btime), winc(winc), binc(binc) {}

    bool is_valid() {
        return wtime >= 0 && btime >= 0 && winc >= 0 && binc >= 0;
    }

    int wtime = -1;
    int btime = -1;
    int winc = -1;
    int binc = -1;
};