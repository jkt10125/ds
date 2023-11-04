#include <bits/stdc++.h>
using namespace std;

// const int BASE = 26;
// const int LEN = 100;

template <const int BASE, const size_t LEN>
struct ArbitraryBaseInt : std::vector<int> {
    bool NEGATIVE;
    ArbitraryBaseInt() : std::vector<int> (LEN), NEGATIVE(false) {}

    ArbitraryBaseInt operator + (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = rhs;
        for (int i = 0; i < LEN; i++) {
            res[i] += carry;
            (res[i] < BASE) ? carry = 0 : (res[i] -= BASE, carry = 1);
            if (carry == 0) break;
        }

        return res;
    }

    ArbitraryBaseInt operator + (const ArbitraryBaseInt &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = 0;
        for (int i = 0; i < LEN; i++) {
            res[i] += carry + rhs[i];
            (res[i] < BASE) ? carry = 0 : (res[i] -= BASE, carry = 1);
        }

        return res;
    }

    ArbitraryBaseInt operator - (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = rhs;
        for (int i = 0; i < LEN; i++) {
            res[i] -= carry;
            (res[i] < 0) ? (res[i] += BASE, carry = 1) : carry = 0;
            if (carry == 0) break;
        }

        return res;
    
    }

    ArbitraryBaseInt operator - (const ArbitraryBaseInt &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = 0;
        for (int i = 0; i < LEN; i++) {
            res[i] -= carry + rhs[i];
            (res[i] >= 0) ? carry = 0 : (res[i] += BASE, carry = 1);
        }

        return res;
    }

    ArbitraryBaseInt operator * (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = 0;
        for (int i = 0; i < LEN || carry; i++) {
            res[i] = res[i] * rhs + carry;
            (res[i] < BASE) ? carry = 0 : (res[i] -= BASE, carry = 1);
        }

        return res;
    }

    ArbitraryBaseInt operator / (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        
        int carry = 0;
        for (int i = LEN - 1; i >= 0; i--) {
            res[i] += carry * BASE;
            carry = res[i] % rhs;
            res[i] /= rhs;
        }

        return res;
    }
};

int main() {
    ArbitraryBaseInt<26, 100> a, b;

    for (int i = 0; i < 100; i++) {
        a[i] = 25;
    }
    a[99] = 0;

    a = a + 1;
    a = a - 1;

    for (int i = 0; i < 100; i++) {
        cout << a[i] << " ";
    }

    return 0;
}