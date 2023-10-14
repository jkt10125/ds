#include <bits/stdc++.h>

template <typename T, const T (*func) (T, T)>
class LazySegmentTree {
    int n;
    std::vector<T> tree, lazy;

    inline int MSB(int x) {
        return 31 - __builtin_clz(x);
    }

    inline void UPDATE(int NODE, T VAL, int SIZE) {
        tree[NODE] += VAL;
        if (NODE < n) {
            lazy[NODE] += VAL;
        }
    }

    void push(int NODE, int SIZE) {
        if (lazy[NODE] == T()) {
            return;
        }
        UPDATE(NODE << 1, lazy[NODE], SIZE / 2);
        UPDATE(NODE << 1 | 1, lazy[NODE], SIZE / 2);
        lazy[NODE] = T();
    }

    public:
    LazySegmentTree(int size, T default_value) {
        for (n = 1; n < size; n <<= 1);
        tree.assign(n << 1, default_value);
        lazy.assign(n, T());
    }

    LazySegmentTree(std::vector<T> &A, T default_value) : LazySegmentTree(A.size(), default_value) {
        std::copy(A.begin(), A.end(), tree.begin() + n);
        for (int i = n - 1; i; i--) {
            tree[i] = func(tree[i << 1], tree[i << 1 | 1]);
        }
    }

    void update(int l, int r, T val, int NODE = 1) {
        int LEN = (n >> MSB(NODE));
        int L = (NODE ^ (1 << MSB(NODE))) * LEN;
        int R = L + LEN - 1;

        if (l == L && r == R) {
            UPDATE(NODE, val, LEN);
        }
        else {
            int M = (L + R) >> 1;
            push(NODE, LEN);

            if (l > M) update(l, r, val, NODE << 1 | 1);
            else if (r <= M) update(l, r, val, NODE << 1);
            else {
                update(l, M, val, NODE << 1);
                update(M + 1, r, val, NODE << 1 | 1);
            }
            tree[NODE] = func(tree[NODE << 1], tree[NODE << 1 | 1]);
        }
    }

    T query(int l, int r, int NODE = 1) {
        int LEN = (n >> MSB(NODE));
        int L = (NODE ^ (1 << MSB(NODE))) * LEN;
        int R = L + LEN - 1;

        if (l == L && r == R) {
            return tree[NODE];
        }
        else {
            int M = (L + R) >> 1;
            push(NODE, LEN);
            if (l > M) {
                return query(l, r, NODE << 1 | 1);
            }
            else if (r <= M) {
                return query(l, r, NODE << 1);
            }
            else {
                return func(query(l, M, NODE << 1), query(M + 1, r, NODE << 1 | 1));
            }
        }
    }

    // int findFirst(int x, int NODE = 1) {
    //     int LEN = (n >> MSB(NODE));
    //     int L = (NODE ^ (1 << MSB(NODE))) * LEN;
    //     int R = L + LEN - 1;

    //     if (LEN == 1) {
    //         return L;
    //     }
    //     else {
    //         int M = (L + R) >> 1;
    //         push(NODE, LEN);
    //         ...
    //     }
    // }

    // int findLast(int x, int NODE = 1) {
    //     int LEN = (n >> MSB(NODE));
    //     int L = (NODE ^ (1 << MSB(NODE))) * LEN;
    //     int R = L + LEN - 1;

    //     if (LEN == 1) {
    //         return R;
    //     }
    //     else {
    //         int M = (L + R) >> 1;
    //         push(NODE, LEN);
    //         ...
    //     }
    // }
};