#include <bits/stdc++.h>

const size_t BIGBINARY_LIM = 1000;
class BigBinary : std::deque<bool> {
    public:

    BigBinary(unsigned long long n = 0) {
        resize(BIGBINARY_LIM, 0);
        for (size_t i = 0; i < BIGBINARY_LIM && n; i++) {
            (*this)[i] = n & 1;
            n >>= 1;
        }
    }

    bool &operator [] (size_t i) { return at(i); }
    bool  operator [] (size_t i) const { return at(i); }

    // O(BIGBINARY_LIM)
    BigBinary operator + (const BigBinary &rhs) const {
        BigBinary lhs = (*this);
        bool carry = 0;
        for (size_t i = 0; i < BIGBINARY_LIM; i++) {
            bool sum = (lhs[i] ^ rhs[i] ^ carry);
            carry = (lhs[i] & rhs[i]) | (lhs[i] & carry) | (rhs[i] & carry);
            lhs[i] = sum;
        }
        return lhs;
    }

    // O(BIGBINARY_LIM)
    BigBinary operator - () const {
        return ++(~(*this));
    }

    // O(BIGBINARY_LIM)
    BigBinary operator - (const BigBinary &rhs) const {
        return (*this) + (-rhs);
    }

    // O(BIGBINARY_LIM * BIGBINARY_LIM)
    BigBinary operator * (const BigBinary &rhs) const {
        BigBinary lhs = (*this), ans;
        for (size_t i = 0; i < BIGBINARY_LIM; i++) {
            if (rhs[i]) { ans = ans + lhs; }
            lhs = lhs << 1;
        }
        return ans;
    }

    // O(BIGBINARY_LIM * BIGBINARY_LIM * log(rhs))
    BigBinary operator / (const BigBinary &rhs) const {
        BigBinary ans, l, r = (*this);
        while (l <= r) {
            BigBinary m = (l + r) >> 1;
            if ((*this) < (m * rhs)) { r = --m; }
            else { ans = m; l = ++m; }
        }
        return ans;
    }

    // O(BIGBINARY_LIM * BIGBINARY_LIM * log(rhs))
    BigBinary operator % (const BigBinary &rhs) const {
        return (*this) - ((*this) / rhs) * rhs;
    }

    // O(BIGBINARY_LIM)
    BigBinary operator ^ (const BigBinary &rhs) const {
        BigBinary lhs = (*this);
        for (size_t i = 0; i < BIGBINARY_LIM; i++) {
            lhs[i] = lhs[i] ^ rhs[i];
        }
        return lhs;
    }

    // O(BIGBINARY_LIM)
    BigBinary operator & (const BigBinary &rhs) const {
        BigBinary lhs = (*this);
        for (size_t i = 0; i < BIGBINARY_LIM; i++) {
            lhs[i] = lhs[i] & rhs[i];
        }
        return lhs;
    }

    // O(BIGBINARY_LIM)
    BigBinary operator | (const BigBinary &rhs) const {
        BigBinary lhs = (*this);
        for (size_t i = 0; i < BIGBINARY_LIM; i++) {
            lhs[i] = lhs[i] | rhs[i];
        }
        return lhs;
    }

    // O(BIGBINARY_LIM)
    BigBinary operator ~ () const {
        BigBinary lhs = (*this);
        for (bool &b : lhs) { b = !b; }
        return lhs;
    }

    // O(n)
    BigBinary operator << (size_t n) const {
        BigBinary lhs = (*this);
        while (n--) {
            lhs.pop_back();
            lhs.push_front(0);
        }
        return lhs;
    }

    // O(n)
    BigBinary operator >> (size_t n) const {
        BigBinary lhs = (*this);
        while (n--) {
            lhs.pop_front();
            lhs.push_back(0);
        }
        return lhs;
    }

    // O(1) : Amortized
    BigBinary &operator ++ () {
        size_t i = 0;
        while (i < BIGBINARY_LIM && (*this)[i]) {
            (*this)[i] = 0;
            i++;
        }
        if (i < BIGBINARY_LIM) { (*this)[i] = 1; }
        return (*this);
    }

    // O(1) : Amortized
    BigBinary &operator ++ (int) {
        return ++(*this);
    }

    // O(1) : Amortized
    BigBinary &operator -- () {
        size_t i = 0;
        while (i < BIGBINARY_LIM && !(*this)[i]) {
            (*this)[i] = 1;
            i++;
        }
        if (i < BIGBINARY_LIM) { (*this)[i] = 0; }
        return (*this);
    }

    // O(1) : Amortized
    BigBinary &operator -- (int) {
        return --(*this);
    }

    // O(BIGBINARY_LIM)
    bool operator == (const BigBinary &rhs) const {
        for (int i = 0; i < BIGBINARY_LIM; i++) {
            if ((*this)[i] ^ rhs[i]) { return 0; }
        }
        return 1;
    }

    // O(BIGBINARY_LIM)
    bool operator != (const BigBinary &rhs) const {
        return !((*this) == rhs);
    }

    // O(BIGBINARY_LIM) : Only for unsigned
    bool operator < (const BigBinary &rhs) const {
        for (int i = BIGBINARY_LIM - 1; i >= 0; i--) {
            if ((*this)[i] ^ rhs[i]) { return rhs[i]; }
        }
        return 0;
    }

    // O(BIGBINARY_LIM) : Only for unsigned
    bool operator > (const BigBinary &rhs) const {
        return rhs < (*this);
    }

    // O(BIGBINARY_LIM) : Only for unsigned
    bool operator <= (const BigBinary &rhs) const {
        return !((*this) > rhs);
    }

    // O(BIGBINARY_LIM) : Only for unsigned
    bool operator >= (const BigBinary &rhs) const {
        return !((*this) < rhs);
    }

    // Output Stream
    friend std::ostream &operator << (std::ostream &os, const BigBinary &rhs) {
        for (bool i : rhs) { os << i; }
        return os;
    }

};



int main() {
    int t;
    std::cin >> t;
    while(t--) {
        int n;
        std::cin >> n;
        BigBinary a(1), b(n);
        while(b != 0) {
            a = a * b;
            --b;
        }
        // std::cout << a.toDecimal() << std::endl;
        std::cout << a << std::endl;
    }
    
    return 0;
}

