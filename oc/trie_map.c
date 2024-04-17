typedef int       i32;  typedef unsigned int       u32;
typedef long long i64;  typedef unsigned long long u64;
// typedef __int128 i128;  typedef __uint128_t       u128;

#define TRIE_NODES 1000000

typedef char key_type;
typedef int value_type;

#define TRIE_CHILDREN 26
#define BASE_CHAR 'a'

typedef struct trie_node {
    int c[TRIE_CHILDREN], cnt, par, end;
    value_type value;
} trie_node;

trie_node trie[TRIE_NODES];
int delete[TRIE_NODES];
int trie_node_ctr = 1, trie_del_ctr = 0;

void trie_init() {
    trie[0].par = -1;
    trie[0].cnt = trie[0].end = 0;
    for (int i = 0; i < TRIE_CHILDREN; ++i) {
        trie[0].c[i] = 0;
    }
    trie_node_ctr = 1, trie_del_ctr = 0;
}

int trie_new_node_impl() {
    int idx = (trie_del_ctr) ? delete[--trie_del_ctr] : trie_node_ctr++;
    // Check for memory error
    // if (trie_node_ctr == TRIE_NODES) [[unlikely]] {
    //     return -1;
    // }
    for (int i = 0; i < TRIE_CHILDREN; ++i) {
        trie[idx].c[i] = 0;
    }
    trie[idx].cnt = 0;
    return idx; 
}

int trie_get_node(key_type *key, int key_len) {
    int p = 0;
    for (int i = 0; i < key_len; ++i) {
        if (!trie[p].c[key[i] - BASE_CHAR]) {
            trie[p].c[key[i] - BASE_CHAR] = trie_new_node_impl();
            // Check for memory error
            // if (trie[p].c[key[i] - BASE_CHAR] == -1) [[unlikely]] {
            //     return NULL;
            // }
            trie[trie[p].c[key[i] - BASE_CHAR]].par = p;
        }
        p = trie[p].c[key[i] - BASE_CHAR];
    }
    if (!trie[p].end) {
        for (int tp = p; tp != -1; tp = trie[tp].par) {
            trie[tp].cnt++;
        }
        trie[p].end = 1;
        trie[p].value = 0;
    }
    return p;
}

// trie_erase(element[], len, 0, 0)
int trie_erase(key_type *key, int key_len, int i, int p) {
    if (i == key_len) {
        if (!trie[p].end) { return 0; }
        trie[p].end = 0; trie[p].cnt--;
        return 1;
    }
    if (!trie[p].c[key[i] - BASE_CHAR]) { return 0; }
    int ok = trie_erase(key, key_len, i + 1, trie[p].c[key[i] - BASE_CHAR]);
    if (ok) {
        trie[p].cnt--;
        if (!trie[trie[p].c[key[i] - BASE_CHAR]].cnt) {
            delete[trie_del_ctr++] = trie[p].c[key[i] - BASE_CHAR];
            trie[p].c[key[i] - BASE_CHAR] = 0;
        }
    }
    return ok;
}

int trie_find(key_type *key, int key_len) {
    int p = 0;
    for (int i = 0; i < key_len; ++i) {
        p = trie[p].c[key[i] - BASE_CHAR];
        if (!p) { break; }
    }
    return (trie[p].end) ? p : 0;
}

int main() {
    
}
