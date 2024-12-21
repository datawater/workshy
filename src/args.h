#pragma once

#include <stddef.h>

typedef enum { __workshy_arg_test, __workshy_arg_bench, __workshy_arg_help } __workshy_args_command;

typedef struct {
    __workshy_args_command command;
    unsigned short threads;
    char* func_to_run;
    __workshy_args_command func_in_command;
    size_t func_index;
} __workshy_args;

__workshy_args __workshy_args_from_array(char** argv, int argc);
void __workshy_evaluate_args(__workshy_args* args);