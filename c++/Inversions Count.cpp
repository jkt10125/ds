#include <bits/stdc++.h>

int inv_count (std::vector<int> &A, int l, int r) {
    int res = 0;
    if (l < r) {
        int m = (l + r) / 2;
        res = inv_count (A, l, m) + inv_count(A, m + 1, r);
        int p1 = m + 1, p2, p3;
        for (int i = l; i <= m; i++) {
            while (p1 <= r && A[i] > A[p1]) {
                p1++;
            }
            res += (p1 - m - 1);
        }
        std::vector<int> tmp(r - l + 1);
        p1 = l, p2 = m + 1, p3 = 0;
        while (p1 <= m && p2 <= r) {
            tmp[p3++] = (A[p1] < A[p2]) ? A[p1++] : A[p2++];
        }
        while (p1 <= m) tmp[p3++] = A[p1++];
        while (p2 <= r) tmp[p3++] = A[p2++];
        p1 = 0, p2 = l;
        while (p2 <= r) A[p2++] = tmp[p1++];
    }
    return res;
}

int main() {
    std::vector<int> A = {7, 3, 5, 8, 1};
    std::cout << inv_count(A, 0, 4);
}