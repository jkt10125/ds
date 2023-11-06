#include <bits/stdc++.h>

std::vector<int> LIS(std::vector<int> &A) {
    int n = A.size();
    std::vector<int> lis(n), temp;
    for (int i = 0; i < n; i++) {
        int idx = std::lower_bound(temp.begin(), temp.end(), A[i]) - temp.begin();
        if (idx == temp.size()) {
            temp.push_back(A[i]);
        }
        else {
            temp[idx] = A[i];
        }
        lis[i] = idx + 1;
    }
    return lis;
}

int main() {
    int t;
    std::cin >> t;
    while (t--) {
        int n;
        std::cin >> n;
        std::vector<int> A(n);
        for (int i = 0; i < n; i++) {
            std::cin >> A[i];
        }
        auto lis = LIS(A);
        int ctr = 0;
        for (int i : lis) {
            ctr += (i == 2);
        }
        std::cout << ctr << "\n";
    }
    return 0;
}