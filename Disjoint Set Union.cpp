#include <bits/stdc++.h>

class DisjointSetUnion : std::vector<int> {
public:
    DisjointSetUnion(int n) : std::vector<int>(n, -1) { }

    int find(int x) {
        return (*this)[x] < 0 ? x : (*this)[x] = find((*this)[x]);
    }
    
    bool merge(int x, int y) {
        if ((x = find(x)) == (y = find(y))) return 0;
        if ((*this)[x] > (*this)[y]) std::swap(x, y);
        (*this)[x] += (*this)[y]; (*this)[y] = x;
        return 1;
    }

    int size(int x) {
        return -(*this)[find(x)];
    }
    
    std::vector<std::vector<int>> get_leaders() {
        int n = std::vector<int>::size();
        std::vector<std::vector<int>> lead(n);
        for (int i = 0; i < n; ++i) {
            lead[find(i)].push_back(i);
        }
        return lead;
    }
};
