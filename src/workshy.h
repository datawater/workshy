#pragma once

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#define WORKSHY_VERSION_MAJOR 0
#define WORKSHY_VERSION_MINOR 2
#define WORKSHY_VERSION_PATCH 0

#include <stdbool.h>
#include <stddef.h>

#include "register.h"
#include "workshy_macros.h"

result_t new_result(void);
result_t new_result_error(char* error);
result_t new_result_ok(void);

int __workshy_main(int argc, char** argv);

void __workshy__register_test(result_t (*test_function)(void), char* function_name);
void __workshy__register_benchmark(void (*test_function)(void), char* function_name);

#ifdef __cplusplus
}
#endif  // __cplusplus