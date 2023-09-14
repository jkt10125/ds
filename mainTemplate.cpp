#include <bits/stdc++.h>

using x64 = long long;

using T = int32_t;

// ################## Binary Lifting ################### //

class treeAncestor {
    int n, log;
    std::vector<int> lvl;
    std::vector<std::vector<int>> max, lift, adj;
public:
    
    treeAncestor(int __n) {
        n = __n;
        log = std::__lg(n) + 1;
        lvl.resize(n);
        adj.resize(n);
        max.resize(log, std::vector<int>(n));
        lift.resize(log, std::vector<int>(n));
    }

    treeAncestor(const std::vector<int> &v) {
        n = v.size();
        log = std::__lg(n) + 1;
        lvl.resize(n);
        adj.resize(n);
        max.resize(log, std::vector<int>(n));
        lift.resize(log, std::vector<int>(n));
        max[0] = v;
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void calc_lvl() {
        std::function<void(int, int)> dfs = [&] (int x, int p) {
            lvl[x] = (p != -1) ? lvl[p] + 1 : 0;
            for (int y : adj[x]) {
                if (y != p) { dfs(y, x); }
            }
        };
        dfs(0, -1);
    }

    void apply() {
        std::function<void(int, int)> dfs = [&] (int x, int p) {
            lift[0][x] = p;
            for (int i = 1; i < log; i++) {
                lift[i][x] = (lift[i - 1][x] != -1) ? lift[i - 1][lift[i - 1][x]] : -1;
                max[i][x] = std::max(max[i - 1][x], (lift[i - 1][x] != -1) ? max[i - 1][lift[i - 1][x]] : 0);
            }
            for (int y : adj[x]) {
                if (y != p) { dfs(y, x); }
            }
        };
        dfs(0, -1);
    }

    int kthAncestor(int x, int k) {
        for (int i = log - 1; i >= 0; i--) {
            if (k & (1 << i)) { x = lift[i][x]; }
        }
        return x;
    }

    int lca(int x, int y) {
        if (lvl[x] < lvl[y]) { std::swap(x, y); }
        x = kthAncestor(x, lvl[x] - lvl[y]);
        if (x == y) { return x; }
        for (int i = log - 1; i >= 0; i--) {
            if (lift[i][x] != lift[i][y]) {
                x = lift[i][x];
                y = lift[i][y];
            }
        }
        return lift[0][x];
    }

    int maxEdge(int x, int y) {
        int l = lca(x, y), ans = 0;

        for (int i = log - 1; i >= 0; i--) {
            if (lvl[x] - (1 << i) >= lvl[l]) {
                ans = std::max(ans, max[i][x]);
                x = lift[i][x];
            }
            if (lvl[y] - (1 << i) >= lvl[l]) {
                ans = std::max(ans, max[i][y]);
                y = lift[i][y];
            }
        }
        return ans;
    }
};


// ################## Custom Stack and Queue ################### //

// using T = int32_t;
// f is an idempotent function. Example: min, max, gcd, lcm, etc.
T f(T a, T b);

struct Stack : std::vector<std::array<T, 2>> {
    void push(T v) { push_back({v, ::f(v, empty() ? v : back()[1])}); }
    void pop() { pop_back(); }
    bool empty() { return this->std::vector<std::array<T, 2>>::empty(); }
    int size() { return this->std::vector<std::array<T, 2>>::size(); }
    T top() { return back()[0]; }
    T f() { return back()[1]; }
};

// [0][] : push stack, [1][] : pop stack
struct Queue : std::array<Stack, 2> {
    void ts() {
        if (!(*this)[1].empty()) { return; }
        while (!(*this)[0].empty()) { (*this)[1].push((*this)[0].top()); (*this)[0].pop(); }
    }
    void push(T v) { (*this)[0].push(v); }
    void pop() { ts(); (*this)[1].pop(); }
    bool empty() { return (*this)[0].empty() && (*this)[1].empty(); }
    int size() { return (*this)[0].size() + (*this)[1].size(); }
    T front() { ts(); return (*this)[1].top(); }
    T f() { ts(); return (*this)[0].empty() ? (*this)[1].f() : ::f((*this)[0].f(), (*this)[1].f()); }
};

// ################## Disjoint Set Union ################### //

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

// ################## Extended GCD ################### //

int egcd(int a, int b, int &x, int &y) {
	if (!a && !b) { return (x = y = 0); }
	int q0, r0, r1, r2, x0, x1, x2;
	r2 = a, r1 = b; x2 = 1, x1 = 0;
	while (r1) {
		q0 = r2 / r1;
		r0 = r2 - q0 * r1; r2 = r1, r1 = r0;
		x0 = x2 - q0 * x1; x2 = x1, x1 = x0;
	}
	y = (b) ? (r2 - a * x2) / b : 0, x = x2;
	return r2;
}

// ################## Fenwick Tree ################### //

// using T = int32_t;
class FenwickTree : std::array<std::vector<T>, 2> {
    
