#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fibonacci.h"
#include "tests.h"

#define FIBONACCI_ASSERT(expression, n, expected, got)                       \
    do {                                                                     \
        if (!(expression)) {                                                 \
            char* error_string = malloc(sizeof(char) * 128);                 \
            assert(error_string != NULL);                                    \
            memset(error_string, 0, sizeof(char) * 128);                     \
            sprintf(error_string,                                            \
                    "Fibonacci number `%d` doesn't match. Expected result: " \
                    "%llu, got: %llu",                                       \
                    (n), (expected), (got));                                 \
            return new_result_error(error_string);                           \
        }                                                                    \
    } while (0)

result_t test_fibonacci_linear(void) {
    unsigned long long first_10[10] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

    for (int i = 0; i < 10; i++) {
        unsigned long long f_n = nth_fibonacci_number_linear(i);
        FIBONACCI_ASSERT(f_n == first_10[i], i, first_10[i], f_n);
    }

    return new_result_ok();
}

result_t test_fibonacci_fast_double(void) {
    unsigned long long first_10[10] = {0, 1, 1, 2, 3, 5, 8, 13, 21, 34};

    for (int i = 0; i < 10; i++) {
        unsigned long long f_n = nth_fibonacci_number_fast_double(i);
        FIBONACCI_ASSERT(f_n == first_10[i], i, first_10[i], f_n);
    }
    
    return new_result_ok();
}