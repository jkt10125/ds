#include <bits/stdc++.h>

const int inf = 1000000000;

int n;
std::vector<std::vector<int>> capacity, flow;
std::vector<int> height, excess;

void push(int u, int v) {
    int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
    flow[u][v] += d; excess[v] += d;
    flow[v][u] -= d; excess[u] -= d;
}

void relabel(int u) {
    int d = inf;
    for (int i = 0; i < n; i++) {
        if (capacity[u][i] - flow[u][i] > 0) {
            d = std::min(d, height[i]);
        }
    }
    if (d < inf) { height[u] = d + 1; }
}

std::vector<int> find_max_height_vertices(int s, int t) {
    std::vector<int> max_height;
    for (int i = 0; i < n; i++) {
        if (i != s && i != t && excess[i] > 0) {
            if (!max_height.empty() && height[i] > height[max_height[0]])
                max_height.clear();
            if (max_height.empty() || height[i] == height[max_height[0]])
                max_height.push_back(i);
        }
    }
    return max_height;
}

int max_flow(int s, int t) {
    height.assign(n, 0);
    height[s] = n;
    flow.assign(n, std::vector<int>(n, 0));
    excess.assign(n, 0);
    excess[s] = inf;
    for (int i = 0; i < n; i++) {
        if (i != s) { push(s, i); }
    }

    std::vector<int> current;
    while (!(current = find_max_height_vertices(s, t)).empty()) {
        for (int i : current) {
            bool pushed = false;
            for (int j = 0; j < n && excess[i]; j++) {
                if (capacity[i][j] - flow[i][j] > 0 && height[i] == height[j] + 1) {
                    push(i, j);
                    pushed = true;
                }
            }
            if (!pushed) { relabel(i); break; }
        }
    }

    int max_flow = 0;
    for (int i = 0; i < n; i++) { max_flow += flow[i][t]; }
    return max_flow;
}

// function to find the minimum cut
std::vector<std::pair<int, int>> min_cut(int s, int t) {
    max_flow(s, t);

    std::vector<bool> vis(n, false);
    std::queue<int> Q;
    Q.push(s);
    while (!Q.empty()) {
        int u = Q.front();
        vis[u] = true;
        Q.pop();
        for (int v = 0; v < n; v++) {
            if (vis[v] == false && capacity[u][v] - flow[u][v] > 0) {
                Q.push(v);
            }
        }
    }

    std::vector<std::pair<int, int>> res;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (vis[i] && !vis[j] && capacity[i][j]) {    
                res.push_back({i, j});
            }
        }
    }

    return res;
}

int main() {

    int m;
    std::cin >> n >> m;

    capacity.assign(n, std::vector<int>(n, 0));
    for (int i = 0; i < m; i++) {
        int u, v;
        std::cin >> u >> v;
        u--; v--;
        capacity[u][v] = 1;
        capacity[v][u] = 1;
    }

    // print min cut
    std::vector<std::pair<int, int>> res = min_cut(0, n - 1);
    std::cout << res.size() << "\n";
    for (auto p : res) {
        std::cout << p.first + 1 << " " << p.second + 1 << "\n";
    }
    return 0;
}