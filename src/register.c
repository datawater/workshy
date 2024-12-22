#include "register.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "workshy.h"

__workshy_test_function_ptr* workshy_tests_list = NULL;
char** workshy_test_function_name_list = NULL;
static int workshy_tests_amount;
static int __workshy_tests_array_capacity;

__workshy_benchmark_function_ptr* workshy_benchmark_list;
char** workshy_benchmark_function_name_list;
static int workshy_benchmark_amount;
static int __workshy_benchmark_array_capacity;

#define SAFE_MALLOC(ptr, size, type)                           \
    do {                                                       \
        ptr = (type*)malloc(sizeof(type) * (size));            \
        memset((ptr), 0, sizeof(type) * (size));               \
        assert((ptr) != NULL && "couldn't allocate memory\n"); \
    } while (0);

#define SAFE_REALLOC(ptr, capacity, type, delta)               \
    do {                                                       \
        capacity = (int)(((float)(capacity)) * (delta));       \
        (ptr) = (type*)realloc((ptr), (capacity));             \
        assert((ptr) != NULL && "couldn't allocate memory\n"); \
    } while (0);

#define PHI 1.618033988
#define NO_INCREASE 1.0

void safe_check_array_pointers() {
    if (workshy_tests_list == NULL) {
        SAFE_MALLOC(workshy_tests_list, WORKSHY_DEFAULT_AMOUNT_OF_TESTS, __workshy_test_function_ptr)
    }

    if (workshy_test_function_name_list == NULL) {
        SAFE_MALLOC(workshy_test_function_name_list, WORKSHY_DEFAULT_AMOUNT_OF_TESTS, char*);
    }

    if (workshy_tests_amount + 1 == __workshy_tests_array_capacity) {
        SAFE_REALLOC(workshy_tests_list, __workshy_tests_array_capacity, __workshy_test_function_ptr, PHI);
        SAFE_REALLOC(workshy_test_function_name_list, __workshy_tests_array_capacity, char*, NO_INCREASE);
    }

    if (workshy_benchmark_list == NULL) {
        SAFE_MALLOC(workshy_benchmark_list, WORKSHY_DEFAULT_AMOUNT_OF_BENCHMARKS, __workshy_benchmark_function_ptr)
    }

    if (workshy_benchmark_function_name_list == NULL) {
        SAFE_MALLOC(workshy_benchmark_function_name_list, WORKSHY_DEFAULT_AMOUNT_OF_BENCHMARKS, char*);
    }

    if (workshy_benchmark_amount + 1 == __workshy_benchmark_array_capacity) {
        SAFE_REALLOC(workshy_benchmark_list, __workshy_benchmark_array_capacity, __workshy_benchmark_function_ptr, PHI);
        SAFE_REALLOC(workshy_benchmark_function_name_list, __workshy_benchmark_array_capacity, char*, NO_INCREASE);
    }
}

void __workshy__register_test(__workshy_test_function_ptr test_function, char* function_name) {
    safe_check_array_pointers();

    // O(n) insertion instead of O(1) because, ofc.
    for (int i = 0; i < workshy_tests_amount; i++) {
        if (workshy_tests_list[i] == test_function) return;
    }

    if (function_name == NULL) {
        assert(0 &&
               "function_name is __workshy__register_test is null. something "
               "went horribly wrong");
    }

    workshy_tests_list[workshy_tests_amount] = test_function;
    workshy_test_function_name_list[workshy_tests_amount++] = function_name;
}

void __workshy__register_benchmark(__workshy_benchmark_function_ptr test_function, char* function_name) {
    safe_check_array_pointers();

    // O(n) insertion instead of O(1) because, ofc.
    for (int i = 0; i < workshy_benchmark_amount; i++) {
        if (workshy_benchmark_list[i] == test_function) return;
    }

    if (function_name == NULL) {
        assert(0 &&
               "function_name is __workshy__register_benchmark is null. "
               "something went horribly wrong");
    }

    workshy_benchmark_list[workshy_benchmark_amount] = test_function;
    workshy_benchmark_function_name_list[workshy_benchmark_amount++] = function_name;
}

__workshy_test_function_ptr* __workshy_get_test_functions() { return workshy_tests_list; }

char** __workshy_get_test_function_names() { return workshy_test_function_name_list; }

int* __workshy_get_test_amount() { return (int*)&workshy_tests_amount; }

__workshy_benchmark_function_ptr* __workshy_get_benchmark_functions() { return workshy_benchmark_list; }

char** __workshy_get_benchmark_function_names() { return workshy_benchmark_function_name_list; }

int* __workshy_get_benchmark_amount() { return (int*)&workshy_benchmark_amount; }
