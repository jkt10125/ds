typedef int       i32;  typedef unsigned int       u32;
typedef long long i64;  typedef unsigned long long u64;

const int btrie_DEPTH = 31;
const int btrie_pINF = 0xffffffff;
const int btrie_mINF = 0x00000000;

#define TRIE_NODES 1000000

typedef int key_type;
typedef int value_type;

typedef struct btrie_node {
    int c[2], cnt, par;
    value_type value;
} btrie_node;

btrie_node btrie[TRIE_NODES];
int delete[TRIE_NODES];
int btrie_node_ctr = 1, btrie_del_ctr = 0;

void btrie_init() {
    btrie[0].par = -1;
    btrie[0].c[0] = btrie[0].c[1] = btrie[0].cnt = 0;
    btrie_node_ctr = 1, btrie_del_ctr = 0;
}

int btrie_new_node_impl() {
    int idx = (btrie_del_ctr) ? delete[--btrie_del_ctr] : btrie_node_ctr++;
    // Check for memory error
    // if (btrie_node_ctr == TRIE_NODES) [[unlikely]] {
    //     return -1;
    // }
    btrie[idx].c[0] = btrie[idx].c[1] = btrie[idx].cnt = 0;
    return idx; 
}

// Go up to the parent node
void btrie_go_up_impl(int *p, key_type *x) {
    (*p) = btrie[*p].par;
    (*x) >>= 1;
}

// Go down to the child depending on the bit b
void btrie_go_down_impl(int *p, key_type *x, int b) {
    (*p) = btrie[*p].c[b];
    (*x) = (*x) << 1 | b;
}

// Returns 1 if the function correctly finds the greater element
// b: 1 for gt, 0 for lt
int btrie_find_impl(int p, key_type *x, int up, int b) {
    if (up) {
        while (!((b ^ (*x) & 1) && btrie[btrie[p].par].c[b])) {
            btrie_go_up_impl(&p, x);
            if (p == -1) { return 0; }
        }
        btrie_go_up_impl(&p, x);
        btrie_go_down_impl(&p, x, b);
    }
    while (btrie[p].c[0] | btrie[p].c[1]) {
        btrie_go_down_impl(&p, x, b ^ (btrie[p].c[1 ^ b] != 0));
    }
    return 1;
}

// Get the node corresponding to the key
int btrie_get_node(key_type key) {
    key ^= (1 << btrie_DEPTH);
    int p = 0;
    for (int i = btrie_DEPTH; i >= 0; --i) {
        int b = (key >> i) & 1;
        if (!btrie[p].c[b]) {
            btrie[p].c[b] = btrie_new_node_impl();
            // Check for memory error
            // if (btrie[p].c[b] == -1) [[unlikely]] {
            //     return NULL;
            // }
            btrie[btrie[p].c[b]].par = p; 
        }
        p = btrie[p].c[b];
    }
    if (!btrie[p].cnt) {
        for (int tp = p; tp != -1; tp = btrie[tp].par) {
            btrie[tp].cnt++;
        }
        btrie[p].value = 0;
    }
    return p;
}

// btrie_erase(element, count, btrie_DEPTH, 0)
// returns 1 if the element was successfully erased
int btrie_erase(key_type key, int i, int p) {
    key ^= 1 << btrie_DEPTH;
    if (i == -1) {
        return (btrie[p].cnt) ? (btrie[p].cnt--, 1) : 0;
    }
    int b = (key >> i) & 1;
    if (!btrie[p].c[b]) { return 0; }
    int ok = btrie_erase(key, i - 1, btrie[p].c[b]);
    if (ok) {
        btrie[p].cnt--;
        if (!btrie[btrie[p].c[b]].cnt) {
            delete[btrie_del_ctr++] = btrie[p].c[b];
            btrie[p].c[b] = 0;
        }
    }
    return ok;
}

// b: 1 for MAX, 0 for MIN
// Return mINF for MAX and pINF for MIN if btrie is empty
key_type btrie_get_extreme(int b) {
    int p = 0; key_type res = 0;
    (btrie[0].cnt) ? btrie_find_impl(p, &res, 0, 1 ^ b) : (res = (b) ? btrie_mINF : btrie_pINF);
    return res ^ (1 << btrie_DEPTH);
}

// b: 1 for ge, 0 for le
// Return mINF for ge and pINF for le if no such element exists
key_type btrie_find(key_type x, int b) {
    x ^= (1 << btrie_DEPTH);
    int p = 0; key_type res = 0;
    for (int i = btrie_DEPTH; i >= 0; --i) {
        int bit = (x >> i) & 1;
        if (btrie[p].c[bit]) {
            p = btrie[p].c[bit];
            res = res << 1 | bit;
        } else {
            (btrie_find_impl(p, &res, 1 ^ b ^ bit, b) == 0) ? (res = (b) ? btrie_mINF : btrie_pINF) : 0;
            break;
        }
    }
    return res ^ (1 << btrie_DEPTH);
}

// Find the k-th smallest element in the btrie
// Always executes successfully if k is in bounds
key_type btrie_value_at(int k) {
    // handle the case when k is out of bounds
    // if (k < 0 || k >= btrie[0].cnt) {
    //     return 0x7fffffff;
    // }
    int p = 0; key_type res = 0;
    for (int i = btrie_DEPTH; i >= 0; --i) {
        if (btrie[p].c[0] && btrie[btrie[p].c[0]].cnt > k) {
            p = btrie[p].c[0];
        } else {
            k -= (btrie[p].c[0] != 0) * btrie[btrie[p].c[0]].cnt;
            p = btrie[p].c[1];
            res |= (1 << i);
        }
    }
    return res ^ (1 << btrie_DEPTH);
}

// Find the number of elements strictly less than x
// Always executes successfully
int btrie_order_of_key(key_type x) {
    x ^= (1 << btrie_DEPTH);
    int p = 0, res = 0;
    for (int i = btrie_DEPTH; i >= 0; --i) {
        int b = (x >> i) & 1;
        res += b * btrie[btrie[p].c[0]].cnt;
        if (!btrie[p].c[b]) {
            return res;
        }
        p = btrie[p].c[b];
    }
    return res;
}

int main() {
    
    return 0;
}