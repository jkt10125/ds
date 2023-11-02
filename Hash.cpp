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

            #define OP(idx) H.d##idx  = H.d##idx  * M.d##idx + DQ[i]
                OP(0); OP(1); OP(2); OP(3); OP(4); OP(5); OP(6); OP(7);
            #undef OP

            #define PO(idx) rH.d##idx = rH.d##idx * M.d##idx + DQ[DQ.size() - i - 1]
                PO(0); PO(1); PO(2); PO(3); PO(4); PO(5); PO(6); PO(7);
            #undef PO
        }
    }

    void push_back(int x) {

        #define OP(idx) H.d##idx  = H.d##idx * M.d##idx + x
            OP(0); OP(1); OP(2); OP(3); OP(4); OP(5); OP(6); OP(7);
        #undef OP

        #define PO(idx) rH.d##idx = rH.d##idx + x * M.d##idx.pow(DQ.size())
            PO(0); PO(1); PO(2); PO(3); PO(4); PO(5); PO(6); PO(7);
        #undef PO

        DQ.push_back(x);
    }

    void push_front(int x) {

        #define OP(idx) H.d##idx  = H.d##idx + x * M.d##idx.pow(DQ.size())
            OP(0); OP(1); OP(2); OP(3); OP(4); OP(5); OP(6); OP(7);
        #undef OP

        #define PO(idx) rH.d##idx = rH.d##idx * M.d##idx + x
            PO(0); PO(1); PO(2); PO(3); PO(4); PO(5); PO(6); PO(7);
        #undef PO

        DQ.push_front(x);
    }

    void pop_back() {

        int x = DQ.back();
        DQ.pop_back();

        #define OP(idx) H.d##idx = (H.d##idx - x) / M.d##idx
            OP(0); OP(1); OP(2); OP(3); OP(4); OP(5); OP(6); OP(7);
        #undef OP

        #define PO(idx) rH.d##idx = rH.d##idx - x * M.d##idx.pow(DQ.size())
            PO(0); PO(1); PO(2); PO(3); PO(4); PO(5); PO(6); PO(7);
        #undef PO
    }

    void pop_front() {

        int x = DQ.front();
        DQ.pop_front();

        #define OP(idx) H.d##idx = H.d##idx - x * M.d##idx.pow(DQ.size())
            OP(0); OP(1); OP(2); OP(3); OP(4); OP(5); OP(6); OP(7);
        #undef OP

        #define PO(idx) rH.d##idx = (rH.d##idx - x) / M.d##idx
            PO(0); PO(1); PO(2); PO(3); PO(4); PO(5); PO(6); PO(7);
        #undef PO
    }
};

int main() {

    Hash<31> H({});
    int type;
    while (true) {
        std::cin >> type;

        if (type == 10125) {
            std::cout << "HEY";
            break;
        } 

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