    void update(int i, T v) {
        for (int j = i + 1; j <= (int)(*this)[0].size(); j += j & -j) {
            
            (*this)[0][j - 1] += v;
            (*this)[1][j - 1] += v * i;
        }
    }

    T prefixSum(int i) {
        T s0 = T(), s1 = T();
        
        for (int j = i + 1; j; j -= j & -j) {
            
            s0 += (*this)[0][j - 1];
            s1 += (*this)[1][j - 1];
        }

        return s0 * i - s1;
    }

public:
    FenwickTree() = default;
    FenwickTree(int n) {
        
        (*this)[0] = (*this)[1] = std::vector<T>(n, 0);
    }
    void rangeUpdate(int l, int r, T val) {
        
        update(l, val);
        update(r + 1, -val);
    }
    T rangeSum(int l, int r) {
        return prefixSum(r + 1) - prefixSum(l);
    }
};

// fenwick tree in 2D
// using T = int32_t;
struct FenwickTree2D : std::vector<std::vector<T>> {
    FenwickTree2D(int n, int m) { assign(n, std::vector<T>(m, T())); }

    void add(int x, int y, T v) {
        for (int i = x + 1; i <= size(); i += (i & -i)) {
            for (int j = y + 1; j <= (*this)[i - 1].size(); j += (j & -j)) {
                (*this)[i - 1][j - 1] += v;
            }
        }
    }

    T prefixSum(int x, int y) {
        T sum = T();
        for (int i = x + 1; i; i -= (i & -i)) {
            for (int j = y + 1; j; j -= (j & -j)) {
                sum += (*this)[i - 1][j - 1];
            }
        }
        return sum;
    }

    T rangeSum(int x1, int y1, int x2, int y2) {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
        return prefixSum(x2, y2) - prefixSum(x1 - 1, y2) - prefixSum(x2, y1 - 1) + prefixSum(x1 - 1, y1 - 1);
    }
};

// ################## Segment Tree ################### //

// using T = int32_t;
class SegTree : std::vector<T> {
    int n;
public:
    SegTree(int __n, bool k = 0) {
        n = (k) ? (1 << (32 - __builtin_clz(__n - 1))) : __n;
        resize(n << 1);
    }

    SegTree(std::vector<T> &A, bool k = 0) {
        n = (k) ? (1 << (32 - __builtin_clz(A.size() - 1))) : A.size();
        resize(n << 1); std::copy(A.begin(), A.end(), begin() + n);
        for (int i = n - 1; i; i--) {
            (*this)[i] = std::min((*this)[i << 1], (*this)[i << 1 | 1]);
        }
    }

    // point update
    void update(int i, T v) {
        for ((*this)[i += n] = v; i > 1; i >>= 1) {
            (*this)[i >> 1] = std::min((*this)[i], (*this)[i ^ 1]);
        }
    }

    // range query
    T query(int l, int r) {
        T res = INT_MAX;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = std::min(res, (*this)[l++]);
            if (r & 1) res = std::min(res, (*this)[--r]);
        }
        return res;
    }
};

// ################## Lazy Segment Tree ################### //

// using T = int32_t;
class LazySegTree {
    int h, n;
    std::vector<T> tree, lazy;

