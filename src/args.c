#include "args.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "register.h"
#include "run.h"

void help() {
    printf("workshy <command> [specific benchmark/test to run] [options]\n\n");

    printf("commands:\n");
    printf("\ttest | t                 runs the tests\n");
    printf("\tbenchmark | bench | b    runs the benchmarks\n");
    printf("\thelp | h                 prints this help command\n\n");

    printf("options:\n");
    printf("\t--threads | -T {value}   set how many threads to use. uses all if {value} is blank. default is 1\n");
    printf("\t--help | -h              prints this help command\n");
}

__workshy_args __workshy_args_from_array(char** argv, int argc) {
    argv++;
    argc--;

    char** test_function_names = __workshy_get_test_function_names();
    char** bench_function_names = __workshy_get_benchmark_function_names();

    __workshy_args args = {0};
    args.threads = 1;

    if (argc == 0) {
        args.command = __workshy_arg_test;
        return args;
    }

    for (int i = 0; i < argc; i++) {
        bool is_help = strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "help") == 0 ||
                       strcmp(argv[i], "h") == 0;

        if (is_help) {
            args.command = __workshy_arg_help;
            return args;
        }

        bool is_test = strcmp(argv[i], "test") == 0 || strcmp(argv[i], "t") == 0;
        bool is_bench = strcmp(argv[i], "bench") == 0 || strcmp(argv[i], "b") == 0 || strcmp(argv[i], "benchmark") == 0;

        if (is_test) args.command = __workshy_arg_test;

        if (is_bench) args.command = __workshy_arg_bench;

        if (is_test || is_bench) continue;

        if (*argv[i] == '-') {
            bool is_thread = strcmp(argv[i], "--threads") == 0 || strcmp(argv[i], "-T") == 0;

            if (is_thread) {
                args.threads = (unsigned short)((i + 1 >= argc) ? sysconf(_SC_NPROCESSORS_ONLN) : atoi(argv[++i]));
                continue;
            } else {
                fprintf(stderr,
                        "[workshy_fail] unknown command line option: '%s'. Run "
                        "`workshy --help` for help.\n",
                        *argv);

                exit(EXIT_FAILURE);
            }
        }

        for (int j = 0; j < *__workshy_get_test_amount(); j++) {
            if (strcmp(test_function_names[j], argv[i]) != 0) continue;

            args.func_to_run = strdup(argv[i]);
            args.func_in_command = __workshy_arg_test;
            args.func_index = j;

            goto end;
        }

        for (int j = 0; j < *__workshy_get_test_amount(); j++) {
            if (strcmp(bench_function_names[j], argv[i]) != 0) continue;

            args.func_to_run = strdup(argv[i]);
            args.func_in_command = __workshy_arg_bench;
            args.func_index = j;

            goto end;
        }
    }

end:
    return args;
}

void __workshy_evaluate_args(__workshy_args* args) {
    __workshy_test_function_ptr* test_functions = __workshy_get_test_functions();
    __workshy_benchmark_function_ptr* bench_functions = __workshy_get_benchmark_functions();

    if (args->func_to_run != NULL) {
        if (args->func_in_command == __workshy_arg_test)
            __workshy_run_test(test_functions[args->func_index], args->func_to_run, args->func_index);
        else if (args->func_in_command == __workshy_arg_bench)
            __workshy_run_bench(bench_functions[args->func_index], args->func_to_run);
    }

    switch (args->command) {
        case __workshy_arg_test:
            __workshy_run_tests(args->threads);
            break;

        case __workshy_arg_bench:
            __workshy_run_benches();
            break;

        case __workshy_arg_help: {
            help();
            exit(0);
        } break;
    }
}