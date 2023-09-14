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
    static int primitive_root;

    static void precompute(int n) {
        int m = int(Fact.size());
        if (n <= m) return;
        n = (1 << (32 - __builtin_clz(n)));
        Fact.resize(n), iFact.resize(n), Inv.resize(n);
        for (int i = m; i < n; i++) Fact[i] = Fact[i - 1] * i;
        iFact.back() = Fact.back().pow(MOD - 2);
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
        return pow(MOD - 2);
    }

    modint<MOD> nCr(const modint<MOD> &r) const {
        return (value < r.value) ? 0 : iFact[value - r.value] * iFact[r.value] * fact();
    }

    modint<MOD> nPr(const modint<MOD> &r) const {
        return (value < r.value) ? 0 : iFact[value - r.value] * fact();
    }

    modint<MOD> doublefac() const { // computes v * (v - 2) * (v - 4) ...
        int k = (value + 1) / 2;
        return (value & 1) ? modint(k * 2).fact() / (modint(2).pow(k) * modint(k).fact()) : modint(k).fact() * modint(2).pow(k);
    }

    modint<MOD> sqrt() const {
        if (MOD == 2 || !this->value) return this->value;
        if (this->pow((MOD - 1) / 2) != 1) return -1;
        modint<MOD> b = 1;
        while (b.pow((MOD - 1) / 2) == 1) b += 1;
        int e = __builtin_ctz(MOD - 1), m = ((MOD - 1) >> e);
        modint<MOD> x = this->pow((m - 1) / 2), y = (this->value) * x * x, z = b.pow(m);
        for (x *= (this->value); y != 1;) {
            int j = 0;
            modint<MOD> t = y;
            while (t != 1) j++, t *= t;
            z = z.pow(1ll << (e - j - 1));
            x *= z, z *= z, y *= z;
            e = j;
        }
        return std::min(x.value, MOD - x.value);
    }

    static modint<MOD> primitiveRoot() {
        if (!primitive_root) {
            std::vector<int> factr;
            int v = MOD - 1;
            for (int i = 2; 1ll * i * i <= v; i++) {
                if (v % i == 0) {
                    factr.push_back(i);
                    while (v % i == 0) { v /= i; }
                }
            }
            if (v > 1) factr.push_back(v);
            for (int g = 2; g < MOD; g++) {
                bool ok = true;
                for (int i : factr) {
                    if (modint<MOD>(g).pow((MOD - 1) / i) == 1) {
                        ok = false;
                        break;
                    }
                }
                if (ok) return (primitive_root = g);
            }
            return (primitive_root = -1);
        }
        return primitive_root;
    }
};

template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::Inv = {0};
template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::Fact = {1};
template <const int32_t MOD> std::vector<modint<MOD>> modint<MOD>::iFact = {1};
template <const int32_t MOD> int modint<MOD>::primitive_root = 0;

template <const int32_t MOD> modint<MOD> operator * (int64_t value, modint<MOD> n) { return modint<MOD>(value) * n; }
template <const int32_t MOD> modint<MOD> operator * (int32_t value, modint<MOD> n) { return modint<MOD>(value % MOD) * n; }
template <const int32_t MOD> std::istream &operator >> (std::istream & in, modint<MOD> &n) { in >> n.value; n.normalize(); return in; }
template <const int32_t MOD> std::ostream &operator << (std::ostream & out, modint<MOD> n) { return out << n.value; }


// Integer convolution for arbitrary mod
// with NTT (and Garner's algorithm) for ModInt / ModIntRuntime class.
// We skip Garner's algorithm if `skip_garner` is true or mod is in `nttprimes`.
// input: a (size: n), b (size: m)
// return: vector (size: n + m - 1)
template <const int32_t MOD>
std::vector<modint<MOD>> fftconv(std::vector<modint<MOD>>, std::vector<modint<MOD>>, bool = false);

constexpr int32_t fftprimes[] = {998244353, 167772161, 469762049};

