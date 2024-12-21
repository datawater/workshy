#pragma once

#include <stdbool.h>

#include "register.h"

void __workshy_run_tests(unsigned short threads);
void __workshy_run_benches();
bool __workshy_run_test(__workshy_test_function_ptr function, char* function_name, int i);
void __workshy_run_bench(__workshy_benchmark_function_ptr function, char* function_name);