    void apply(int p, T value, int k) { tree[p] += value; if (p < n) lazy[p] += value; }

    void calc(int p, int k) { tree[p] = std::max(tree[p << 1], tree[p << 1 | 1]) + lazy[p]; }

    void push(int l, int r) {
        int s = h, k = (1 << (h - 1));
        for (l += n, r += n - 1; s > 0; --s, k >>= 1) {
            for (int i = l >> s; i <= r >> s; ++i) {
                if (lazy[i]) {
                    apply(i << 1, lazy[i], k);
                    apply(i << 1 | 1, lazy[i], k);
                    lazy[i] = 0;
                }
            }
        }
    }

public:
    LazySegTree() { }
    LazySegTree(int n) : n(n), h(std::__lg(n) + 1) { lazy.resize(n); tree.resize(n << 1); }
    
    // range update
    void update(int l, int r, T v) {
        push(l, l + 1); push(r, r + 1);
        bool cl = 0, cr = 0; int k = 1;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1, k <<= 1) {
            if (cl) calc(l - 1, k); if (cr) calc(r, k);
            if (l & 1) apply(l++, v, k), cl = 1;
            if (r & 1) apply(--r, v, k), cr = 1;
        }
        for (--l; r > 0; l >>= 1, r >>= 1, k <<= 1) {
            if (cl) calc(l, k);
            if (cr && (!cl || l != r)) calc(r, k);
        }
    }

    // range query
    T query(int l, int r) {
        push(l, l + 1); push(r, r + 1);
        T res = -1e18;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) res = std::max(res, tree[l++]);
            if (r & 1) res = std::max(res, tree[--r]);
        }
        return res;
    }
};

// ################## String Algorithms ################### //

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

std::vector<int> prefixFunction (const std::string &s) {
    int n = s.size();
    std::vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i - 1];
        while (j > 0 && s[i] != s[j]) { j = pi[j - 1]; }
        if (s[i] == s[j]) { j++; } pi[i] = j;
    }
    return pi;
}

// ################## Prime Number Theory ################### //

std::vector<int> spf, prime;

// O(n) sieve
void sieve(int n) {
    spf.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) { spf[i] = i, prime.push_back(i); }
        for (int p : prime) { 
            if (p > spf[i] || i * p > n) break; 
            spf[i * p] = p; 
        }
    }
}

// prime must contain primes atleast upto sqrt(n)
bool isPrime (int n) {
    if (n < spf.size()) { return (spf[n] == n); }
    for(int i : prime) { 
        if (i * i > n) break; 
        if (n % i == 0) return false; 
    } 
    return true;
}

// prime must contain primes atleast upto sqrt(n)
std::vector<int> factor(int n) {
    std::vector<int> factr;
    if (n < spf.size()) {
        while (n > 1) { factr.push_back(spf[n]); n /= spf[n]; }
    }
    else {
        for (int i : prime) {
            if (i * i > n) break;
            while (n % i == 0) { factr.push_back(i); n /= i; }
        }
        if (n > 1) factr.push_back(n);
    }
    return factr;
}

int phi(int n) {
    std::vector<int> factr = factor(n);
    factr.erase(std::unique(factr.begin(), factr.end()), factr.end());

    for (int i : factr) n -= n / i;
    return n;
}

// divisor code for applying inclusion-exclusion principle
std::vector<std::array<int, 2>> divisor(int n) {
    std::vector<int> fact = factor(n);
    fact.erase(std::unique(fact.begin(), fact.end()), fact.end());
    
    std::vector<std::array<int, 2>> div(1, {1, 1});
    for (int i : fact) {
        int sz = div.size();
        for (int j = 0; j < sz; j++) {
            auto [x, y] = div[j];
            div.push_back({x * i, -y});
        }
    }
    return div;
}

// ################## Max Flow algorithms ################### //

namespace flow {
    const int inf = 1000000000;

    int n;
    std::vector<std::vector<int>> capacity, flow;
    std::vector<int> height, excess;

    void push(int u, int v) {
        int d = std::min(excess[u], capacity[u][v] - flow[u][v]);
        flow[u][v] += d; excess[v] += d;
        flow[v][u] -= d; excess[u] -= d;
    }

