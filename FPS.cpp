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

template <const int32_t MOD>
struct FormalPowerSeries : std::vector<modint<MOD>> {
    using std::vector<modint<MOD>>::vector;

    void shrink() { while (this->size() and this->back() == modint<MOD>(0)) this->pop_back(); }

    FormalPowerSeries operator+(const FormalPowerSeries &r) const { return FormalPowerSeries(*this) += r; }
    FormalPowerSeries operator-(const FormalPowerSeries &r) const { return FormalPowerSeries(*this) -= r; }
    FormalPowerSeries operator*(const FormalPowerSeries &r) const { return FormalPowerSeries(*this) *= r; }
    FormalPowerSeries operator/(const FormalPowerSeries &r) const { return FormalPowerSeries(*this) /= r; }
    FormalPowerSeries operator%(const FormalPowerSeries &r) const { return FormalPowerSeries(*this) %= r; }
    
    FormalPowerSeries operator+(const modint<MOD> &r) const { return FormalPowerSeries(*this) += r; }
    FormalPowerSeries operator-(const modint<MOD> &r) const { return FormalPowerSeries(*this) -= r; }
    FormalPowerSeries operator*(const modint<MOD> &r) const { return FormalPowerSeries(*this) *= r; }
    FormalPowerSeries operator/(const modint<MOD> &r) const { return FormalPowerSeries(*this) /= r; }
    
    FormalPowerSeries &operator+=(const FormalPowerSeries &r) {
        if (this->size() < r.size()) this->resize(r.size());
        for (int i = 0; i < int(r.size()); i++) (*this)[i] += r[i];
        shrink(); return *this;
    }

    FormalPowerSeries &operator+=(const modint<MOD> &r) {
        if (this->empty()) this->resize(1);
        (*this)[0] += r; shrink();
        return *this;
    }

    FormalPowerSeries &operator-=(const FormalPowerSeries &r) {
        if (r.size() > this->size()) this->resize(r.size());
        for (int i = 0; i < (int)r.size(); i++) (*this)[i] -= r[i];
        shrink(); return *this;
    }
    
    FormalPowerSeries &operator-=(const modint<MOD> &v) {
        if (this->empty()) this->resize(1);
        (*this)[0] -= v; shrink();
        return *this;
    }
    
    FormalPowerSeries &operator*=(const modint<MOD> &v) {
        for (auto &x : (*this)) x *= v;
        shrink(); return *this;
    }
    
    FormalPowerSeries &operator*=(const FormalPowerSeries &r) {
        if (this->empty() || r.empty())
            this->clear();
        else {
            auto ret = fftconv(*this, r);
            *this = FormalPowerSeries(ret.begin(), ret.end());
        }
        shrink(); return *this;
    }
    
    FormalPowerSeries &operator%=(const FormalPowerSeries &r) {
        *this -= *this / r * r;
        shrink(); return *this;
    }
    
    FormalPowerSeries operator-() const {
        FormalPowerSeries ret = *this;
        for (auto &v : ret) v = -v;
        return ret;
    }
    
    FormalPowerSeries &operator/=(const modint<MOD> &v) {
        assert(v != modint<MOD>(0));
        for (auto &x : (*this)) x /= v;
        return *this;
    }
    
    FormalPowerSeries &operator/=(const FormalPowerSeries &r) {
        if (this->size() < r.size()) {
            this->clear();
            return *this;
        }
        int n = (int)this->size() - r.size() + 1;
        return *this = (reversed().pre(n) * r.reversed().inv(n)).pre(n).reversed(n);
    }
    
    FormalPowerSeries pre(int sz) const {
        FormalPowerSeries ret(this->begin(), this->begin() + std::min((int)this->size(), sz));
        ret.shrink(); return ret;
    }
    
    FormalPowerSeries operator>>(int sz) const {
        if ((int)this->size() <= sz) return {};
        return FormalPowerSeries(this->begin() + sz, this->end());
    }
    
    FormalPowerSeries operator<<(int sz) const {
        if (this->empty()) return {};
        FormalPowerSeries ret(*this);
        ret.insert(ret.begin(), sz, modint<MOD>(0));
        return ret;
    }

    FormalPowerSeries reversed(int deg = -1) const {
        assert(deg >= -1);
        FormalPowerSeries ret(*this);
        if (deg != -1) ret.resize(deg, modint<MOD>(0));
        reverse(ret.begin(), ret.end());
        ret.shrink(); return ret;
    }

    FormalPowerSeries differential() const { // formal derivative (differential) of f.p.s.
        const int n = (int)this->size();
        FormalPowerSeries ret(std::max(0, n - 1));
        for (int i = 1; i < n; i++) ret[i - 1] = (*this)[i] * modint<MOD>(i);
        return ret;
    }

    FormalPowerSeries integral() const { // computes integration of f.p.s
        const int n = (int)this->size();
        FormalPowerSeries ret(n + 1);
        ret[0] = modint<MOD>(0);
        for (int i = 0; i < n; i++) ret[i + 1] = (*this)[i] / modint<MOD>(i + 1);
        return ret;
    }

