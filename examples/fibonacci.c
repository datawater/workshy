#include "fibonacci.h"

typedef struct ull_pair_s {
    ull a;
    ull b;
} ull_pair;

#define MAKE_PAIR(a, b) \
    (ull_pair) { a, b }

ull nth_fibonacci_number_linear(int n) {
    ull a, b, c;
    a = 0;
    b = 1;
    c = 1;

    if (n == 0) return n;
    if (n == 1) return b;
    if (n == 2) return c;

    for (int i = 0; i < n - 1; i++) {
        c = a + b;
        a = b;
        b = c;
    }

    return c;
}

ull_pair __nth_fibonacci_number_fast_double(int n) {
    if (n == 0) {
        return MAKE_PAIR(0, 1);
    }

    ull_pair x = __nth_fibonacci_number_fast_double(n >> 1);
    ull c = x.a * ((x.b << 1) - x.a);
    ull d = x.a * x.a + x.b * x.b;

    return ((n & 1) == 0) ? MAKE_PAIR(c, d) : MAKE_PAIR(d, c + d);
}

ull nth_fibonacci_number_fast_double(int n) {
    if (n == 0) {
        return 0;
    }

    return __nth_fibonacci_number_fast_double(n).a;
}