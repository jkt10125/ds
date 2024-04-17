#define FENWICK_SIZE 100007

typedef int fenwick_type;

fenwick_type fenwick_tree[FENWICK_SIZE][2];

void fenwick_update_impl(int idx, fenwick_type val) {
    for (int i = idx + 1; i <= FENWICK_SIZE; i += i & -i) {
        fenwick_tree[i - 1][0] += val;
        fenwick_tree[i - 1][1] += val * idx;
    }
}

fenwick_type fenwick_prefix_sum_impl(int idx) {
    fenwick_type s0 = 0, s1 = 0;
    for (int i = idx + 1; i; i -= i & -i) {
        s0 += fenwick_tree[i - 1][0];
        s1 += fenwick_tree[i - 1][1];
    }
    return s0 * idx - s1;
}

void fenwick_range_update(int l, int r, fenwick_type val) {
    fenwick_update_impl(l, val);
    fenwick_update_impl(r + 1, -val);
}

fenwick_type fenwick_range_sum(int l, int r) {
    return fenwick_prefix_sum_impl(r + 1) - fenwick_prefix_sum_impl(l);
}

