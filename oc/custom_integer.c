typedef int       i32;  typedef unsigned int       u32;
typedef long long i64;  typedef unsigned long long u64;
// typedef __int128 i128;  typedef __uint128_t       u128;

int mod = 998244353;

int add(int a, int b) { return (a + b) % mod; }

int sub(int a, int b) { return (a - b + mod) % mod; }

int mul(int a, int b) { return (a * 1ll * b) % mod; }

int power(int a, int b) {
    int res = 1;
    while (b) {
        if (b & 1) { res = mul(res, a); }
        a = mul(a, a);
        b >>= 1;
    }
    return res;
}

#define MAX_FACT 1000001

int fact[MAX_FACT], invfact[MAX_FACT], inv[MAX_FACT];

void fact_init() {
    inv[0] = fact[0] = 1;
    for (int i = 1; i < MAX_FACT; i++) {
        fact[i] = mul(fact[i - 1], i);
    }
    invfact[MAX_FACT - 1] = power(fact[MAX_FACT - 1], mod - 2);
    for (int i = MAX_FACT - 2; i >= 0; i--) {
        invfact[i] = mul(invfact[i + 1], i + 1);
        inv[i + 1] = mul(invfact[i + 1], fact[i]);
    }
}

int inverse(int a) { return (a < MAX_FACT) ? inv[a] : power(a, mod - 2); }

int div(int a, int b) { return mul(a, inverse(b)); }

int main() {
    fact_init();
    return 0;
}