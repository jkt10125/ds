#include <stdio.h>

#define SEG_TREE_NODES (1 << 20)
#define INT_MAX 0x7fffffff
#define INT_MIN 0x80000000

typedef int cmp_type;

int i32_min(int a, int b) { return a < b ? a : b; }
int i32_max(int a, int b) { return a > b ? a : b; }

long long i64_min(long long a, long long b) { return a < b ? a : b; }
long long i64_max(long long a, long long b) { return a > b ? a : b; }

typedef struct segtree_tag_type {
    int val;
} segtree_tag_type;

typedef struct segtree_info_type {
    int max_val, second_max_val, max_cnt;
    long long sum;
} segtree_info_type;

segtree_tag_type segtree_tag[2 * SEG_TREE_NODES];
segtree_info_type segtree_info[2 * SEG_TREE_NODES];

// Initializes segment tree tag to default value
void segtree_tag_init(int node) {
    segtree_tag[node] = segtree_tag[0];
}

// Applies the tag to the segment tree node
void segtree_tag_apply_impl(int node, segtree_tag_type tag) {
    segtree_tag[node].val = i32_min(segtree_tag[node].val, tag.val);
}

// for comparing with default value
int segtree_tag_zeq_impl(int node) {
    segtree_tag[node].val == segtree_tag[0].val;
}

// Applies the tag to the segment tree info
void segtree_info_apply_impl(int node, segtree_tag_type tag) {
    if (segtree_info[node].second_max_val < tag.val && tag.val < segtree_info[node].max_val) {
        segtree_info[node].sum -= (long long) (segtree_info[node].max_val - tag.val) * segtree_info[node].max_cnt;
        segtree_info[node].max_val = tag.val;
    }
}

// Merges two segment tree info
segtree_info_type segtree_info_combine_impl(segtree_info_type lhs, segtree_info_type rhs) {
    segtree_info_type res;
    if (lhs.max_val > rhs.max_val) {
        res.max_val = lhs.max_val;
        res.max_cnt = lhs.max_cnt;
        res.second_max_val = i32_max(lhs.second_max_val, rhs.max_val);
    } else if (lhs.max_val < rhs.max_val) {
        res.max_val = rhs.max_val;
        res.max_cnt = rhs.max_cnt;
        res.second_max_val = i32_max(rhs.second_max_val, lhs.max_val);
    } else {
        res.max_val = lhs.max_val;
        res.max_cnt = lhs.max_cnt + rhs.max_cnt;
        res.second_max_val = i32_max(lhs.second_max_val, rhs.second_max_val);
    }
    res.sum = lhs.sum + rhs.sum;
    return res;
}

void segtree_apply_impl(int node, segtree_tag_type tag) {
    segtree_tag_apply_impl(node, tag);
    segtree_info_apply_impl(node, tag);
}

void segtree_push_impl(int node) {
    if (segtree_tag_zeq_impl(node)) {
        return;
    }
    segtree_apply_impl(2 * node, segtree_tag[node]);
    segtree_apply_impl(2 * node + 1, segtree_tag[node]);
    segtree_tag_init(node);
}

void segtree_pull_impl(int node) {
    segtree_info[node] = segtree_info_combine_impl(segtree_info[2 * node], segtree_info[2 * node + 1]);
}

void segtree_point_update_impl(int idx, segtree_info_type info, int node, int l, int r) {
    if (l == r) {
        segtree_info[node] = info;
        return;
    }
    int m = (l + r) / 2;
    segtree_push_impl(node);
    if (m < idx) {
        segtree_point_update_impl(idx, info, 2 * node + 1, m + 1, r);
    } else {
        segtree_point_update_impl(idx, info, 2 * node, l, m);
    }
    segtree_pull_impl(node);
}

void segtree_range_update_impl(int L, int R, segtree_tag_type tag, int node, int l, int r) {
    if (r < L || R < l || segtree_info[node].max_val <= tag.val) {
        return;
    } else if (L <= l && r <= R && segtree_info[node].second_max_val < tag.val) {
        segtree_apply_impl(node, tag);
        return;
    }
    int m = (l + r) / 2;
    segtree_push_impl(node);
    segtree_range_update_impl(L, R, tag, 2 * node, l, m);
    segtree_range_update_impl(L, R, tag, 2 * node + 1, m + 1, r);
    segtree_pull_impl(node);
}

segtree_info_type segtree_range_query_impl(int L, int R, int node, int l, int r) {
    if (r < L || R < l) {
        return segtree_info[0];
    } else if (L <= l && r <= R) {
        return segtree_info[node];

    }
    int m = (l + r) / 2;
    segtree_push_impl(node);
    return segtree_info_combine_impl(segtree_range_query_impl(L, R, 2 * node, l, m), segtree_range_query_impl(L, R, 2 * node + 1, m + 1, r));
}

// Segment Tree default Functions
void segtree_default_init() {
    segtree_tag[0].val = INT_MAX;
    segtree_info[0].max_val = segtree_info[0].second_max_val = INT_MIN;
    segtree_info[0].sum = segtree_info[0].max_cnt = 0;
    for (int i = 1; i < 2 * SEG_TREE_NODES; i++) {
        segtree_tag[i] = segtree_tag[0];
        segtree_info[i] = segtree_info[0];
    }
}

void segtree_point_update(int idx, segtree_info_type info) {
    segtree_point_update_impl(idx, info, 1, 0, SEG_TREE_NODES - 1);
}

void segtree_range_update(int L, int R, segtree_tag_type tag) {
    segtree_range_update_impl(L, R, tag, 1, 0, SEG_TREE_NODES - 1);
}

segtree_info_type segtree_range_query(int L, int R) {
    return segtree_range_query_impl(L, R, 1, 0, SEG_TREE_NODES - 1);
}

void segtree_print(int n) {
    for (int i = 0; i < n; i++) {
        printf("%lld ", segtree_range_query(i, i).sum);
    }
    printf("\n");
}

int A[SEG_TREE_NODES], H[SEG_TREE_NODES];

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        int n, mex = 0;
        scanf("%d", &n);
        segtree_default_init(n); 
        for (int i = 0; i <= n; i++) H[i] = 0;
        segtree_info_type info = segtree_info[0]; info.max_cnt = 1;
        segtree_tag_type tag = segtree_tag[0];
        for (int i = 0; i < n; i++) {
            scanf("%d", &A[i]);
            H[A[i]] = 1;
            while (H[mex]) mex++;
            info.sum = info.max_val = mex;
            segtree_point_update(i, info);
        }
        long long ans = segtree_range_query(0, n - 1).sum;
        printf("##$ ");
        segtree_print(n);

        for (int i = 0; i < n; i++) {
            tag.val = A[i];
            info.sum = info.max_val = n;
            segtree_range_update(0, n - 1, tag);
            segtree_point_update(i, info);
            long long sum = segtree_range_query(0, n - 1).sum;
            ans = ans > sum ? ans : sum;
            printf("##$ ");

            segtree_print(n);
        }

        printf("%lld\n", ans);
    }
    return 0;
}