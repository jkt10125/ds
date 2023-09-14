#include <bits/stdc++.h>

// for longest non decreasing subsequence, change the lower_bound to upper_bound
// NOTE : the vector lis is not the longest increasing subsequence, but the length of lis is the length of longest increasing subsequence
// one counter example is [2, 3, 1] : lis according to the function is : [1, 3], but the longest increasing subsequence is [2, 3]
int LongestIncreasingSubsequence(const std::vector<int> &A) {
    std::vector<int> lis;
    for (int i : A) {
        std::vector<int>::iterator it = std::lower_bound(lis.begin(), lis.end(), i);
        if (it == lis.end()) { lis.push_back(i); }
        else { *it = i; }
    }
    return lis.size();
}

int main() {
    
}
