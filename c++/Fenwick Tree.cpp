#include <bits/stdc++.h>

// Point Update and Range Sum
using T = int32_t;
struct FenwickTree : std::vector<T> {
    FenwickTree() = default;
    
    FenwickTree(int n) {
        assign(n, T());
    }
    
    void add(int x, T v) {
        for (++x; x <= size(); x += x & -x) {
            (*this)[x - 1] += v;
        }
    }

    T prefixSum(int x) {
        T ans = T();
        for (++x; x; x -= x & -x) {
            ans += (*this)[x - 1];
        }
        return ans;
    }

    T rangeSum(int l, int r) {
        return prefixSum(r) - prefixSum(l - 1);
    }
};

// Range Update and Range Sum
using T = int32_t;
class FenwickTree : std::array<std::vector<T>, 2> {
    
    void update(int i, T v) {
        for (int j = i + 1; j <= (int)(*this)[0].size(); j += j & -j) {
            
            (*this)[0][j - 1] += v;
            (*this)[1][j - 1] += v * i;
        }
    }

    T prefixSum(int i) {
        T s0 = T(), s1 = T();
        
        for (int j = i + 1; j; j -= j & -j) {
            
            s0 += (*this)[0][j - 1];
            s1 += (*this)[1][j - 1];
        }

        return s0 * i - s1;
    }

public:
    FenwickTree() = default;
    FenwickTree(int n) {
        
        (*this)[0] = (*this)[1] = std::vector<T>(n, 0);
    }
    void rangeUpdate(int l, int r, T val) {
        
        update(l, val);
        update(r + 1, -val);
    }
    T rangeSum(int l, int r) {
        return prefixSum(r + 1) - prefixSum(l);
    }
};

// fenwick tree in 2D
struct FenwickTree2D : std::vector<std::vector<T>> {
    FenwickTree2D(int n, int m) { assign(n, std::vector<T>(m, T())); }

    void add(int x, int y, T v) {
        for (int i = x + 1; i <= size(); i += (i & -i)) {
            for (int j = y + 1; j <= (*this)[i - 1].size(); j += (j & -j)) {
                (*this)[i - 1][j - 1] += v;
            }
        }
    }

    T prefixSum(int x, int y) {
        T sum = T();
        for (int i = x + 1; i; i -= (i & -i)) {
            for (int j = y + 1; j; j -= (j & -j)) {
                sum += (*this)[i - 1][j - 1];
            }
        }
        return sum;
    }

    T rangeSum(int x1, int y1, int x2, int y2) {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        return prefixSum(x2, y2) - prefixSum(x1 - 1, y2) - prefixSum(x2, y1 - 1) + prefixSum(x1 - 1, y1 - 1);
    }
};

int main() {
    FenwickTree ft[26];
    for (int i = 0; i < 26; i++) { ft[i] = FenwickTree(100); }
}
