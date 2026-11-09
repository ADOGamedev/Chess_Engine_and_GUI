#pragma once

#include "HashEntry.h"

// 4.194.304 entries
const uint32_t NUMBER_OF_ENTRIES = 1 << 22;

enum HashEntryFlag {
    EXACT, UPPERBOUND, LOWERBOUND
};

namespace TranspositionTable {

    extern HashEntry table[NUMBER_OF_ENTRIES];

    void add(const HashEntry& entry);
    HashEntry get(const uint64_t& zobrist);
    uint32_t hash(const uint64_t& zobrist);

}