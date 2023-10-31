#include <bits/stdc++.h>

std::vector<std::array<int, 2>> HopcroftKarp(int n, int m, const std::vector<std::array<int, 2>> &edges) {
    std::vector<int> G(edges.size()), L(n, -1), R(m, -1);
    std::vector<int> deg(n + 1), a, p, q(n);
    for (auto &[x, y] : edges) { deg[x]++; }
    for (int i = 1; i <= n; i++) { deg[i] += deg[i - 1]; }
    for (auto &[x, y] : edges) { G[--deg[x]] = y; }
    while (true) {
        a.assign(n, -1), p.assign(n, -1);
        int t = 0;
        for (int i = 0; i < n; i++) {
            if (L[i] == -1) {
                q[t++] = a[i] = p[i] = i;
            }
        }
        bool match = false;
        for (int i = 0; i < t; i++) {
            int x = q[i];
            if (L[a[x]] != -1) {
                continue;
            }
            for (int j = deg[x]; j < deg[x + 1]; j++) {
                int y = G[j];
                if (R[y] == -1) {
                    while (y != -1) {
                        R[y] = x;
                        std::swap(L[x], y);
                        x = p[x];
                    }
                    match = true;
                    break;
                }
                if (p[R[y]] == -1) {
                    q[t++] = y = R[y];
                    p[y] = x;
                    a[y] = a[x];
                }
            }
        }
        if (!match) {
            break;
        }
    }
    std::vector<std::array<int, 2>> res;
    for (int i = 0; i < L.size(); i++) {
        if (L[i] != -1) {
            res.push_back({i, L[i]});
        }
    }
    return res;
}

using namespace std;
int main() {

    ios::sync_with_stdio(false);
    cin.tie(nullptr);


    int n, m, k;
    cin >> n >> m >> k;

    vector<array<int, 2>> edges;

    for (int i = 0; i < k; i++) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }

    auto it = HopcroftKarp(n, m, edges);

    cout << it.size() << "\n";

    for (auto &[u, v] : it) {
        cout << u << " " << v << "\n";
    }

    return 0;
}