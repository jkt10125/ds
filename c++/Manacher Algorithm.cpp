#include <bits/stdc++.h>

std::vector<int> manacher(const std::string &s) {
    std::string t = "#";
    int n = (s.size() << 1 | 1);
    for (auto c : s) {
        t += (std::string(1, c) + "#");
    }
    std::vector<int> r(n);
    for (int i = 0, j = 0; i < n; i++) {   
        if ((j << 1) - i >= 0 && j + r[j] > i) {
            r[i] = std::min(r[(j << 1) - i], j + r[j] - i);
        }
        while (i - r[i] >= 0 && i + r[i] < n && t[i - r[i]] == t[i + r[i]]) {
            r[i]++;
        }
        if (i + r[i] > j + r[j]) {
            j = i;
        }
    }
    return r;
}

int main() {
    std::string s;
    std::cin >> s;
    int n = s.length();
    std::vector<int> r = manacher(s);
    for (auto x : r) { std::cout << x << " "; }
}