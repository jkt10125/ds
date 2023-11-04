#include <bits/stdc++.h>
using namespace std;

const int BASE = 26;
const int LEN = 1048576;

/**
 * Assumptions:
 *  - LEN is a power of 2 (while using NTT)
*/

// template <const int BASE, const size_t LEN>
struct ArbitraryBaseInt : std::vector<int> {
    bool NEGATIVE;
    ArbitraryBaseInt() : std::vector<int> (LEN), NEGATIVE(false) {}


    ArbitraryBaseInt operator + (const ArbitraryBaseInt &rhs) const {
        ArbitraryBaseInt res = *this;
        for (int i = 0, c = 0; i < LEN; i++) {
            res[i] += c + rhs[i];
            (res[i] < BASE) ? c = 0 : (res[i] -= BASE, c = 1);
        }
        return res;
    }


    ArbitraryBaseInt operator - (const ArbitraryBaseInt &rhs) const {
        ArbitraryBaseInt res = *this;
        for (int i = 0, c = 0; i < LEN; i++) {
            res[i] -= c + rhs[i];
            (res[i] < 0) ? (res[i] += BASE, c = 1) : c = 0;
        }
        return res;
    }

    ArbitraryBaseInt operator * (ArbitraryBaseInt rhs) const {
        ArbitraryBaseInt res = *this;
        const int mod = 7340033;
        const int root = 5;
        const int root_1 = 4404020;
        const int len_1 = 7340026;

        auto ntt = [&] (ArbitraryBaseInt &A, bool inv) {
            for (int i = 1, j = 0; i < LEN; i++) {
                int bit = LEN >> 1;
                for (; j & bit; bit >>= 1) {
                    j ^= bit;
                }
                j ^= bit;
                if (i < j) {
                    std::swap(A[i], A[j]);
                }
            }
            for (int len = 2; len <= LEN; len <<= 1) {
                int wlen = inv ? root_1 : root;
                for (int i = len; i < LEN; i <<= 1) {
                    wlen = (int)(1LL * wlen * wlen % mod);
                }
                for (int i = 0; i < LEN; i += len) {
                    int w = 1;
                    for (int j = 0; j < (len >> 1); j++) {
                        int u = A[i + j], v = (int) (1LL * A[i + j + (len >> 1)] * w % mod);
                        A[i + j] = u + v < mod ? u + v : u + v - mod;
                        A[i + j + (len >> 1)] = u < v ? u - v + mod : u - v;
                        w = (int) (1LL * w * wlen % mod);
                    }
                }
            }
            if (inv) {
                int n_inv = len_1; // must be inverse of LEN
                for (int &v : A) {
                    v = (int) (1LL * v * n_inv % mod);
                }
            }
        };
        ntt(res, false); ntt(rhs, false);
        for (int i = 0; i < LEN; i++) {
            res[i] = (int) (1LL * res[i] * rhs[i] % mod);
        }
        ntt(res, true);
        for (int i = 0, c = 0; i < LEN; i++) {
            res[i] += c;
            (res[i] < BASE) ? c = 0 : (c = res[i] / BASE, res[i] %= BASE);
        }
        return res;
    }

    /* Assumption: res < BASE */
    ArbitraryBaseInt operator + (const int &rhs) const {
        ArbitraryBaseInt res = *this;    
        for (int i = 0, c = rhs; i < LEN && c; i++) {
            res[i] += c;
            (res[i] < BASE) ? c = 0 : (res[i] -= BASE, c = 1);
        }
        return res;
    }

    ArbitraryBaseInt operator - (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        for (int i = 0, c = rhs; i < LEN && c; i++) {
            res[i] -= c;
            (res[i] < 0) ? (res[i] += BASE, c = 1) : c = 0;
        }
        return res;
    }

    ArbitraryBaseInt operator * (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        for (int i = 0, c = 0; i < LEN; i++) {
            res[i] = res[i] * rhs + c;
            (res[i] < BASE) ? c = 0 : (res[i] -= BASE, c = 1);
        }
        return res;
    }

    ArbitraryBaseInt operator / (const int &rhs) const {
        ArbitraryBaseInt res = *this;
        for (int i = LEN - 1, c = 0; i >= 0; i--) {
            res[i] += c * BASE;
            c = res[i] % rhs;
            res[i] /= rhs;
        }
        return res;
    }

    
};

int main() {
    // vector<int> a(LEN), b(LEN);

    // for (int i = 0; i < 9; i++) {
    //     a[i] = b[i] = 1;
    // }

    // ArbitraryBaseInt base;

    // base.fft(a, false);
    // base.fft(b, false);

    // for (int i = 0; i < LEN; i++) {
    //     a[i] = (int) (1LL * a[i] * b[i] % base.mod);
    // }

    // base.fft(a, true);

    // for (int i = 0; i < 20; i++) {
    //     cout << a[i] << " ";
    // }

    ArbitraryBaseInt a, b;
    for (int i = 0; i < 9; i++) {
        a[i] = b[i] = 1;
    }

    ArbitraryBaseInt c = a * b;

    for (int i = 0; i < 20; i++) {
        cout << c[i] << " ";
    }

    return 0;
}