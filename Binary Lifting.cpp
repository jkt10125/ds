#include <bits/stdc++.h>

class treeAncestor {
    int n, log;
    std::vector<int> lvl;
    std::vector<std::vector<int>> max, lift, adj;
public:
    
    treeAncestor(int __n) {
        n = __n;
        log = std::__lg(n) + 1;
        lvl.resize(n);
        adj.resize(n);
        max.resize(log, std::vector<int>(n));
        lift.resize(log, std::vector<int>(n));
    }

    treeAncestor(const std::vector<int> &v) {
        n = v.size();
        log = std::__lg(n) + 1;
        lvl.resize(n);
        adj.resize(n);
        max.resize(log, std::vector<int>(n));
        lift.resize(log, std::vector<int>(n));
        max[0] = v;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void calc_lvl() {
        std::function<void(int, int)> dfs = [&] (int x, int p) {
            lvl[x] = (p != -1) ? lvl[p] + 1 : 0;
            for (int y : adj[x]) {
                if (y != p) { dfs(y, x); }
            }
        };
        dfs(0, -1);
    }

    void apply() {
        std::function<void(int, int)> dfs = [&] (int x, int p) {
            lift[0][x] = p;
            for (int i = 1; i < log; i++) {
                lift[i][x] = (lift[i - 1][x] != -1) ? lift[i - 1][lift[i - 1][x]] : -1;
                max[i][x] = std::max(max[i - 1][x], (lift[i - 1][x] != -1) ? max[i - 1][lift[i - 1][x]] : 0);
            }
            for (int y : adj[x]) {
                if (y != p) { dfs(y, x); }
            }
        };
        dfs(0, -1);
    }

    int kthAncestor(int x, int k) {
        for (int i = log - 1; i >= 0; i--) {
            if (k & (1 << i)) { x = lift[i][x]; }
        }
        return x;
    }

    int lca(int x, int y) {
        if (lvl[x] < lvl[y]) { std::swap(x, y); }
        x = kthAncestor(x, lvl[x] - lvl[y]);
        if (x == y) { return x; }
        for (int i = log - 1; i >= 0; i--) {
            if (lift[i][x] != lift[i][y]) {
                x = lift[i][x];
                y = lift[i][y];
            }
        }
        return lift[0][x];
    }

    int maxEdge(int x, int y) {
        int l = lca(x, y), ans = 0;

        for (int i = log - 1; i >= 0; i--) {
            if (lvl[x] - (1 << i) >= lvl[l]) {
                ans = std::max(ans, max[i][x]);
                x = lift[i][x];
            }
            if (lvl[y] - (1 << i) >= lvl[l]) {
                ans = std::max(ans, max[i][y]);
                y = lift[i][y];
            }
        }
        return ans;
    }
};