    void relabel(int u) {
        int d = inf;
        for (int i = 0; i < n; i++) {
            if (capacity[u][i] - flow[u][i] > 0) {
                d = std::min(d, height[i]);
            }
        }
        if (d < inf) { height[u] = d + 1; }
    }

    std::vector<int> find_max_height_vertices(int s, int t) {
        std::vector<int> max_height;
        for (int i = 0; i < n; i++) {
            if (i != s && i != t && excess[i] > 0) {
                if (!max_height.empty() && height[i] > height[max_height[0]])
                    max_height.clear();
                if (max_height.empty() || height[i] == height[max_height[0]])
                    max_height.push_back(i);
            }
        }
        return max_height;
    }

    int max_flow(int s, int t) {
        height.assign(n, 0);
        height[s] = n;
        flow.assign(n, std::vector<int>(n, 0));
        excess.assign(n, 0);
        excess[s] = inf;
        for (int i = 0; i < n; i++) {
            if (i != s) { push(s, i); }
        }

        std::vector<int> current;
        while (!(current = find_max_height_vertices(s, t)).empty()) {
            for (int i : current) {
                bool pushed = false;
                for (int j = 0; j < n && excess[i]; j++) {
                    if (capacity[i][j] - flow[i][j] > 0 && height[i] == height[j] + 1) {
                        push(i, j);
                        pushed = true;
                    }
                }
                if (!pushed) { relabel(i); break; }
            }
        }

        int max_flow = 0;
        for (int i = 0; i < n; i++) { max_flow += flow[i][t]; }
        return max_flow;
    }

    // function to find the minimum cut
    std::vector<std::pair<int, int>> min_cut(int s, int t) {
        max_flow(s, t);

        std::vector<bool> vis(n, false);
        std::queue<int> Q;
        Q.push(s);
        while (!Q.empty()) {
            int u = Q.front();
            vis[u] = true;
            Q.pop();
            for (int v = 0; v < n; v++) {
                if (vis[v] == false && capacity[u][v] - flow[u][v] > 0) {
                    Q.push(v);
                }
            }
        }

        std::vector<std::pair<int, int>> res;

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (vis[i] && !vis[j] && capacity[i][j]) {    
                    res.push_back({i, j});
                }
            }
        }

        return res;
    }
};

// ################## Strongly Connected Components ################### //

std::vector<int> topo_sort(const std::vector<std::vector<int>> &A) {
    int n = A.size();
    std::vector<int> order, vis(n, false);
    std::function<void(int)> dfs = [&] (int x) {
        vis[x] = true;
        for (int y : A[x]) { if (!vis[y]) { dfs(y); } }
        order.push_back(x);
    };
    for (int i = 0; i < n; i++) { if (!vis[i]) { dfs(i); } }
    return std::reverse(order.begin(), order.end()), order;
}

// KosaRaju's Algorithm
std::vector<std::vector<int>> get_scc(const std::vector<std::vector<int>> &A) {
    int n = A.size();
    std::vector<std::vector<int>> rA(n), SCC;
    for (int i = 0; i < n; i++) {
        for (int j : A[i]) { rA[j].push_back(i); }
    }
    std::vector<bool> vis(n, false);
    std::function<void(int)> scc_dfs = [&] (int x) {
        vis[x] = true;
        SCC.back().push_back(x);
        for (int y : rA[x]) { if (!vis[y]) { scc_dfs(y); } }
    };

    for (int x : topo_sort(A)) {
        if (!vis[x]) { SCC.emplace_back(); scc_dfs(x); }
    }
    return SCC;
}

// ################## Topological Labelling ################### //
// Condition: A is a DAG

