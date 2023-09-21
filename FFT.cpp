#include <bits/stdc++.h>

#define int long long

template <const int32_t MOD>
class modint {
    
    static int primitive_root;
    static std::vector<modint<MOD>> Fact, iFact, Inv;
    
    public:
    int32_t value;
    modint() : value(0) {}
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
    inline modint<MOD> &operator /= (modint<MOD> other) { return *this *= other.inv(); }
    inline bool operator == (modint<MOD> other) const { return value == other.value; }
    inline bool operator != (modint<MOD> other) const { return value != other.value; }
    inline bool operator < (modint<MOD> other) const { return value < other.value; }
    inline bool operator > (modint<MOD> other) const { return value > other.value; }

    static void precompute(int n) {
        int m = Fact.size();
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
            primitive_root = -1;
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
template <const int32_t MOD> std::ostream &operator << (std::ostream & out, const modint<MOD> &n) { return out << n.value; }

template <const int32_t MOD>
void ntt(std::vector<modint<MOD>> &a, bool inv) {

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
        
        for (modint<MOD> &v : a) {
            
            v *= n_inv;
        }
    }
}

void fft(std::vector<std::complex<long double>> &A, bool invert) {

    int n = A.size();

    for (int i = 1, j = 0; i < n; i++) {

        int bit = n >> 1;

        for (; j & bit; bit >>= 1) {
            
            j ^= bit;
        }

        j ^= bit;

        if (i < j) {
            
            std::swap(A[i], A[j]);
        }
    }

    for (int len = 2; len <= n; len <<= 1) {

        long double ang = 2 * M_PI / len * (invert ? -1 : 1);

        std::complex<long double> wlen(cos(ang), sin(ang));

        for (int i = 0; i < n; i += len) {

            std::complex<long double> w(1);

            for (int j = 0; j < len / 2; j++) {

                std::complex<long double> u = A[i + j], v = A[i + j + len / 2] * w;
                
                A[i + j] = u + v;
                
                A[i + j + len / 2] = u - v;
                
                w *= wlen;
            }
        }
    }

    if (invert) {
        
        for (std::complex<long double> &c : A) {

            c /= n;
        }
    }
}

template <const int32_t MOD>
std::vector<modint<MOD>> multiply(std::vector<modint<MOD>> A, std::vector<modint<MOD>> B) {

    int n = 1;
    while (n < A.size() + B.size()) {
        
        n <<= 1;
    }

    A.resize(n); B.resize(n);

    ntt(A, false); ntt(B, false);

    for (int i = 0; i < n; i++) {
        
        A[i] *= B[i];
    }

    ntt(A, true);

    return A;
}

std::vector<int> multiply(const std::vector<int> &A, const std::vector<int> &B) {

    int n = 1;
    while (n < A.size() + B.size()) {
        
        n <<= 1;
    }

    std::vector<std::complex<long double>> A_(n), B_(n), C_(n);

    for (int i = 0; i < A.size(); i++) {
        
        A_[i] = A[i];
    }

    for (int i = 0; i < B.size(); i++) {
        
        B_[i] = B[i];
    }

    fft(A_, false); fft(B_, false);

    for (int i = 0; i < n; i++) {
        
        C_[i] = A_[i] * B_[i];
    }

    fft(C_, true);

    std::vector<int> C(n);

    for (int i = 0; i < n; i++) {
        
        C[i] = round(C_[i].real());
    }

    return C;
}

using mint = modint<998244353>;

signed main() {

    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k, n, m;

    std::cin >> k >> n >> m;

    std::vector<int> A(k + 1), B(k + 1);

    for (int i = 0; i < n; i++) {

        int x;
        std::cin >> x;
        A[x] = A[x] + 1;
    }

    for (int i = 0; i < m; i++) {

        int x;
        std::cin >> x;
        B[x] = B[x] + 1;
    }

    auto C = multiply(A, B);

    for (int i = 2; i <= 2 * k; i++) {

        std::cout << C[i] << " ";
    }

    std::cout << "\n";
}