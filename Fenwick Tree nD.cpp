#include <bits/stdc++.h>

template <const int n, typename T>
struct FenwickTree : std::vector<T> {
    std::array<int, n> dim;

    int getIdx(std::array<int, n> &idx) {
        int ans = 0;
        for (int i = 0; i < n; ++i) { ans = ans * dim[i] + idx[i] - 1; }
        return ans;
    }

    void nestedloopAdd(std::array<int, n> &i, const std::array<int, n> &idx, int cd, T val) {
        if (cd == n) { (*this)[getIdx(i)] += val; return; }
        for (i[cd] = idx[cd] + 1; i[cd] <= dim[cd]; i[cd] += (i[cd] & -i[cd])) {
            nestedloopAdd(i, idx, cd + 1, val);
        }
    }
    
    void nestedLoopSum(std::array<int, n> &i, const std::array<int, n> &idx, int cd, T &ans) {
        if (cd == n) { ans += (*this)[getIdx(i)]; return; }
        for (i[cd] = idx[cd] + 1; i[cd] > 0; i[cd] -= (i[cd] & -i[cd])) {
            nestedLoopSum(i, idx, cd + 1, ans);
        }
    }

public:
    FenwickTree(std::array<int, n> dim) : dim(dim) {
        this->assign(std::accumulate(dim.begin(), dim.end(), 1, std::multiplies<int>()), T());
    }

    void add(const std::array<int, n> &idx, T v) {
        std::array<int, n> i;
        nestedloopAdd(i, idx, 0, v);
    }

    T prefixSum(const std::array<int, n> &idx) {
        T ans = 0; std::array<int, n> i;
        nestedLoopSum(i, idx, 0, ans);
        return ans;
    }

    T rangeSum(const std::array<int, n> &idx1, const std::array<int, n> &idx2) { // 2^n x (prod(log(dim_i)) + n)
        std::array<int, n> idx; T ans = T();
        for (int i = 0; i < (1 << n); i++) {
            for (int j = 0; j < n; j++) { idx[j] = ((i >> j) & 1) ? idx1[j] - 1 : idx2[j]; }
            if (__builtin_popcount(i) & 1) { ans -= prefixSum(idx); }
            else { ans += prefixSum(idx); }
        }
        return ans;
    }
};

int main() {
    FenwickTree<1, long long> ft({5});
    ft.add({0}, 1);
    ft.add({0}, 2);
    ft.add({1}, 3);
    ft.add({2}, 4);

    std::cout << ft.prefixSum({0}) << std::endl;
}