#define SEG_TREE_NODES (1 << 20)

typedef struct segtree_tag_type {

} segtree_tag_type;

typedef struct segtree_info_type {

} segtree_info_type;

segtree_tag_type segtree_tag[2 * SEG_TREE_NODES];
segtree_info_type segtree_info[2 * SEG_TREE_NODES];

// Initializes segment tree tag to default value
void segtree_tag_init(int node) {

}

// Applies the tag to the segment tree node
void segtree_tag_apply_impl(int node, segtree_tag_type tag) {

}

// for comparing with default value
int segtree_tag_zeq_impl(int node) {

}

// Applies the tag to the segment tree info
void segtree_info_apply_impl(int node, segtree_tag_type tag, int size) {

}

// Merges two segment tree info
segtree_info_type segtree_info_combine_impl(segtree_info_type lhs, segtree_info_type rhs) {

}

/*******************************************************************************************/

// predicate function to find first/last node satisfying some condition
int (*segtree_node_pred)(int);

int segtree_length; // Segment Tree default Length. Must be initialized as power of 2

void segtree_apply_impl(int node, segtree_tag_type tag, int size) {
    segtree_tag_apply_impl(node, tag);
    segtree_info_apply_impl(node, tag, size);
}

void segtree_push_impl(int node, int size) {
    if (segtree_tag_zeq_impl(node)) {
        return;
    }
    segtree_apply_impl(2 * node, segtree_tag[node], size / 2);
    segtree_apply_impl(2 * node + 1, segtree_tag[node], size / 2);
    segtree_tag_init(node);
}

void segtree_pull_impl(int node) {
    segtree_info[node] = segtree_info_combine_impl(
        segtree_info[2 * node], 
        segtree_info[2 * node + 1]
    );
}

void segtree_point_update_impl(int idx, segtree_info_type info, int node, int l, int r) {
    if (l == r) {
        segtree_info[node] = info;
        return;
    }
    int m = (l + r) / 2;
    segtree_push_impl(node, r - l + 1);
    if (m < idx) {
        segtree_point_update_impl(idx, info, 2 * node + 1, m + 1, r);
    } else {
        segtree_point_update_impl(idx, info, 2 * node, l, m);
    }
    segtree_pull_impl(node);
}

void segtree_range_update_impl(int L, int R, segtree_tag_type tag, int node, int l, int r) {
    if (r < L || R < l) {
        return;
    } else if (L <= l && r <= R) {
        segtree_apply_impl(node, tag, r - l + 1);
        return;
    }
    int m = (l + r) / 2;
    segtree_push_impl(node, r - l + 1);
    segtree_range_update_impl(L, R, tag, 2 * node, l, m);
    segtree_range_update_impl(L, R, tag, 2 * node + 1, m + 1, r);
    segtree_pull_impl(node);
}

segtree_info_type segtree_range_query_impl(int L, int R, int node, int l, int r) {
    if (r < L || R < l) {
        return segtree_info[0]; // segtree_info_default
    } else if (L <= l && r <= R) {
        return segtree_info[node];
    }
    int m = (l + r) / 2;
    segtree_push_impl(node, r - l + 1);
    return segtree_info_combine_impl(
        segtree_range_query_impl(L, R, 2 * node, l, m), 
        segtree_range_query_impl(L, R, 2 * node + 1, m + 1, r)
    );
}

// b: 0 for first, 1 for last
int segtree_find_impl(int L, int R, int node, int l, int r, int b) {
    if (r < L || R < l || !segtree_node_pred(node)) {
        return -1;
    } else if (l == r) {
        return l;
    }
    int m = (l + r) / 2;
    segtree_push_impl(node, r - l + 1);
    int res = segtree_find_impl(L, R, 2 * node + b, !b * l + b * (m + 1), !b * m + b * r, b);
    if (res == -1) {
        res = segtree_find_impl(L, R, 2 * node + !b, b * l + !b * (m + 1), b * m + !b * r, b);
    }
    return res;
}

// int segtree_find_kth_impl(int L, int R, int k, int node, int l, int r) {
//     if (r < L || R < l || segtree_info[node].cnt < k) return -1;
//     if (l == r) return l;
//     int m = (l + r) / 2;
//     segtree_push_impl(node, r - l + 1);
//     int res = segtree_find_kth_impl(L, R, k, 2 * node, l, m);
//     if (res == -1) {
//         res = segtree_find_kth_impl(L, R, k - segtree_info[2 * node].cnt, 2 * node + 1, m + 1, r);
//     }
//     return res;
// }


void segtree_point_update(int idx, segtree_info_type info) {
    segtree_point_update_impl(idx, info, 1, 0, segtree_length - 1);
}

void segtree_range_update(int L, int R, segtree_tag_type tag) {
    segtree_range_update_impl(L, R, tag, 1, 0, segtree_length - 1);
}

segtree_info_type segtree_range_query(int L, int R) {
    return segtree_range_query_impl(L, R, 1, 0, segtree_length - 1);
}

int segtree_find_first(int L, int R, int (*pred)(int)) {
    segtree_node_pred = pred;
    return segtree_find_impl(L, R, 1, 0, segtree_length - 1, 0);
}

int segtree_find_last(int L, int R, int (*pred)(int)) {
    segtree_node_pred = pred;
    return segtree_find_impl(L, R, 1, 0, segtree_length - 1, 1);
}

// int segtree_find_kth(int L, int R, int k) {
//     return segtree_find_kth_impl(L, R, k, 1, 0, segtree_length - 1);
// }

/*******************************************************************************************/

// Segment Tree default Functions
void segtree_default_init(int n) {
    segtree_length = 1;
    while (n > segtree_length) { segtree_length <<= 1; }
    // ...
}

int main() {
    segtree_default_init(SEG_TREE_NODES);
    return 0;
}