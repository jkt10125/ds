#include <stdlib.h> // for random number generator

typedef int       i32;  typedef unsigned int       u32;
typedef long long i64;  typedef unsigned long long u64;
typedef __int128 i128;  typedef __uint128_t       u128;

typedef u64 mont_type;
// must be large enough to store the product of two mont_type
typedef u128 mont_type2;

struct montgomery {
    mont_type n, nr, dn;
} mont;

void montgomery_init(mont_type n) {
    mont.n = n;
    mont.nr = 1;
    // for (int i = 0; i < 5; i++) {
    for (int i = 0; i < 6; i++) {
        mont.nr *= 2 - n * mont.nr;
    }
    mont.dn = (mont.n << 1);
}

// returns a number in the range [0, 2 * n - 2]
mont_type montgomery_reduce(mont_type2 x) {
    mont_type q = (mont_type) x * mont.nr;
    mont_type m = ((mont_type2) q * mont.n) >> 64;
    // mont_type m = ((mont_type2) q * mont.n) >> 32;
    return (x >> 64) + mont.n - m;
    // return (x >> 32) + mont.n - m;
}

mont_type montgomery_multiply(mont_type x, mont_type y) {
    return montgomery_reduce((mont_type2) x * y);
}

mont_type montgomery_transform(mont_type x) {
    return ((mont_type2) x << 64) % mont.n;
    // return ((mont_type2) x << 32) % mont.n;
}

mont_type montgomery_pow(mont_type a, mont_type b) {
    mont_type r = montgomery_transform(1);
    a = montgomery_transform(a);
    for (; b; b >>= 1) {
        if (b & 1) {
            r = montgomery_multiply(r, a);
        }
        a = montgomery_multiply(a, a);
    }
    return r;
}

mont_type montgomery_norm(mont_type x) {
    return x < mont.n ? x : x - mont.n;
}

int miller_rabin(mont_type n, const mont_type base[], int len) {
    mont_type d = n - 1;
    int ctr = __builtin_ctzll(d);
    d >>= ctr;
    for (int i = 0; i < len; i++) {
        if (n <= base[i]) { continue; }
        // mont_type a = base[i];
        mont_type x = montgomery_pow(base[i], d);
        mont_type y = montgomery_norm(montgomery_reduce(x));
        if (y == 1 || y == n - 1) { continue; }
        for (int j = 1; j < ctr; ++j) {
            x = montgomery_multiply(x, x);
            y = montgomery_reduce(x);
            if (y == n - 1) { break; }
        }
        if (y != n - 1) { return 0; }
    }
    return 1;
}

/* ---------- Prime Precomputation ---------- */
// #define sieve_N 10000007
// #define sieve_p_N 664579

// int spf[sieve_N], prime[sieve_p_N], cnt;

// void sieve() {
//     for (int i = 2; i < sieve_N; i++) {
//         if (!spf[i]) {
//             spf[i] = prime[cnt++] = i;
//         }
//         for (int j = 0; j < cnt && prime[j] <= spf[i] && i * prime[j] < sieve_N; j++) {
//             spf[i * prime[j]] = prime[j];
//         }
//     }
// }
/* ------------------------------------------ */

int is_prime(mont_type n) {
    // if (n < sieve_N) { return spf[n] == n; }
    if (n == 1 || (1 ^ n & 1)) { return n == 2; }
    montgomery_init(n);
    int pred = (n > 4759123140ull);
    mont_type base[] = {2, 7, 61, 2, 325, 9375, 28178, 450775, 9780504, 1795265022};
    return miller_rabin(n, base + pred * 3, 3 + pred * 4);
}

u64 dist(u64 a, u64 b) {
    return a > b ? a - b : b - a;
}

u64 binary_gcd(u64 a, u64 b) {
    if (!a || !b) return (a ^ b);
    u64 shift = __builtin_ctzll(a | b);
    b >>= __builtin_ctzll(b);
    while (a) {
        a >>= __builtin_ctzll(a);
        u64 diff = dist(a, b);
        b = (a < b) ? a : b;
        a = diff;
    }
    return b << shift;
}

u64 pollard_rho(u64 n) {
    if (1 ^ n & 1) return 2;
    u64 x = rand() % n;
    const int M = 128;
    for (int l = M;; l <<= 1) {
        u64 y = x;
        for (int i = 0; i < l; i += M) {
            u64 z = x, p = 1;
            for (int j = 0; j < M; ++j) {
                x = montgomery_multiply(x, x) + 1;
                p = montgomery_multiply(p, dist(x, y));
            }
            u64 g = binary_gcd(p, n);
            if (g != 1) {
                for (int j = 0; j < M; ++j) {
                    z = montgomery_multiply(z, z) + 1;
                    g = binary_gcd(montgomery_norm(dist(z, y)), n);
                    if (g != 1) return g;
                }
            }
        }
    }
    // unreachable section
    return 0;
}

u64 find_factor(u64 n) {
    // if (n < sieve_N) { return spf[n]; }
    if (is_prime(n)) return n;
    u64 d; do { d = pollard_rho(n); } while (d == n);
    return d;
}

u64 factor[100], factor_ctr;

void factorize(u64 n) {
    if (n == 1) return;
    if (is_prime(n)) {
        factor[factor_ctr++] = n;
        return;
    }
    u64 d = find_factor(n);
    factorize(d);
    factorize(n / d);
}

int main() {
    
}