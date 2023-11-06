#include <bits/stdc++.h>

using T = int32_t;

// f is an idempotent function. Example: min, max, gcd, lcm, etc.
T f(T a, T b);

struct Stack : std::vector<std::array<T, 2>> {
    void push(T v) { push_back({v, ::f(v, empty() ? v : back()[1])}); }
    void pop() { pop_back(); }
    bool empty() { return this->std::vector<std::array<T, 2>>::empty(); }
    int size() { return this->std::vector<std::array<T, 2>>::size(); }
    T top() { return back()[0]; }
    T f() { return back()[1]; }
};

// [0][] : push stack, [1][] : pop stack
struct Queue : std::array<Stack, 2> {
    void ts() {
        if (!(*this)[1].empty()) { return; }
        while (!(*this)[0].empty()) { (*this)[1].push((*this)[0].top()); (*this)[0].pop(); }
    }
    void push(T v) { (*this)[0].push(v); }
    void pop() { ts(); (*this)[1].pop(); }
    bool empty() { return (*this)[0].empty() && (*this)[1].empty(); }
    int size() { return (*this)[0].size() + (*this)[1].size(); }
    T front() { ts(); return (*this)[1].top(); }
    T f() { ts(); return (*this)[0].empty() ? (*this)[1].f() : ::f((*this)[0].f(), (*this)[1].f()); }
};