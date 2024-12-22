#include "benchmarks.h"
#include "fibonacci.h"

void benchmark_fibonacci_linear(void) {
    WORKSHY_BLACK_BOX({ volatile unsigned long long F = nth_fibonacci_number_linear(2147483648 - 1); });
}

void benchmark_fibonacci_fast_double(void) {
    WORKSHY_BLACK_BOX({ volatile unsigned long long F = nth_fibonacci_number_fast_double(2147483648 - 1); });
}