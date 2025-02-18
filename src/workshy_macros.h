#pragma once

#ifndef WORKSHY_DEFAULT_AMOUNT_OF_TESTS
#define WORKSHY_DEFAULT_AMOUNT_OF_TESTS 128
#endif  // WORKSHY_DEFAULT_AMOUNT_OF_TESTS

#ifndef WORKSHY_DEFAULT_AMOUNT_OF_BENCHMARKS
#define WORKSHY_DEFAULT_AMOUNT_OF_BENCHMARKS 128
#endif  // WORKSHY_DEFAULT_AMOUNT_OF_TESTS

#define __workshy_benchmark_samples_amount 20

#define CONCAT_1(x, y) x##y
#define CONCAT_2(x, y) CONCAT_1(x, y)
#define CONCAT_3(x) CONCAT_2(x, __COUNTER__)

#define WORKSHY_TEST(function_name)                                                                                \
    result_t function_name(void);                                                                                  \
    static void function_name##__runtime_marker(void) __attribute__((constructor));                                \
    static void function_name##__runtime_marker(void) { __workshy__register_test(function_name, #function_name); } \
    result_t function_name(void)

#define WORKSHY_BENCHMARK(function_name)                                            \
    void function_name(void);                                                       \
    static void function_name##__runtime_marker(void) __attribute__((constructor)); \
    static void function_name##__runtime_marker(void) {                             \
        __workshy__register_benchmark(function_name, #function_name);               \
    }                                                                               \
    void function_name(void)

#define WORKSHY_RUN() \
    int main(int argc, char** argv) { return __workshy_main(argc, argv); }

#define WORKSHY_BLACK_BOX(code_blox)                  \
    do {                                              \
        __asm__ volatile("" ::: "memory");            \
        code_blox __asm__ volatile("" :: : "memory"); \
    } while (0)
