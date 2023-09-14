#include <bits/stdc++.h>

void radixSort(int *a, int *b, int *r, int n, int K) {
    int *c = new int[K + 1];
    for (int i = 0; i <= K; i++) { c[i] = 0; }
    for (int i = 0; i < n; i++) { c[r[a[i]]]++; }
    for (int i = 0, sum = 0; i <= K; i++) {
        std::swap(sum, c[i]); sum += c[i];
    }
    for (int i = 0; i < n; i++) b[c[r[a[i]]]++] = a[i];
    delete[] c;
}

template <const int32_t n>
inline bool leq(const std::array<int, n> &a, const std::array<int, n> &b) { return a <= b; }

// find the suffix array SA of s[0..n-1] in {1..K}ˆn
// require s[n]=s[n+1]=s[n+2]=0, n>=2
void suffix_array(int *s, int *SA, int n, int K) {
    if (n == 1) { return void (SA[0] = 0); }
    int n0 = (n + 2) / 3, n1 = (n + 1) / 3, n2 = n / 3, n02 = n0 + n2;
    int *s12 = new int[n02 + 3], *SA12 = new int[n02 + 3], *s0 = new int[n0], *SA0 = new int[n0];
    s12[n02] = s12[n02 + 1] = s12[n02 + 2] = 0;
    SA12[n02] = SA12[n02 + 1] = SA12[n02 + 2] = 0;
    for (int i = 0, j = 0; i < n + (n0 - n1); i++) {
        if (i % 3 != 0) { s12[j++] = i; }
    }
    radixSort(s12, SA12, s + 2, n02, K);
    radixSort(SA12, s12, s + 1, n02, K);
    radixSort(s12, SA12, s, n02, K);
    int name = 0, c0 = -1, c1 = -1, c2 = -1;
    for (int i = 0; i < n02; i++) {
        if (s[SA12[i]] != c0 || s[SA12[i] + 1] != c1 || s[SA12[i] + 2] != c2) {
            name++; c0 = s[SA12[i]]; c1 = s[SA12[i] + 1]; c2 = s[SA12[i] + 2];
        }
        (SA12[i] % 3 == 1) ? s12[SA12[i] / 3] = name : s12[SA12[i] / 3 + n0] = name;
    }
    if (name < n02) {
        suffix_array(s12, SA12, n02, name);
        for (int i = 0; i < n02; i++) { s12[SA12[i]] = i + 1; }
    }
    else {
        for (int i = 0; i < n02; i++) { SA12[s12[i] - 1] = i; }
    }
    for (int i = 0, j = 0; i < n02; i++) {
        if (SA12[i] < n0) { s0[j++] = 3 * SA12[i]; }
    }
    radixSort(s0, SA0, s, n0, K);
    for (int p = 0, t = n0 - n1, k = 0; k < n; k++) {
        #define GetI() (SA12[t] < n0 ? SA12[t] * 3 + 1 : (SA12[t] - n0) * 3 + 2)
        int i = GetI(), j = SA0[p];
        if (SA12[t] < n0 ? leq<2>({s[i], s12[SA12[t] + n0]}, {s[j], s12[j / 3]}) : leq<3>({s[i], s[i + 1], s12[SA12[t] - n0 + 1]}, {s[j], s[j + 1], s12[j / 3 + n0]})) {
            SA[k] = i;
            if (++t == n02) { for (k++; p < n0; p++, k++) { SA[k] = SA0[p]; } }
        }
        else {
            SA[k] = j;
            if (++p == n0) { for (k++; t < n02; t++, k++) { SA[k] = GetI(); } }
        }
        #undef GetI
    }
    delete[] s12; delete[] SA12; delete[] SA0; delete[] s0;
}

std::vector<int> suffixArray(std::vector<int> &s) {
    int n = s.size();
    std::vector<int> SA(n);
    s.resize(n + 3, 0);
    suffix_array(s.data(), SA.data(), n, *std::max_element(s.begin(), s.end()));
    s.resize(n);
    return SA;
}

std::vector<int> longestCommonPrefix(const std::vector<int> &SA, const std::vector<int> &s) {
    int n = SA.size(), k = 0;
    std::vector<int> lcp(n - 1), rank(n);
    for (int i = 0; i < n; i++) { rank[SA[i]] = i; }
    for (int i = 0; i < n; i++) {
        if (rank[i] == n - 1) { k = 0; continue; }
        int j = SA[rank[i] + 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) { k++; }
        lcp[rank[i]] = (k) ? k-- : 0;
    }
    return lcp;
}

int64_t countDistinctSubstrings(const std::vector<int> &lcp) {
    int64_t n = lcp.size();
    return n * (n + 1) / 2 - std::accumulate(lcp.begin(), lcp.end(), 0ll);
}

// Returns the range of matched prefixes of t in the suffix array SA of s. : O(|t|log|s|)
std::pair<int, int> substringFind(const std::vector<int> &SA, const std::vector<int> &s, const std::vector<int> &t) {
    int n = s.size(), o = t.size(), L = -1, R = -2, l = 0, r = n - 1;
    while (l <= r) {
        int m = (l + r) / 2, len = std::min(SA[m] + o, n);
        std::vector<int> tmp(s.begin() + SA[m], s.begin() + len);
        if (tmp == t) { L = m; r = m - 1; }
        else if (tmp < t) { l = m + 1; }
        else { r = m - 1; }
    }
    l = 0, r = n - 1;
    while (l <= r) {
        int m = (l + r) / 2, len = std::min(SA[m] + o, n);
        std::vector<int> tmp(s.begin() + SA[m], s.begin() + len);
        if (tmp == t) { R = m; l = m + 1; }
        else if (tmp > t) { r = m - 1; }
        else { l = m + 1; }
    }
    return {L, R};
}

int main() {
    std::vector<int> s = {1, 2, 3, 1, 2, 3, 1, 2, 3};
    std::vector<int> lcp = longestCommonPrefix(suffixArray(s), s);
    for (int i = 0; i < lcp.size(); i++) {
        std::cout << lcp[i] << " ";
    }
}