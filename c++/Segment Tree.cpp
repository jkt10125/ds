#include <bits/stdc++.h>

using T = int32_t;

class SegTree : std::vector<T> {
    int n;
public:
    SegTree(int __n, bool k = 0) {
        n = (k) ? (1 << (32 - __builtin_clz(__n - 1))) : __n;
        resize(n << 1);
    }

    SegTree(std::vector<T> &A, bool k = 0) {
        n = (k) ? (1 << (32 - __builtin_clz(A.size() - 1))) : A.size();
        resize(n << 1); std::copy(A.begin(), A.end(), begin() + n);
        for (int i = n - 1; i; i--) {
            (*this)[i] = std::min((*this)[i << 1], (*this)[i << 1 | 1]);
        }
    }

    // point update
    void update(int i, T v) {
        for ((*this)[i += n] = v; i > 1; i >>= 1) {
            (*this)[i >> 1] = std::min((*this)[i], (*this)[i ^ 1]);
        }
    }

    // range query
    T query(int l, int r) {
        T res = INT_MAX;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = std::min(res, (*this)[l++]);
            if (r & 1) res = std::min(res, (*this)[--r]);
        }
        return res;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    return 0;
}