std::vector<int> topo_label(const std::vector<std::vector<int>> &A) {
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

// ################## Bridges and Articulation Points ################### //
// condition: A is an undirected graph

std::vector<int> articulationPoints(const std::vector<std::vector<int>> &A) {
    int n = A.size(), time = 0;
    std::vector<int> low(n, -1), tin(n, -1), vis(n, false), art_points;
    std::function<void(int, int)> dfs = [&] (int x, int p) {
        vis[x] = true; low[x] = tin[x] = time++;
        int child = 0; bool ok = false;
        for (int y : A[x]) {
            if (y == p) { continue; }
            if (vis[y]) { low[x] = std::min(low[x], tin[y]); }
            else {
                dfs(y, x);
                low[x] = std::min(low[x], low[y]);
                if (low[y] >= tin[x] && p != -1) { ok = true; }
                child++;
            }
        }
        if (p == -1 && child > 1) { ok = true; }
        if (ok) { art_points.push_back(x); }
    };
    for (int i = 0; i < n; i++) { if (!vis[i]) { dfs(i, -1); } }
    return art_points;
}

std::vector<std::array<int, 2>> Bridges(const std::vector<std::vector<int>> &A) {
    int n = A.size(), time = 0;
    std::vector<int> low(n, -1), tin(n, -1), vis(n, false);
    std::vector<std::array<int, 2>> bridges;
    std::function<void(int, int)> dfs = [&] (int x, int p) {
        vis[x] = true; low[x] = tin[x] = time++;
        for (int y : A[x]) {
            if (y == p) { continue; }
            if (vis[y]) { low[x] = std::min(low[x], tin[y]); }
            else {
                dfs(y, x);
                low[x] = std::min(low[x], low[y]);
                if (low[y] > tin[x]) { bridges.push_back({x, y}); }
            }
        }
    };
    for (int i = 0; i < n; i++) { if (!vis[i]) { dfs(i, -1); } }
    return bridges;
};

// ################## Suffix Array ################### //

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

// ################## Trie ################### //

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

// ################## Modular Arithmetic ################### //

template <const int32_t MOD>
struct modint {
    int32_t value;
    modint() = default;
    modint(int32_t __value) : value(__value) { normalize(); }
    void normalize() { if (value >= MOD || value < 0) value %= MOD; if (value < 0) value += MOD; }
    inline modint<MOD> operator + (modint<MOD> other) const { int32_t c = this->value + other.value; return modint<MOD>(c >= MOD ? c - MOD : c); }
    inline modint<MOD> operator - (modint<MOD> other) const { int32_t c = this->value - other.value; return modint<MOD>(c < 0 ? c + MOD : c); }
    inline modint<MOD> operator * (modint<MOD> other) const { int32_t c = (int64_t)this->value * other.value % MOD; return modint<MOD>(c < 0 ? c + MOD : c); }
    inline modint<MOD> &operator += (modint<MOD> other) { this->value += other.value; if (this->value >= MOD) this->value -= MOD; return *this; }
    inline modint<MOD> &operator -= (modint<MOD> other) { this->value -= other.value; if (this->value < 0) this->value += MOD; return *this; }
    inline modint<MOD> &operator *= (modint<MOD> other) { this->value = (int64_t)this->value * other.value % MOD; if (this->value < 0) this->value += MOD; return *this; }
    inline modint<MOD> operator - () const { return modint<MOD>(this->value ? MOD - this->value : 0); }
    modint<MOD> pow(uint64_t k) const { modint<MOD> x = *this, y = 1; for (; k; k >>= 1) { if (k & 1) y *= x; x *= x; } return y; }
    inline modint<MOD> operator / (modint<MOD> other) const { return *this * other.inv(); }
    inline modint<MOD> operator /= (modint<MOD> other) { return *this *= other.inv(); }
    inline bool operator == (modint<MOD> other) const { return value == other.value; }
    inline bool operator != (modint<MOD> other) const { return value != other.value; }
    inline bool operator < (modint<MOD> other) const { return value < other.value; }
    inline bool operator > (modint<MOD> other) const { return value > other.value; }

    static std::vector<modint<MOD>> Fact, iFact, Inv;

    static void precompute(int n) {
        int m = int(Fact.size());
        if (n <= m) return;
        n = (1 << (32 - __builtin_clz(n)));
        Fact.resize(n), iFact.resize(n), Inv.resize(n);
        for (int i = m; i < n; i++) Fact[i] = Fact[i - 1] * i;
        iFact.back() = Fact.back().pow(MOD - 2); // MOD must be a prime
        for (int i = n - 2; i >= m; i--) iFact[i] = iFact[i + 1] * (i + 1);
        for (int i = n - 1; i >= m; i--) Inv[i] = iFact[i] * Fact[i - 1];
    }

    modint<MOD> fact() const {
        precompute(value + 1);
        return Fact[value];
    }

    modint<MOD> ifact() const {
        precompute(value + 1);
        return iFact[value];
    }

    modint<MOD> inv() const {
        if (value < std::min(MOD >> 1, 1 << 21)) {
            precompute(value + 1);
            return Inv[value];
        }
        return pow(MOD - 2); // MOD must be a prime
    }

    modint<MOD> nCr(const modint<MOD> &r) const {
        return (value < r.value) ? 0 : modint<MOD>(value - r.value).ifact() * r.ifact() * fact();
    }

    modint<MOD> nPr(const modint<MOD> &r) const {
        return (value < r.value) ? 0 : modint<MOD>(value - r.value).ifact() * fact();
    }

    modint<MOD> doublefac() const { // computes v * (v - 2) * (v - 4) ...
        int k = (value + 1) / 2;
        return (value & 1) ? modint(k * 2).fact() / (modint(2).pow(k) * modint(k).fact()) : modint(k).fact() * modint(2).pow(k);
    }
};

template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::Inv = {0};
template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::Fact = {1};
template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::iFact = {1};

template <const int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <const int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <const int32_t MOD> std::istream &operator >> (std::istream & in, modint<MOD> &n) { in >> n.value; n.normalize(); return in; }
template <const int32_t MOD> std::ostream &operator << (std::ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<1000000007>;

/* OUTPUT OPERATORS */
template <typename T> std::ostream &operator << (std::ostream &os, const std::vector<T> &v) { for (T x : v) os << x << ' '; return os; }
template <typename T> std::ostream &operator << (std::ostream &os, const std::set<T> &s) { for (T x : s) os << x << ' '; return os; }
template <typename T> std::ostream &operator << (std::ostream &os, const std::multiset<T> &s) { for (T x : s) os << x << ' '; return os; }
template <typename F, typename S> std::ostream &operator << (std::ostream &os, const std::pair<F, S> &p) { os << '{' << p.first << ", " << p.second << '}'; return os; }
template <typename F, typename S> std::ostream &operator << (std::ostream &os, const std::map<F, S> &m) { for (auto x : m) os << x.first << " : " << x.second << '\n'; return os; }

/* INPUT OPERATORS */
template <typename T> std::istream &operator >> (std::istream &is, std::vector<T> &v) { for (T &x : v) is >> x; return is; }
template <typename T> std::istream &operator >> (std::istream &is, std::pair<T, T> &p) { is >> p.first >> p.second; return is; }


/* Contains the information of upper and lower half of the array */
class pc {
    public:
    int low_sum, up_sum;
    std::multiset<int> low, up;

    pc() { low_sum = up_sum = 0; }

    int median() {
        assert(!low.empty());
        return *low.rbegin();
    }

    void adjust() {
        while(up.size() > low.size()) {
            int moving = *up.begin();
            up_sum -= moving;
            low_sum += moving;
            up.erase(up.find(moving));
            low.insert(moving);
        }
        while(low.size() > up.size() + 1) {
            int moving = *low.rbegin();
            low_sum -= moving;
            up_sum += moving;
            low.erase(low.find(moving));
            up.insert(moving);
        }
    }

    void insert(int val) {
        if(low.empty()) {
            low.insert(val);
            low_sum += val;
        }
        else {
            if(val > median()) {
                up.insert(val);
                up_sum += val;
            }
            else {
                low.insert(val);
                low_sum += val;
            }
            adjust();
        }
    }

    void erase(int val) {
        if(up.find(val) != up.end()) {
            up.erase(up.find(val));
            up_sum -= val;
        }
        else {
            low.erase(low.find(val));
            low_sum -= val;
        }
        adjust();
    }
};

#define fastIO std::ios_base::sync_with_stdio(0); std::cin.tie(0)
#define all(x) (x).begin(), (x).end()

int main() {

}