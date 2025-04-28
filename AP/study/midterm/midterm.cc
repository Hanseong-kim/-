#include <iostream>
using namespace std;


struct chemical {
    enum class tp { A, B, C, D, E } disc;
    union {
        size_t c_len; // C 프로세스용
        struct { double ratio; size_t max_len; } d_info; // D 프로세스용
        struct { size_t start; size_t len; } e_info; // E 프로세스용
    };
};

size_t count_type(chemical const* cs, size_t n, chemical::tp t) {
    size_t c = 0;
    for (size_t i = 0; i < n; ++i) {
        c += cs[i].disc == t;
    }
    return c;
}


static bool verify(chemical const* cs, size_t n, size_t preceding = 0) {
    if (n == 0) return true;
    switch (cs[0].disc) {
        case chemical::tp::A:
            return preceding == 0 || (cs[-1].disc != chemical::tp::A
                && verify(cs + 1, n - 1, preceding + 1));
        case chemical::tp::B:
            return verify(cs + 1, n - 1, preceding + 1);
        case chemical::tp::C:
            return preceding >= cs->c_len
                && 2 * cs->c_len <= preceding
                && count_type(cs - cs->c_len, cs->c_len, chemical::tp::B) ==
                   count_type(cs - 2 * cs->c_len, cs->c_len, chemical::tp::B)
                && verify(cs + 1, n - 1, preceding + 1);
        case chemical::tp::D:
            for (size_t i = 1; i <= preceding && i <= cs->d_info.max_len; i++) {
                if ((double)count_type(cs - 1, i, chemical::tp::A) / i >= cs->d_info.ratio)
                    return verify(cs + 1, n - 1, preceding + 1);
            }
            return false;
        case chemical::tp::E:
            if (cs->e_info.start > preceding || cs->e_info.len > cs->e_info.start)
                return false;
            return verify(cs - cs->e_info.start, cs->e_info.len)
                && verify(cs + 1, n - 1, preceding + 1);
    }
}

long solve(chemical const* cs, size_t n, chemical* out, size_t preceding) {
    if (n == 0) return verify(out - preceding, preceding);

    for (size_t i = 0; i < n; i++) {
        chemical cs_copy[100];
        for (size_t j = 0; j < n; j++)
            cs_copy[j] = cs[j];

        // swap cs_copy[i] with cs_copy[0]
        chemical t = cs_copy[0];
        cs_copy[0] = cs_copy[i];
        cs_copy[i] = t;

        long s = solve(cs + 1, n - 1, out + 1, preceding + 1);
        if (s > 0) {
            out[0] = cs_copy[0];
            num_solutions += s;
        }
    }
    return num_solutions;
}


bool solve(chemical const* cs, size_t n, chemical* out, size_t preceding){
    if (n == 0) return verify(out - preceding, preceding);
    for (size_t i = 0; i < n; ++i) {
       chemical cs_copy[100];
       for (size_t j = 0; j < n; ++j) {
            cs_copy[j] = cs[j];
        }
        chemical t = cs_copy[0];
        cs_copy[0] = cs_copy[i];
        cs_copy[i] = t;
        if (solve(cs + 1, n - 1, out + 1, preceding + 1)){
            return true;
        }
    }
    return false;       
}


bool solve(chemical* cs, size_t n){
    chemical solution[100];
    if (solve(cs, n, solution, 0)){
        for (size_t i = 0; i < n; i++)
            cs[i] = solution[i];
        return true;
    }
    else
        return false;
}


int main(){
    size_t n;
    while(1){
        cin >> n;
        if (cin && n == 0) break;
        cout << "Try again" << endl;
        cin.clear();   
        cin.ignore(10000, '\n');
    }
    chemical cs[3] = {
        { chemical::tp::A }
        { chemical::tp::A },
        { chemical::tp::B }};

        
}