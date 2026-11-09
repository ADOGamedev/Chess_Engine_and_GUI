#include "transposition_table.h"

namespace TranspositionTable {

    HashEntry table[NUMBER_OF_ENTRIES];

    void add(const HashEntry& entry) {
        uint32_t index = hash(entry.zobrist);
        table[index] = entry;
    }

    HashEntry get(const uint64_t& zobrist) {
        return table[hash(zobrist)];
    }

    uint32_t hash(const uint64_t& zobrist) {
        return zobrist & (NUMBER_OF_ENTRIES - 1);
    }

}