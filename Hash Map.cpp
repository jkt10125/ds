#include <bits/stdc++.h>

using namespace std;

template <typename K, typename V, const int32_t LG>
struct hash_map_open_addressing {
    const int N = (1 << LG);
    std::pair<K, V> *table = new std::pair<K, V> [N];
    std::vector<bool> used = std::vector<bool>(N, false);

    constexpr inline void next_pos(int32_t &pos) const {
        pos = pos < (N - 1) ? pos + 1 : 0;
    }

    int32_t hash(const K &key) const {
        return key & (N - 1);
        
        static constexpr uint64_t conf = 11995408973635179863ull;
        // return (uint64_t) key * conf >> (64 - LG);
    }

    void clear() {
        std::fill(used.begin(), used.end(), false);
    }

    inline int32_t find_pos(const K &key) const {
        int32_t p = hash(key), i = p;
        do {
            if (!used[p] || table[p].first == key) {
                return p;
            }
            next_pos(p);
        } while (i != p);
        return -1;
    }

    V &operator [] (const K &key) {
        int32_t p = find_pos(key);
        if (p < 0) {
            cout << "Hash Map full for key: " << key << "\n";
            return table[0].second;
        }
        if (used[p]) {
            return table[p].second;
        }
        used[p] = true;
        table[p].first = key;
        return table[p].second = V();
    }
};

int main(int argc, char *argv[]) {
    int type = atoi(argv[1]);

    const int N = 1000007;

    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    uniform_int_distribution<int> dist(0, 1000000007);

    if (type == 0) {
        
        map<int, string> mp;

        for (int i = 0; i < N; i++) {

            int type = dist(rng) & 1;
            
            if (type == 0) {
                mp[dist(rng)] = "ghjkt";
            }
            else {
                int x = dist(rng);
                string get_val = mp[x];
            }
        }
    }

    else if (type == 1) {

        unordered_map<int, string> mp;

        for (int i = 0; i < N; i++) {

            int type = dist(rng) & 1;
            
            if (type == 0) {
                mp[dist(rng)] = "ghjkt";
            }
            else {
                int x = dist(rng);
                string get_val = mp[x];
            }
        }
    }

    else {

        hash_map_open_addressing<int, string, 20> mp;

        int sum_dist = 0;

        for (int i = 0; i < N; i++) {

            int type = dist(rng) & 1;
            
            if (type == 0) {
                int x = dist(rng);
                mp[x] = "ghjkt";

                int pos = mp.find_pos(x);
                int hsh = mp.hash(x);

                sum_dist += (pos - hsh + mp.N) % mp.N;

            }
            else {
                int x = dist(rng);
                string get_val = mp[x];
            }
        }

        cout << setprecision(10) << fixed;

        cout << (double) sum_dist / mp.N << '\n';
    }
}