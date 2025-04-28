#ifndef MIDTERM_H
#define MIDTERM_H

#include <cstddef> // size_t 사용을 위해 추가

struct chemical {
    enum class tp { A, B, C, D, E };
    tp type;
    union {
        size_t c_n; // C 프로세스용
        struct { double r; size_t n; } d_params; // D 프로세스용
        struct { size_t b; size_t l; } e_params; // E 프로세스용
    };
};

// 함수 선언 추가
bool next_permutation(chemical* seq, size_t len);
bool solve(chemical* seq, size_t len);
size_t count_type(const chemical* arr, size_t len, chemical::tp type);

#endif // MIDTERM_H
