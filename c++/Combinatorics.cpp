#include <bits/stdc++.h>

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

int main() {
    
}