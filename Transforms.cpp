#include <bits/stdc++.h>

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

template <typename T>
inline void forward_multiple_transform(std::vector<T> &A) {
    int n = A.size() - 1;
    for (int p : prime) {
        if (p > n) break;
        for (int i = n / p; i > 0; i--) {
            A[i] += A[i * p];
        }
    }
}

template <typename T>
inline void backward_multiple_transform(std::vector<T> &A) {
    int n = A.size() - 1;
    for (int p : prime) {
        if (p > n) break;
        for (int i = 1; i * p <= n; i++) {
            A[i] -= A[i * p];
        }
    }
}

template <class T>
inline void forward_divisor_transform(std::vector<T>& a) {
    const int n = (int)a.size() - 1;
    for (int p : prime) {
        if (p > n) break;
        for (int i = 1; i * p <= n; ++i) {
            a[i * p] += a[i];
        }
    }
}

template <class T>
inline void backward_divisor_transform(std::vector<T>& a) {
    const int n = (int)a.size() - 1;
    for (const auto p : prime) {
        if (p > n) break;
        for (int i = n / p; i > 0; --i) {
            a[i * p] -= a[i];
        }
    }
}

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
    modint<MOD> inv() const { return pow(MOD - 2); }  // MOD must be a prime
    inline modint<MOD> operator / (modint<MOD> other) const { return *this * other.inv(); }
    inline modint<MOD> operator /= (modint<MOD> other) { return *this *= other.inv(); }
    inline bool operator == (modint<MOD> other) const { return value == other.value; }
    inline bool operator != (modint<MOD> other) const { return value != other.value; }
    inline bool operator < (modint<MOD> other) const { return value < other.value; }
    inline bool operator > (modint<MOD> other) const { return value > other.value; }
};

template <const int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <const int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <const int32_t MOD> std::istream &operator >> (std::istream & in, modint<MOD> &n) { in >> n.value; n.normalize(); return in; }
template <const int32_t MOD> std::ostream &operator << (std::ostream & out, modint<MOD> n) { return out << n.value; }

using mint = modint<998244353>;

// using namespace std;

int main() {

    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    sieve(1000000);

    int n;
    std::cin >> n;
    std::vector<mint> A(n + 1), B(n + 1);
    for (int i = 1; i <= n; i++) {
        std::cin >> A[i];
    }

    for (int i = 1; i <= n; i++) {
        std::cin >> B[i];
    }

    forward_divisor_transform(A);
    forward_divisor_transform(B);

    std::vector<mint> C(n + 1);

    for (int i = 1; i <= n; i++) {
        
        C[i] = (A[i] * B[i]);
    }

    backward_divisor_transform(C);

    for (int i = 1; i <= n; i++) {
        std::cout << C[i] << " ";
    }



    std::cout << "\n";

    return 0;
}

// __builtin_mul_overflow_p