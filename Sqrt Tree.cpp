#include <bits/stdc++.h>
// using namespace std;

using T = char;

T f(T a, T b);

class SqrtTree {
private:
    int n, lg;
    std::vector<int> clz, layers, onLayer;
    std::vector<T> v;
    std::vector<std::vector<T>> pref, suf, between;

    void build(int layer, int lBound, int rBound) {
        if (layer >= (int)layers.size()) { return; }
        int bSzLog = (layers[layer] + 1) >> 1;
        int bCntLog = layers[layer] >> 1;
        int bSz = (1 << bSzLog);
        int bCnt = 0;
        for (int l = lBound; l < rBound; l += bSz) {
            bCnt++;
            int r = std::min(l + bSz, rBound);
            pref[layer][l] = v[l];
            for (int i = l + 1; i < r; i++) {
                pref[layer][i] = f(pref[layer][i - 1], v[i]);
            }
            suf[layer][r - 1] = v[r - 1];
            for (int i = r - 2; i >= l; i--) {
                suf[layer][i] = f(v[i], suf[layer][i + 1]);
            }
            build(layer + 1, l, r);
        }
        for (int i = 0; i < bCnt; i++) {
            T ans = 0;
            for (int j = i; j < bCnt; j++) {
                T add = suf[layer][lBound + (j << bSzLog)];
                ans = (i == j) ? add : f(ans, add);
                between[layer][lBound + (i << bCntLog) + j] = ans;
            }
        }
    }

public:
    inline T query(int l, int r) {
        if (l == r) { return v[l]; }
        if (l + 1 == r) { return f(v[l], v[r]); }
        int layer = onLayer[clz[l ^ r]];
        int bSzLog = (layers[layer] + 1) >> 1;
        int bCntLog = layers[layer] >> 1;
        int lBound = (l >> layers[layer]) << layers[layer];
        int lBlock = ((l - lBound) >> bSzLog) + 1;
        int rBlock = ((r - lBound) >> bSzLog) - 1;
        T ans = suf[layer][l];
        if (lBlock <= rBlock) {
            ans = f(ans, between[layer][lBound + (lBlock << bCntLog) + rBlock]);
        }
        ans = f(ans, pref[layer][r]);
        return ans;
    }

    SqrtTree(const std::vector<T> &v) : n((int)v.size()), lg(32 - __builtin_clz(n - 1)), v(v), clz(1 << lg), onLayer(lg + 1) {
        clz[0] = 0;
        for (int i = 1; i < (int)clz.size(); i++) {
            clz[i] = clz[i >> 1] + 1;
        }
        int tlg = lg;
        while (tlg > 1) {
            onLayer[tlg] = (int)layers.size();
            layers.push_back(tlg);
            tlg = (tlg + 1) >> 1;
        }
        for (int i = lg - 1; i >= 0; i--) {
            onLayer[i] = std::max(onLayer[i], onLayer[i + 1]);
        }
        pref.assign(layers.size(), std::vector<T> (n));
        suf.assign(layers.size(), std::vector<T> (n));
        between.assign(layers.size(), std::vector<T> (1 << lg));
        build(0, 0, n);
    }
};