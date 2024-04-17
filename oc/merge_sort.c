typedef int       i32;  typedef unsigned int       u32;
typedef long long i64;  typedef unsigned long long u64;
// typedef __int128 i128;  typedef __uint128_t       u128;

typedef int sort_type;

// sorts the array in descending order and 
// Returns the number of inversions in the array
int merge_sort(sort_type *begin, sort_type *end) {
    int n = end - begin;
    if (n <= 1) { return 0; }
    sort_type *mid = begin + n / 2;
    int inv = merge_sort(begin, mid) + merge_sort(mid, end);
    sort_type *i = begin, *j = mid, *k = begin;
    sort_type temp[n];
    while (i < mid && j < end) {
        temp[k++ - begin] = (*i <= *j) ? *i : *j;
        inv += (mid - i) * (*i > *j);
        (*i <= *j) ? i++ : j++;
    }
    while (i < mid) { temp[k - begin] = *i; k++; i++; }
    while (j < end) { temp[k - begin] = *j; k++; j++; }
    for (sort_type *l = begin; l < end; l++) {
        *l = temp[l - begin];
    }
    return inv;
}

int main() {
    
}