#include <bits/stdc++.h>

std::vector<int> articulationPoints(const std::vector<std::vector<int>> &A) {
    int n = A.size(), time = 0;
    std::vector<int> low(n, -1), tin(n, -1), vis(n, false), art_points;
    std::function<void(int, int)> dfs = [&] (int x, int p) {
        vis[x] = true; low[x] = tin[x] = time++;
        int child = 0; bool ok = false;
        for (int y : A[x]) {
            if (y == p) { continue; }
            if (vis[y]) { low[x] = std::min(low[x], tin[y]); }
            else {
                dfs(y, x);
                low[x] = std::min(low[x], low[y]);
                if (low[y] >= tin[x] && p != -1) { ok = true; }
                child++;
            }
        }
        if (p == -1 && child > 1) { ok = true; }
        if (ok) { art_points.push_back(x); }
    };
    for (int i = 0; i < n; i++) { if (!vis[i]) { dfs(i, -1); } }
    return art_points;
}


int main() {

}
