#include <bits/stdc++.h>

const int K = 26;
// [][K] : # of strings that pass through this node
// [][K + 1] : # of strings that end at this node
struct trie : std::vector<std::array<int, K + 2>> {
    std::array<int, K + 2> newNode() {
        std::array<int, K + 2> node; node[K] = 0; node[K + 1] = 0;
        for (int i = 0; i < K; i++) { node[i] = -1; }
        return node;
    }

    std::vector<int> get(const std::string &s) {
        std::vector<int> S;
        for (char c : s) { S.push_back(c - 'a'); }
        return S;
    }
    
    trie() { this->emplace_back(newNode()); }

    void insert(const std::vector<int> &s) {
        int node = 0; (*this)[node][K]++;
        for (int c : s) {
            if ((*this)[node][c] == -1) { (*this)[node][c] = this->size(); this->emplace_back(newNode()); }
            node = (*this)[node][c]; (*this)[node][K]++;
        }
        (*this)[node][K + 1]++;
    }

    bool erase(const std::vector<int> &s) {
        std::vector<int> i(1, 0);
        for (int c : s) {
            if ((*this)[i.back()][c] == -1) { return false; }
            i.push_back((*this)[i.back()][c]);
        }
        if (!(*this)[i.back()][K + 1]) { return false; }
        else { (*this)[i.back()][K + 1]--; }
        for (int it : i) { (*this)[it][K]--; }
        return true;
    }

    int search(const std::vector<int> &s) {
        int node = 0, i = 0;
        for (; i < (int)s.size(); i++) {
            if ((*this)[node][s[i]] == -1 || (*this)[(*this)[node][s[i]]][K] == 0) { return i; }
            node = (*this)[node][s[i]];
        }
        return i;
    }

    void insert(const std::string &s) { insert(get(s)); }
    bool erase(const std::string &s) { return erase(get(s)); }
    int search(const std::string &s) { return search(get(s)); }
};

// [2]: # of integers that pass through this node
class BIT_trie : std::vector<std::array<int, 3>> {
public:
    BIT_trie() {
        this->push_back({0, 0, 0});
        insert(0, 1);
    }

    void insert(int x, int v) {
        int node = 0;
        for (int i = 30; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if (!(*this)[node][bit]) {
                (*this)[node][bit] = this->size();
                this->push_back({0, 0, 0});
            }
            node = (*this)[node][bit];
            (*this)[node][2] += v;
        }
    }

    int maxXOR(int x) {
        int node = 0, ans = 0;
        for (int i = 30; i >= 0; i--) {
            int bit = (x >> i) & 1;
            if ((*this)[(*this)[node][bit ^ 1]][2]) {
                ans |= (1 << i);
                node = (*this)[node][bit ^ 1];
            }
            else {
                node = (*this)[node][bit];
            }
        }
        return ans;
    }
    int XORnotgtk(int x, int k) {
        int ctr = 0, node = 0;
        for (int i = 30; i >= 0; i--) {
            int x1 = (x >> i) & 1, k1 = (k >> i) & 1;
            if (k1) {
                ctr += (*this)[(*this)[node][x1 ^ 1]][2];
                node = (*this)[node][x1];
            }
            else {
                node = (*this)[node][x1];
            }
        }
        return ctr;
    }
};

int main() {
    // std::ios_base::sync_with_stdio(0);
    // std::cin.tie(0);

    int q;
    std::cin >> q;
    trie TR;
    while (q--) {
        char t; std::string s;
        std::cin >> t >> s;
        if (t == '+') { TR.insert(s); }
        else if (t == '-') { std::cout << TR.erase(s) << "\n"; }
        else { std::cout << TR.search(s) << '\n'; }
    }
}