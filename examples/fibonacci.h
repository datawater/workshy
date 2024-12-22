#pragma once

typedef unsigned long long ull;

ull nth_fibonacci_number_linear(int n);
ull nth_fibonacci_number_fast_double(int n);

// #define SLOW

static ull nth_fibonacci_number(int n) {
#ifdef SLOW
    return nth_fibonacci_number_linear(n);
#else
    return nth_fibonacci_number_fast_double(n);
#endif  // SLOW
}