#include <bits/stdc++.h>

std::mt19937 rng((int) std::chrono::steady_clock::now().time_since_epoch().count());
std::uniform_int_distribution<int> uid(1, 1000000001);

class Treap {
    std::vector<std::array<int, 5>> tree;
    int root;

    int new_node(int val) {
        tree.push_back({1, uid(rng), val, 0, 0});
        return tree.size() - 1;
    }

    void split(int v, int &l, int &r, int idx) {
        if (!v) return void(l = r = 0);
        if (tree[tree[v][3]][0] < idx) {
            split(tree[v][4], tree[v][4], r, idx - tree[tree[v][3]][0] - 1);
            l = v;
        } else {
            split(tree[v][3], l, tree[v][3], idx);
            r = v;
        }
        tree[v][0] = tree[tree[v][3]][0] + tree[tree[v][4]][0] + 1;
    }

    int merge(int l, int r) {
        if (!l || !r) return l + r;
        if (tree[l][1] > tree[r][1]) {
            tree[l][4] = merge(tree[l][4], r);
            tree[l][0] = tree[tree[l][3]][0] + tree[tree[l][4]][0] + 1;
            return l;
        } else {
            tree[r][3] = merge(l, tree[r][3]);
            tree[r][0] = tree[tree[r][3]][0] + tree[tree[r][4]][0] + 1;
            return r;
        }
    }

public:
    Treap() { tree.push_back({0, 0, 0, 0, 0}); root = 0; }

    int size() { return tree[root][0]; }

    void insert(int idx, int val) {
        int l, r;
        split(root, l, r, idx);
        root = merge(merge(l, new_node(val)), r);
    }

    void erase(int idx) {
        int l, r, ll, rr;
        split(root, l, r, idx + 1);
        split(l, ll, rr, idx);
        root = merge(ll, r);
    }

    int operator [] (int idx) {
        int l, r, ll, rr;
        split(root, l, r, idx + 1);
        split(l, ll, rr, idx);
        int ans = tree[rr][2];
        root = merge(merge(ll, rr), r);
        return ans;
    }

    std::vector<int> iterate() {
        std::vector<int> a;
        std::function<void(int)> dfs = [&](int v) {
            if (!v) return;
            dfs(tree[v][3]);
            a.push_back(tree[v][2]);
            dfs(tree[v][4]);
        };
        dfs(root);
        return a;
    }
};

class CustomOrderedSet {
    std::vector<int> index, ft;
    Treap treap;

    void update(int i, int v) {
        for (++i; i <= ft.size(); i += i & -i) {
            ft[i - 1] += v;
        }
    }

    int sum(int i) {
        int ans = 0;
        for (++i; i; i -= i & -i) {
            ans += ft[i - 1];
        }
        return ans;
    }

    int get_idx(int v) {
        return std::lower_bound(index.begin(), index.end(), v) - index.begin();
    }

public:
    CustomOrderedSet() = default;

    void registerIndex(int idx) { index.push_back(idx); }

    void apply() {
        std::sort(index.begin(), index.end());
        index.erase(std::unique(index.begin(), index.end()), index.end());

        ft.assign(index.size(), 0);
    }

    void insert(int v) {
        int i = get_idx(v);
        treap.insert(sum(i - 1), v);
        update(i, 1);
    }

    void erase(int v) {
        int i = get_idx(v);
        treap.erase(sum(i - 1));
        update(i, -1);
    }

    int order_of_key(int v) {
        return sum(get_idx(v) - 1);
    }

    void clear() {
        index.clear();
        treap = Treap();
        ft.clear();
    }

    int find_by_order(int i) {
        return treap[i];
    }

    int size() {
        return treap.size();
    }

    int lower_bound(int v) {
        return treap[order_of_key(v)];
    }

    int upper_bound(int v) {
        return lower_bound(v + 1);
    }

    std::vector<int> iterate() {
        return treap.iterate();
    }
};

int main() {
    

    return 0;
}