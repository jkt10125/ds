#include <bits/stdc++.h>

// 0-based indexing
std::vector<std::array<int, 2>> eularTour(const std::vector<std::vector<int>> &A, int root) {
    std::vector<std::array<int, 2>> res(A.size());
    int timer = 0;
    std::function<void(int, int)> dfs = [&] (int x, int p) {
        res[x][0] = timer++;
        for (int y : A[x]) {
            if (y != p) {
                dfs(y, x);
            }
        }
        res[x][1] = timer++;
    };
    dfs(root, -1);
    return res;
}

int main() {
    
}