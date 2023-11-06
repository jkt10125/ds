#include <bits/stdc++.h>

int egcd(int a, int b, int &x, int &y) {
	if (!a && !b) { return (x = y = 0); }
	int q0, r0, r1, r2, x0, x1, x2;
	r2 = a, r1 = b; x2 = 1, x1 = 0;
	while (r1) {
		q0 = r2 / r1;
		r0 = r2 - q0 * r1; r2 = r1, r1 = r0;
		x0 = x2 - q0 * x1; x2 = x1, x1 = x0;
	}
	y = (b) ? (r2 - a * x2) / b : 0, x = x2;
	return r2;
}

// this is a function that returns the number of pairs (i, j) such that gcd(i, j) = k
// for all k in [1, m]
std::vector<int> func(const std::vector<int> &A) {
	int m = *std::max_element(A.begin(), A.end());
	std::vector<int> dp(m + 1, 0), H(m + 1, 0);
	for (int i : A) { H[i]++; }
	for (int i = 1; i <= m; i++) {
		for (int j = i; j <= m; j += i) {
			dp[j] += H[i];
		}
	}
	for (int i = m; i; i--) {
		dp[i] = dp[i] * 1LL * (dp[i] - 1) / 2;
		for (int j = i + i; j <= m; j += i) {
			dp[i] -= dp[j];
		}
	}
	return dp;
}

int main() {
	int a, b, x, y;
	while (scanf("%d %d", &a, &b) == 2) {
		int g = egcd(a, b, x, y);
		printf("%d %d %d\n", x, y, g);
	}
	return 0;
}