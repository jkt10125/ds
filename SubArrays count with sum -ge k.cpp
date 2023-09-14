#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;

template <typename T>
using ordered_set = tree<T, null_type, std::less<T>, rb_tree_tag, tree_order_statistics_node_update>;

long long countSubarray(std::vector<int> &A, int k) {

    ordered_set<std::pair<long long, int>> os;
    long long res = 0, prefixSum = 0;
    int n = A.size();

    os.insert({0, -1});

    for (int i = 0; i < n; i++) {

        prefixSum += A[i];
        res += os.order_of_key({prefixSum - k, n});
        
        os.insert({prefixSum, i});
    }

    return res;
}

int main() {

    int n, k;

    std::cin >> n >> k;

    std::vector<int> A(n), b(n), c(n);

    for (int i = 0; i < n; i++) {

        std::cin >> A[i];

        b[i] = (A[i] < k) ? -1 : +1;
        c[i] = (A[i] > k) ? +1 : -1;
    }

    std::cout << countSubarray(b, 1) - countSubarray(c, 1);

    return 0;
}