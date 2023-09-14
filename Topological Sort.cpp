#include <bits/stdc++.h>

std::vector<int> toposort(const std::vector<std::vector<int>> &A) {
    int n = A.size();
    std::vector<int> out(n), order;
    std::vector<std::vector<int>> rA(n);
    for (int i = 0; i < n; i++) {
        for (int j : A[i]) {
            out[i]++;
            rA[j].push_back(i);
        }
    }
    std::set<int> s;
    for (int i = 0; i < n; i++) { if (!out[i]) { s.insert(i); } }
    while (!s.empty()) {
        int x = *s.rbegin();
        s.erase(x);
        order.push_back(x);
        for (int y : rA[x]) {
            out[y]--;
            if (!out[y]) { s.insert(y); }
        }
    }
    return std::reverse(order.begin(), order.end()), order;
}