    FormalPowerSeries inv(int deg) const { // computes 1 / F(x) upto the given degree
        assert(deg >= -1);
        assert(this->size() and ((*this)[0]) != modint<MOD>(0)); // Requirement: F(0) != 0
        const int n = this->size();
        if (deg == -1) deg = n;
        FormalPowerSeries ret({modint<MOD>(1) / (*this)[0]});
        for (int i = 1; i < deg; i <<= 1) {
            auto h = (pre(i << 1) * ret).pre(i << 1) >> i;
            auto tmp = (-h * ret).pre(i);
            ret.insert(ret.end(), tmp.begin(), tmp.end());
            ret.resize(i << 1);
        }
        ret = ret.pre(deg);
        ret.shrink(); return ret;
    }

    FormalPowerSeries log(int deg = -1) const { // computes log(F(x)) upto the given degree
        assert(deg >= -1);
        assert(this->size() and ((*this)[0]) == modint<MOD>(1)); // Requirement: F(0) = 1
        const int n = (int)this->size();
        if (deg == 0) return {};
        if (deg == -1) deg = n;
        return (this->differential() * this->inv(deg)).pre(deg - 1).integral();
    }

    FormalPowerSeries sqrt(int deg = -1) const { // computes sqrt(F(x)) upto the given degree
        assert(deg >= -1);
        const int n = (int)this->size();
        if (deg == -1) deg = n;
        if (this->empty()) return {};
        if ((*this)[0] == modint<MOD>(0)) {
            for (int i = 1; i < n; i++)
                if ((*this)[i] != modint<MOD>(0)) {
                    if ((i & 1) or deg - i / 2 <= 0) return {};
                    return (*this >> i).sqrt(deg - i / 2) << (i / 2);
                }
            return {};
        }
        modint<MOD> sqrtf0 = modint<MOD>((*this)[0].value).sqrt();
        if (sqrtf0 == modint<MOD>(0)) return {};

        FormalPowerSeries y = (*this) / (*this)[0], ret({modint<MOD>(1)});
        modint<MOD> inv2 = modint<MOD>(1) / modint<MOD>(2);
        for (int i = 1; i < deg; i <<= 1) ret = (ret + y.pre(i << 1) * ret.inv(i << 1)) * inv2;
        return ret.pre(deg) * sqrtf0;
    }

    FormalPowerSeries exp(int deg = -1) const { // computes e^(F(x)) upto the given degree
        assert(deg >= -1);
        assert(this->empty() or ((*this)[0]) == modint<MOD>(0)); // Requirement: F(0) = 0
        const int n = (int)this->size();
        if (deg == -1) deg = n;
        FormalPowerSeries ret({modint<MOD>(1)});
        for (int i = 1; i < deg; i <<= 1) {
            ret = (ret * (pre(i << 1) + modint<MOD>(1) - ret.log(i << 1))).pre(i << 1);
        }
        return ret.pre(deg);
    }

    FormalPowerSeries pow(int64_t k, int deg = -1) const { // computes (F(x))^k upto the given degree
        assert(deg >= -1);
        const int n = (int)this->size();
        if (deg == -1) deg = n;

        if (k == 0) {
            FormalPowerSeries ret(deg);
            if (deg >= 1) ret[0] = modint<MOD>(1);
            ret.shrink();
            return ret;
        }

        for (int i = 0; i < n; i++) {
            if ((*this)[i] != modint<MOD>(0)) {
                modint<MOD> rev = modint<MOD>(1) / (*this)[i];
                FormalPowerSeries C = (*this) * rev, D(n - i);
                for (int j = i; j < n; j++) D[j - i] = C.coeff(j);
                D = (D.log(deg) * modint<MOD>(k)).exp(deg) * (*this)[i].pow(k);
                if (__int128(k) * i > deg) return {};
                FormalPowerSeries E(deg);
                long long S = i * k;
                for (int j = 0; j + S < deg and j < (int)D.size(); j++) E[j + S] = D[j];
                E.shrink();
                return E;
            }
        }
        return *this;
    }

    // Calculate F(X + c) from F(X), O(NlogN)
    FormalPowerSeries shift(modint<MOD> c) const {
        const int n = (int)this->size();
        FormalPowerSeries ret = *this;
        for (int i = 0; i < n; i++) ret[i] *= modint<MOD>(i).fact();
        std::reverse(ret.begin(), ret.end());
        FormalPowerSeries exp_cx(n, 1);
        for (int i = 1; i < n; i++) exp_cx[i] = exp_cx[i - 1] * c / i;
        ret = (ret * exp_cx), ret.resize(n);
        std::reverse(ret.begin(), ret.end());
        for (int i = 0; i < n; i++) ret[i] /= modint<MOD>(i).fact();
        return ret;
    }

    modint<MOD> coeff(int i) const { // returns the coefficient of x^i
        if ((int)this->size() <= i or i < 0) return modint<MOD>(0);
        return (*this)[i];
    }

    modint<MOD> eval(modint<MOD> x) const { // evaluates the whole expression for the given constant x
        modint<MOD> ret = 0, w = 1;
        for (auto &v : *this) ret += w * v, w *= x;
        return ret;
    }
};


using mint = modint<998244353>;
using fps = FormalPowerSeries<998244353>;

int main() {

    fps A = {0, 0, 0, 0, 0, 4};
    fps B = {4, 1};

    std::cout << mint(9).doublefac();
    // std::vector<mint> A = {2, 4, 5, 1}, B = A;
    // std::reverse(B.begin(), B.end());
    // B = {1, 2};

    auto c = A * B;

    std::cerr << c.coeff(3) * 6;
    // for (mint i : c) std::cout << i << std::endl;
}
