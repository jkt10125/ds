#include <bits/stdc++.h>

long long greaterCount(std::vector<int> &A, int k) {

    int n = A.size();
    long long ans = 0;

    for (int i = 0; i < n; i++) {

        A[i] += (i ? A[i - 1] : 0);

        ans += (A[i] > k || A[i] < -k);
    }

    std::sort(A.begin(), A.end());

    for (int i = 0; i < n; i++) {

        ans += A.end() - std::upper_bound(A.begin(), A.end(), A[i] + k);
    }

    return ans;
}

int main() {

    
}