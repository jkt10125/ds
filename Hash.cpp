#include <bits/stdc++.h>

template <const int32_t MOD>
struct modint {
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

template <const int32_t MOD>
struct Hash {
    std::deque<int> DQ;
    struct Hash_DT {
        modint<1000000007> d0;
        modint<1000000009> d1;
        modint<1000000021> d2;
        modint<1000000033> d3;
        modint<1000000087> d4;
        modint<1000000093> d5;
        modint<1000000097> d6;
        modint<1000000103> d7;
        
        Hash_DT(int32_t x = 0) : d0(x), d1(x), d2(x), d3(x), d4(x), d5(x), d6(x), d7(x) {}
    } H, rH, M;

    Hash(const std::deque<int> dq) : DQ(dq), H(0), rH(0), M(MOD) {
        for (int i = 0; i < (int) DQ.size(); i++) {
            H.d0 = H.d0 * M.d0 + DQ[i];
            H.d1 = H.d1 * M.d1 + DQ[i];
            H.d2 = H.d2 * M.d2 + DQ[i];
            H.d3 = H.d3 * M.d3 + DQ[i];
            H.d4 = H.d4 * M.d4 + DQ[i];
            H.d5 = H.d5 * M.d5 + DQ[i];
            H.d6 = H.d6 * M.d6 + DQ[i];
            H.d7 = H.d7 * M.d7 + DQ[i];

            rH.d0 = rH.d0 * M.d0 + DQ[DQ.size() - i - 1];
            rH.d1 = rH.d1 * M.d1 + DQ[DQ.size() - i - 1];
            rH.d2 = rH.d2 * M.d2 + DQ[DQ.size() - i - 1];
            rH.d3 = rH.d3 * M.d3 + DQ[DQ.size() - i - 1];
            rH.d4 = rH.d4 * M.d4 + DQ[DQ.size() - i - 1];
            rH.d5 = rH.d5 * M.d5 + DQ[DQ.size() - i - 1];
            rH.d6 = rH.d6 * M.d6 + DQ[DQ.size() - i - 1];
            rH.d7 = rH.d7 * M.d7 + DQ[DQ.size() - i - 1];
        }
    }

    void push_back(int x) {
        H.d0 = H.d0 * M.d0 + x;
        H.d1 = H.d1 * M.d1 + x;
        H.d2 = H.d2 * M.d2 + x;
        H.d3 = H.d3 * M.d3 + x;
        H.d4 = H.d4 * M.d4 + x;
        H.d5 = H.d5 * M.d5 + x;
        H.d6 = H.d6 * M.d6 + x;
        H.d7 = H.d7 * M.d7 + x;

        rH.d0 = rH.d0 + x * M.d0.pow(DQ.size());
        rH.d1 = rH.d1 + x * M.d1.pow(DQ.size());
        rH.d2 = rH.d2 + x * M.d2.pow(DQ.size());
        rH.d3 = rH.d3 + x * M.d3.pow(DQ.size());
        rH.d4 = rH.d4 + x * M.d4.pow(DQ.size());
        rH.d5 = rH.d5 + x * M.d5.pow(DQ.size());
        rH.d6 = rH.d6 + x * M.d6.pow(DQ.size());
        rH.d7 = rH.d7 + x * M.d7.pow(DQ.size());

        DQ.push_back(x);
    }

    void push_front(int x) {
        H.d0 = H.d0 + x * M.d0.pow(DQ.size());
        H.d1 = H.d1 + x * M.d1.pow(DQ.size());
        H.d2 = H.d2 + x * M.d2.pow(DQ.size());
        H.d3 = H.d3 + x * M.d3.pow(DQ.size());
        H.d4 = H.d4 + x * M.d4.pow(DQ.size());
        H.d5 = H.d5 + x * M.d5.pow(DQ.size());
        H.d6 = H.d6 + x * M.d6.pow(DQ.size());
        H.d7 = H.d7 + x * M.d7.pow(DQ.size());

        rH.d0 = rH.d0 * M.d0 + x;
        rH.d1 = rH.d1 * M.d1 + x;
        rH.d2 = rH.d2 * M.d2 + x;
        rH.d3 = rH.d3 * M.d3 + x;
        rH.d4 = rH.d4 * M.d4 + x;
        rH.d5 = rH.d5 * M.d5 + x;
        rH.d6 = rH.d6 * M.d6 + x;
        rH.d7 = rH.d7 * M.d7 + x;

        DQ.push_front(x);
    }

