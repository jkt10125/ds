#include <bits/stdc++.h>

using T = int32_t;
class LazySegTree {
    int h, n;
    std::vector<T> tree, lazy;

    void apply(int p, T value, int k) { tree[p] += value; if (p < n) lazy[p] += value; }

    void calc(int p, int k) { tree[p] = std::max(tree[p << 1], tree[p << 1 | 1]) + lazy[p]; }

    void push(int l, int r) {
        int s = h, k = (1 << (h - 1));
        for (l += n, r += n - 1; s > 0; --s, k >>= 1) {
            for (int i = l >> s; i <= r >> s; ++i) {
                if (lazy[i]) {
                    apply(i << 1, lazy[i], k);
                    apply(i << 1 | 1, lazy[i], k);
                    lazy[i] = 0;
                }
            }
        }
    }

public:
    LazySegTree() { }
    LazySegTree(int n) : n(n), h(std::__lg(n) + 1) { lazy.resize(n); tree.resize(n << 1); }
    
    // range update
    void update(int l, int r, T v) {
        push(l, l + 1); push(r, r + 1);
        bool cl = 0, cr = 0; int k = 1;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1, k <<= 1) {
            if (cl) calc(l - 1, k); if (cr) calc(r, k);
            if (l & 1) apply(l++, v, k), cl = 1;
            if (r & 1) apply(--r, v, k), cr = 1;
        }
        for (--l; r > 0; l >>= 1, r >>= 1, k <<= 1) {
            if (cl) calc(l, k);
            if (cr && (!cl || l != r)) calc(r, k);
        }
    }

    // range query
    T query(int l, int r) {
        push(l, l + 1); push(r, r + 1);
        T res = -1e18;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = std::max(res, tree[l++]);
            if (r & 1) res = std::max(res, tree[--r]);
        }
        return res;
    }
};



int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;
    LazySegTree seg(n);
    while (m--) {
        int t;
        std::cin >> t;
        if (t == 1) {
            int l, r, v;
            std::cin >> l >> r >> v;
            seg.update(l, r - 1, v);
        }
        else {
            int l, r;
            std::cin >> l >> r;
            std::cout << seg.query(l, r - 1) << '\n';
        }
    }
    return 0;
}