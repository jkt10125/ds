// returns: gcd(a, b), x, y such that ax + by = gcd(a, b)
int egcd(int a, int b, int *x, int *y) {
    if (!a && !b) { return (*x = *y = 0); }
    int q0, r0, r1, r2, x0, x1, x2;
    r2 = a, r1 = b; x2 = 1, x1 = 0;
    while (r1) {
        q0 = r2 / r1;
        r0 = r2 - q0 * r1; r2 = r1, r1 = r0;
        x0 = x2 - q0 * x1; x2 = x1, x1 = x0;
    }
    *y = (b) ? (r2 - a * x2) / b : 0, *x = x2;
    return r2;
}