    void pop_back() {
        int x = DQ.back();
        DQ.pop_back();

        H.d0 = (H.d0 - x) / M.d0;
        H.d1 = (H.d1 - x) / M.d1;
        H.d2 = (H.d2 - x) / M.d2;
        H.d3 = (H.d3 - x) / M.d3;
        H.d4 = (H.d4 - x) / M.d4;
        H.d5 = (H.d5 - x) / M.d5;
        H.d6 = (H.d6 - x) / M.d6;
        H.d7 = (H.d7 - x) / M.d7;

        rH.d0 = (rH.d0 - x * M.d0.pow(DQ.size()));
        rH.d1 = (rH.d1 - x * M.d1.pow(DQ.size()));
        rH.d2 = (rH.d2 - x * M.d2.pow(DQ.size()));
        rH.d3 = (rH.d3 - x * M.d3.pow(DQ.size()));
        rH.d4 = (rH.d4 - x * M.d4.pow(DQ.size()));
        rH.d5 = (rH.d5 - x * M.d5.pow(DQ.size()));
        rH.d6 = (rH.d6 - x * M.d6.pow(DQ.size()));
        rH.d7 = (rH.d7 - x * M.d7.pow(DQ.size()));
    }

    void pop_front() {
        int x = DQ.front();
        DQ.pop_front();

        H.d0 = (H.d0 - x * M.d0.pow(DQ.size()));
        H.d1 = (H.d1 - x * M.d1.pow(DQ.size()));
        H.d2 = (H.d2 - x * M.d2.pow(DQ.size()));
        H.d3 = (H.d3 - x * M.d3.pow(DQ.size()));
        H.d4 = (H.d4 - x * M.d4.pow(DQ.size()));
        H.d5 = (H.d5 - x * M.d5.pow(DQ.size()));
        H.d6 = (H.d6 - x * M.d6.pow(DQ.size()));
        H.d7 = (H.d7 - x * M.d7.pow(DQ.size()));

        rH.d0 = (rH.d0 - x) / M.d0;
        rH.d1 = (rH.d1 - x) / M.d1;
        rH.d2 = (rH.d2 - x) / M.d2;
        rH.d3 = (rH.d3 - x) / M.d3;
        rH.d4 = (rH.d4 - x) / M.d4;
        rH.d5 = (rH.d5 - x) / M.d5;
        rH.d6 = (rH.d6 - x) / M.d6;
        rH.d7 = (rH.d7 - x) / M.d7;
    }
};

int main() {

    Hash<31> H({});
    int type;
    while (true) {
        std::cin >> type;

        if (type == 1) {
            int x;
            std::cin >> x;
            H.push_back(x);
        } else if (type == 2) {
            int x;
            std::cin >> x;
            H.push_front(x);
        } else if (type == 3) {
            H.pop_back();
        } else if (type == 4) {
            H.pop_front();
        } else if (type == 5) {
            // std::cout << H.is_palindrome() << std::endl;
        } else if (type == 6) {
            std::cout << H.DQ.size() << std::endl;
        } else if (type == 7) {
            for (int i = 0; i < (int) H.DQ.size(); i++) {
                std::cout << H.DQ[i] << " \n"[i == (int) H.DQ.size() - 1];
            }
        } else if (type == 8) {
            std::cout << H.H.d0 << " " << H.H.d1 << " " << H.H.d2 << " " << H.H.d3 << " " << H.H.d4 << " " << H.H.d5 << " " << H.H.d6 << " " << H.H.d7 << std::endl;
        } else if (type == 9) {
            std::cout << H.rH.d0 << " " << H.rH.d1 << " " << H.rH.d2 << " " << H.rH.d3 << " " << H.rH.d4 << " " << H.rH.d5 << " " << H.rH.d6 << " " << H.rH.d7 << std::endl;
        }
    }
}