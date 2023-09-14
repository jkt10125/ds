#include <bits/stdc++.h>

using T = int32_t;

T f(T a, T b);

class SparseTable : std::vector<std::vector<T>> {
public:
    SparseTable(const std::vector<T> &A) {
        int n = A.size(), log = 32 - __builtin_clz(n);
        resize(log, std::vector<T> (n));
        std::copy(A.begin(), A.end(), (*this)[0].begin());
        for (int i = 1; i < log; i++) {
            for (int j = 0; j + (1 << i) <= n; j++) {
                (*this)[i][j] = f((*this)[i - 1][j], (*this)[i - 1][j + (1 << (i - 1))]);
            }
        }
    }

    T query(int l, int r) {
        int log = 31 - __builtin_clz(r - l + 1);
        return f((*this)[log][l], (*this)[log][r - (1 << log) + 1]);
    }
};