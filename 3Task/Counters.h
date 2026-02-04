#pragma once
#include <cstddef>

struct Counters {
    size_t empty = 0;
    size_t comment = 0;
    size_t code = 0;
    size_t files = 0;

    void merge(const Counters& other) {
        empty += other.empty;
        comment += other.comment;
        code += other.code;
        files += other.files;
    }
};
