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

struct RollbackUF {
	
    std::vector<int> e;
    std::vector<std::array<int, 2>> st;

    RollbackUF(int n) : e(n, -1) { }

	int size(int x) {
        return -e[find(x)];
    }
	
    int find(int x) {
        return e[x] < 0 ? x : find(e[x]);
    }
	
    int time() {
        return st.size();
    }
	
    void rollback(int t) {
		for (int i = time(); i --> t;) {
			e[st[i][0]] = st[i][1];
        }
		st.resize(t);
	}
    
	bool join(int a, int b) {
		a = find(a), b = find(b);
		if (a == b) return false;
		if (e[a] > e[b]) {
            std::swap(a, b);
        }
		st.push_back({a, e[a]});
		st.push_back({b, e[b]});
		e[a] += e[b]; e[b] = a;
		return true;
	}
};