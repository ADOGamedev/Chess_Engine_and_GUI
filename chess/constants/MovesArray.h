#pragma once

#include <cstring>

#include "../move_gen/Move.h"

template <int N>
struct MovesArray {
    static constexpr int MAX_SIZE = N;
    int count = 0;
    Move array[N] = {};

    void push(const Move& new_move) {
        array[count] = new_move;
        count++;
    }

    MovesArray<N> copy() {
        MovesArray<N> new_array;
        new_array.count = count;
        std::memcpy(new_array.array, array, count * sizeof(Move));
        return new_array;
    }

    void insert_back(const Move* from, const Move* to) {
        int added = to - from;
        std::memcpy(array + count, from, added * sizeof(Move));
        count += added;
    }

    const Move* begin() const { return array; }
    const Move* end() const { return array + count; }

    Move* begin() { return array; }
    Move* end() { return array + count; }

};