// Integer FFT (Fast Fourier Transform) for ModInt class
// (Also known as Number Theoretic Transform, NTT)
// inv: inverse transform
// input size must be 2^n
template <const int32_t MOD>
void fft(std::vector<modint<MOD>> &a, bool inv) {
    int n = a.size();
    if (n == 1) return;
    static const modint<MOD> root = modint<MOD>::primitiveRoot();
    assert(__builtin_popcount(n) == 1 && (MOD - 1) % n == 0);
    static std::vector<modint<MOD>> w{1}, iw{1};
    for (int m = w.size(); m < (n >> 1); m <<= 1) {
        modint<MOD> dw = root.pow((MOD - 1) / (4 * m));
        w.resize(m << 1), iw.resize(m << 1);
        for (int i = 0; i < m; i++) {
            w[m + i] = w[i] * dw, iw[m + i] = iw[i] * dw.inv();
        }
    }


    if (!inv) {
        for (int m = (n >> 1); m; m >>= 1) {
            for (int s = 0, k = 0; s < n; s += (m << 1), k++) {
                for (int i = s; i < s + m; i++) {
                    modint<MOD> x = a[i], y = a[i + m] * w[k];
                    a[i] = x + y, a[i + m] = x - y;
                }
            }
        }
    }
    else {
        for (int m = 1; m < n; m <<= 1) {
            for (int s = 0, k = 0; s < n; s += (m << 1), k++) {
                for (int i = s; i < s + m; i++) {
                    modint<MOD> x = a[i], y = a[i + m];
                    a[i] = x + y, a[i + m] = (x - y) * iw[k];
                }
            }
        }

        int n_inv = modint<MOD>(n).inv().value;
        for (modint<MOD> &v : a) v *= n_inv;
    }
}

template <const int32_t MOD>
std::vector<modint<MOD>> fftconv_(const std::vector<int> &a, const std::vector<int> &b) {
    int sz = a.size();
    assert(a.size() == b.size() and __builtin_popcount(a.size()) == 1);
    
    std::vector<modint<MOD>> ap(sz), bp(sz);
    for (int i = 0; i < sz; i++) ap[i] = a[i], bp[i] = b[i];
    fft(ap, false);
    
    if (a == b) { bp = ap; }
    else { fft(bp, false); }
    
    for (int i = 0; i < int(a.size()); i++) ap[i] *= bp[i];
    fft(ap, true);
    return ap;
}

int64_t garner_fft_(int r0, int r1, int r2, int32_t mod) {
    using mint2 = modint<fftprimes[2]>;
    static const int64_t m01 = 1LL * fftprimes[0] * fftprimes[1];
    static const int64_t m0_inv_m1 = modint<fftprimes[1]>(fftprimes[0]).inv().value;
    static const int64_t m01_inv_m2 = mint2(m01 % fftprimes[2]).inv().value;

    int v1 = (m0_inv_m1 * (r1 + fftprimes[1] - r0)) % fftprimes[1];
    mint2 v2 = (mint2(r2) - r0 - mint2(fftprimes[0]) * v1) * m01_inv_m2;
    return (r0 + 1LL * fftprimes[0] * v1 + m01 % mod * v2.value) % mod;
}

template <const int32_t MOD>
std::vector<modint<MOD>> fftconv(std::vector<modint<MOD>> a, std::vector<modint<MOD>> b, bool skip_garner) {
    if (a.empty() or b.empty()) return {};
    int n = a.size(), m = b.size();
    int sz = (1 << (32 - __builtin_clz(n + m)));
    if (skip_garner or std::find(fftprimes, fftprimes + 3, MOD) != fftprimes + 3) {
        a.resize(sz), b.resize(sz);
        
        if (a == b) { fft(a, false); b = a; }
        else { fft(a, false); fft(b, false); }
        
        for (int i = 0; i < sz; i++) a[i] *= b[i];
        fft(a, true);
        a.resize(n + m - 1);
    }
    else {
        std::vector<int> ai(sz), bi(sz);
        for (int i = 0; i < n; i++) ai[i] = a[i].value;
        for (int i = 0; i < m; i++) bi[i] = b[i].value;
        std::vector<modint<MOD>> __fft[3];
        
        auto fft0 = fftconv_<fftprimes[0]>(ai, bi);
        auto fft1 = fftconv_<fftprimes[1]>(ai, bi);
        auto fft2 = fftconv_<fftprimes[2]>(ai, bi);
        
        a.resize(n + m - 1);
        for (int i = 0; i < n + m - 1; i++) {
            a[i] = garner_fft_(fft0[i].value, fft1[i].value, fft2[i].value, MOD);
        }
    }
    return a;
}

using mint = modint<998244353>;

int main() {
    std::vector<mint> A = {1, 4, 2, 7};
    std::vector<mint> B = {3, 2, 1};

    for (mint i : fftconv(A, B)) {
        std::cout << i << ' ';
    }
    std::cout << std::endl;
}
