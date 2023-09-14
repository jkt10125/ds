#include <bits/stdc++.h>

// below is implementation of treap datatype
// which supports two main functions 
// f and g
// f is sum, g is max

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> uid(1, 1000000001);

using T = long long;

T f(T a, T b) { return a + b; }

T g(T a, T b) { return std::min(a, b); }

class treap {
    struct node {
        int cnt, prior;
        T val, _f, _g, lazy;
        bool rev;
        int left, right;
    };

    std::vector<node> tree;
    std::vector<int> deleted;
    int root;
    
    int new_node(T val) {
        if (deleted.empty()) {
            tree.push_back({1, uid(rng), val, val, val, 0, 0, 0, 0});
            return tree.size() - 1;
        }
        int v = deleted.back();
        deleted.pop_back();
        tree[v] = {1, uid(rng), val, val, val, 0, 0, 0, 0};
        return v;
    }

    void push(int v) {
        if (tree[v].rev) {
            tree[v].rev = false;
            std::swap(tree[v].left, tree[v].right);
            if (tree[v].left) tree[tree[v].left].rev ^= true;
            if (tree[v].right) tree[tree[v].right].rev ^= true;
        }
        if (tree[v].lazy) {
            tree[v].val += tree[v].lazy;
            tree[v]._f += tree[v].lazy * tree[v].cnt;
            tree[v]._g += tree[v].lazy;
            if (tree[v].left) tree[tree[v].left].lazy += tree[v].lazy;
            if (tree[v].right) tree[tree[v].right].lazy += tree[v].lazy;
            tree[v].lazy = 0;
        }
    }
    
    void update(int v) {
        if (!v) return;
        tree[v].cnt = tree[tree[v].left].cnt + tree[tree[v].right].cnt + 1;
        tree[v]._f = ::f(::f(tree[tree[v].left]._f, tree[tree[v].right]._f), tree[v].val);
        tree[v]._g = ::g(::g(tree[tree[v].left]._g, tree[tree[v].right]._g), tree[v].val);
    }
    
    void print(std::ostream &os, int v) {
        if (!v) return;
        push(v);
        print(os, tree[v].left);
        os << tree[v].val << ' ';
        print(os, tree[v].right);
    }

    public:
    treap() {
        tree.push_back({0, 0, 0, 0, (T)1e18, 0, 0, 0, 0});
        root = 0;
    }

    void split(int v, int &l, int &r, T key) {
        if (!v) { return void(l = r = 0); }

        push(v);
        if (tree[tree[v].left].cnt < key) {
            split(tree[v].right, tree[v].right, r, key - tree[tree[v].left].cnt - 1);
            l = v;
        } else {
            split(tree[v].left, l, tree[v].left, key);
            r = v;
        }
        update(v);
    }

    int merge(int l, int r) {
        if (!l || !r) return l + r;
        push(l);
        push(r);
        if (tree[l].prior > tree[r].prior) {
            tree[l].right = merge(tree[l].right, r);
            return update(l), l;
        } else {
            tree[r].left = merge(l, tree[r].left);
            return update(r), r;
        }
    }


    void insert(int pos, T val) {
        int l, r;
        split(root, l, r, pos);
        root = merge(merge(l, new_node(val)), r);
    }

    void append(T val) {
        root = merge(root, new_node(val));
    }

    void erase(int pos) {
        int l, r, ll, rr;
        split(root, l, r, pos + 1);
        split(l, ll, rr, pos);
        root = merge(ll, r);
        deleted.push_back(rr);
    }

    void reverse(int l, int r) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        tree[rrr].rev ^= true;
        root = merge(lll, merge(rrr, rr));
    }

    void add(int l, int r, T val) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        tree[rrr].lazy += val;
        root = merge(lll, merge(rrr, rr));
    }

    T f(int l, int r) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        T ans = tree[rrr]._f;
        root = merge(lll, merge(rrr, rr));
        return ans;
    }

    T g(int l, int r) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        T ans = tree[rrr]._g;
        root = merge(lll, merge(rrr, rr));
        return ans;
    }

    void leftCyclicShift(int l, int r, int k = 1) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        int sz = tree[rrr].cnt;
        k %= sz;
        int l1, r1;
        split(rrr, l1, r1, k);
        root = merge(lll, merge(r1, merge(l1, rr)));
    }

    void rightCyclicShift(int l, int r, int k = 1) {
        int ll, rr, lll, rrr;
        split(root, ll, rr, r + 1);
        split(ll, lll, rrr, l);
        int sz = tree[rrr].cnt;
        k %= sz;
        int l1, r1;
        split(rrr, l1, r1, sz - k);
        root = merge(lll, merge(r1, merge(l1, rr)));
    }

    T operator[] (int pos) {
        int l, r, ll, rr;
        split(root, l, r, pos + 1);
        split(l, ll, rr, pos);
        T ans = tree[rr].val;
        root = merge(ll, merge(rr, r));
        return ans;
    }

    int size() {
        return tree[root].cnt;
    }

    friend std::ostream &operator << (std::ostream &os, treap &t) {
        t.print(os, t.root);
        return os;
    }

};

int main() {
    int n, q;
    // std::cin >> n >> q;
    treap t;

    for (int i = 0; i < 10; ++i) {
        t.insert(i, i);
    }

    std::cout << t << "\n";


    // for (int i = 0; i < n; ++i) {
    //     int x;
    //     std::cin >> x;
    //     t.insert(i, x);
    // }

    // while (q--) {
    //     int type;
    //     std::cin >> type;
    //     if (type == 1) {
    //         int l, r;
    //         std::cin >> l >> r;
    //         std::cout << t._f(l, r) << "\n";
    //     } else if (type == 2) {
    //         int l, r;
    //         std::cin >> l >> r;
    //         std::cout << t._g(l, r) << "\n";
    //     } else if (type == 3) {
    //         int l, r;
    //         std::cin >> l >> r;
    //         t.reverse(l, r);
    //     } else if (type == 4) {
    //         int l, r, x;
    //         std::cin >> l >> r >> x;
    //         t.add(l, r, x);
    //     } else if (type == 5) {
    //         int pos, x;
    //         std::cin >> pos >> x;
    //         t.insert(pos, x);
    //     } else if (type == 6) {
    //         int pos;
    //         std::cin >> pos;
    //         t.erase(pos);
    //     }
    //     else {
    //         std::cout << t << "\n";
    //     }
    // }
}