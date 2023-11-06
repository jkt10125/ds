#include <bits/stdc++.h>

class treeAncestor {
    int n, log;
    std::vector<int> lvl;
    std::vector<std::vector<int>> maxe, lift;

public:

    treeAncestor(std::vector<std::vector<std::array<int, 2>>> &A, int root = 0) {
        n = A.size();
        log = std::__lg(n) + 1;
        lvl.resize(n);
        lift.resize(log, std::vector<int>(n));
        maxe.resize(log, std::vector<int>(n));

        std::function<void(int, int)> dfs = [&] (int x, int p) {
            lvl[x] = (p != -1) ? lvl[p] + 1 : 0;
            lift[0][x] = p;
            for (int i = 1; i < log; i++) {
                lift[i][x] = (lift[i - 1][x] != -1) ? lift[i - 1][lift[i - 1][x]] : -1;
                maxe[i][x] = std::max(maxe[i - 1][x], (lift[i - 1][x] != -1) ? maxe[i - 1][lift[i - 1][x]] : 0);
            }
            for (auto [y, w] : A[x]) {
                if (y != p) {
                    maxe[0][y] = w;
                    dfs(y, x);
                }
            }
        };
        dfs(root, -1);
    }

    treeAncestor(std::vector<int> &par, std::vector<int> &W) {
        n = par.size();
        int root = 0;
        while (root < n && par[root] != -1) { root++; }
        std::vector<std::vector<std::array<int, 2>>> A(n);
        for (int i = 1; i < n; i++) {
            A[par[i]].push_back({i, W[i]});
            A[i].push_back({par[i], W[i]});
        }
        *this = treeAncestor(A, root);
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
                ans = std::max(ans, maxe[i][x]);
                x = lift[i][x];
            }
            if (lvl[y] - (1 << i) >= lvl[l]) {
                ans = std::max(ans, maxe[i][y]);
                y = lift[i][y];
            }
        }
        return ans;
    }
};