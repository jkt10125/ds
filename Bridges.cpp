#include <bits/stdc++.h>

std::vector<std::array<int, 2>> Bridges(const std::vector<std::vector<int>> &A) {
    int n = A.size(), time = 0;
    std::vector<int> low(n, -1), tin(n, -1), vis(n, false);
    std::vector<std::array<int, 2>> bridges;
    std::function<void(int, int)> dfs = [&] (int x, int p) {
        vis[x] = true; low[x] = tin[x] = time++;
        for (int y : A[x]) {
            if (y == p) { continue; }
            if (vis[y]) { low[x] = std::min(low[x], tin[y]); }
            else {
                dfs(y, x);
                low[x] = std::min(low[x], low[y]);
                if (low[y] > tin[x]) { bridges.push_back({x, y}); }
            }
        }
    };
    for (int i = 0; i < n; i++) { if (!vis[i]) { dfs(i, -1); } }
    return bridges;
};

