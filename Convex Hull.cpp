#include <bits/stdc++.h>

struct point {
    int x, y;
    point() = default;
    point(int __x, int __y) : x(__x), y(__y) {}
    point operator-(const point &p) const { return point(x - p.x, y - p.y); }
    bool operator<(const point &p) const { return (x == p.x) ? (y < p.y) : (x < p.x); }
    bool operator==(const point &p) const { return (x == p.x) && (y == p.y); }
    int dot(const point &p) const { return x * p.x + y * p.y; }
    int cross(const point &p) const { return x * p.y - y * p.x; }
    int cross(const point &a, const point &b) const { return (a - *this).cross(b - *this); }
};

std::vector<point> convexHull(std::vector<point> pts) {
    if (pts.size() <= 1) return pts;
    sort(pts.begin(), pts.end());
    std::vector<point> H(pts.size() + 1);
    int s = 0, t = 0;
    for (int i = 2; i--; s = --t, std::reverse(pts.begin(), pts.end())) {
        for (point &p : pts) {
            while (t > s + 1 && H[t - 2].cross(H[t - 1], p) <= 0) {
                t--;
            }
            H[t++] = p;
        }
    }
    return {H.begin(), H.begin() + t - (t == 2 && H[0] == H[1])};
};

int main() {

    return 0;
}