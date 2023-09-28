#include <bits/stdc++.h>

std::vector<int> spf, prime;

// O(n) sieve
void sieve(int n) {
    spf.assign(n + 1, 0);
    for (int i = 2; i <= n; i++) {
        if (!spf[i]) { spf[i] = i, prime.push_back(i); }
        for (int p : prime) { 
            if (p > spf[i] || i * p > n) break; 
            spf[i * p] = p; 
        }
    }
}

// prime must contain primes atleast upto sqrt(n)
bool isPrime (int n) {
    if (n < spf.size()) { return (spf[n] == n); }
    for(int i : prime) { 
        if (i * i > n) break; 
        if (n % i == 0) return false; 
    } 
    return true;
}

// prime must contain primes atleast upto sqrt(n)
std::vector<int> factor(int n) {
    std::vector<int> factr;
    if (n < spf.size()) {
        while (n > 1) { factr.push_back(spf[n]); n /= spf[n]; }
    }
    else {
        for (int i : prime) {
            if (i * i > n) break;
            while (n % i == 0) { factr.push_back(i); n /= i; }
        }
        if (n > 1) factr.push_back(n);
    }
    return factr;
}

int phi(int n) {
    std::vector<int> factr = factor(n);
    factr.erase(std::unique(factr.begin(), factr.end()), factr.end());

    for (int i : factr) n -= n / i;
    return n;
}

// divisor code for applying inclusion-exclusion principle
std::vector<std::array<int, 2>> divisor(int n) {
    std::vector<int> fact = factor(n);
    fact.erase(std::unique(fact.begin(), fact.end()), fact.end());
    std::vector<std::array<int, 2>> div(1, {1, 1});
    for (int i : fact) {
        int sz = div.size();
        for (int j = 0; j < sz; j++) {
            auto [x, y] = div[j];
            div.push_back({x * i, -y});
        }
    }
    return div;
}

int main() {
    sieve(1000);
    
    for (auto [x, y] : divisor(8)) {
        std::cout << x << " " << y << "\n";
    }
}
