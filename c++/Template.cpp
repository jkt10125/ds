#include <bits/stdc++.h>

/* OUTPUT OPERATORS */
template <typename T> std::ostream &operator << (std::ostream &os, const std::vector<T> &v) { for (T x : v) os << x << ' '; return os; }
template <typename T> std::ostream &operator << (std::ostream &os, const std::set<T> &s) { for (T x : s) os << x << ' '; return os; }
template <typename T> std::ostream &operator << (std::ostream &os, const std::multiset<T> &s) { for (T x : s) os << x << ' '; return os; }
template <typename F, typename S> std::ostream &operator << (std::ostream &os, const std::pair<F, S> &p) { os << '{' << p.first << ", " << p.second << '}'; return os; }
template <typename F, typename S> std::ostream &operator << (std::ostream &os, const std::map<F, S> &m) { for (auto x : m) os << x.first << " : " << x.second << '\n'; return os; }

/* INPUT OPERATORS */
template <typename T> std::istream &operator >> (std::istream &is, std::vector<T> &v) { for (T &x : v) is >> x; return is; }
template <typename T> std::istream &operator >> (std::istream &is, std::pair<T, T> &p) { is >> p.first >> p.second; return is; }

#define fastIO std::ios_base::sync_with_stdio(0); std::cin.tie(0)
#define all(x) (x).begin(), (x).end()

using x64 = long long;

int main() {
    fastIO;
    int t = 1;
    // std::cin >> t;
    while (t--) {
        
    }
}