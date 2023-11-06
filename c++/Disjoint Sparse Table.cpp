#include <bits/stdc++.h>

using T = int32_t;

T f(T a, T b);

class DisjointSparseTable : std::vector<std::vector<T>> {
public:
    DisjointSparseTable(std::vector<T> &A) {
        int log = 32 - __builtin_clz(A.size() - 1), n = (1 << log);
        resize(log + 1, std::vector<T> (n, T())); A.resize(n, T());
        std::copy(A.begin(), A.end(), (*this)[0].begin());
        for (int h = 1, range; (range = (1 << h)) <= n; h++) {
            int half = range >> 1;
            for (int i = half; i < n; i += range) {
                (*this)[h][i - 1] = A[i - 1];
                for (int j = i - 2; j >= i - half; j--) {
                    (*this)[h][j] = f((*this)[h][j + 1], A[j]);
                }
                (*this)[h][i] = A[i];
                for (int j = i + 1; j < i + half; j++) {
                    (*this)[h][j] = f((*this)[h][j - 1], A[j]);
                }
            }
        }
    }

    T query(int l, int r) {
        if (l == r) return (*this)[0][l];
        int h = 32 - __builtin_clz(l ^ r);
        return f((*this)[h][l], (*this)[h][r]);
    }
};

int main() {
    int n, q;
    std::cin >> n >> q;
    std::vector<int> A(n);
    for (int &a : A) std::cin >> a;
    DisjointSparseTable dst(A);
    while (q--) {
        int l, r;
        std::cin >> l >> r;
        std::cerr << dst.query(l, r) << '\n';
    }
}