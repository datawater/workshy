#include "run.h"

#include <assert.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#include "console.h"
#include "register.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// #include "run_utils.h"

static pthread_mutex_t printf_mutex;

int sync_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    pthread_mutex_lock(&printf_mutex);
    int x = vprintf(format, args);
    pthread_mutex_unlock(&printf_mutex);

    va_end(args);

    return x;
}

bool __workshy_run_test(__workshy_test_function_ptr function, char* function_name, int i) {
    // TODO: figure out a way to block each threads stderr/stdout instead of just not blocking
    // __workshy_block_stdout();
    // __workshy_block_stderr();

    result_t result = function();

    // __workshy_unblock_stdout();
    // __workshy_unblock_stderr();

    if (result.result == fail) {
        sync_printf("Test N%d %s: "ANSI_COLOR_RED "failed\n" ANSI_COLOR_RESET, i + 1, function_name);

        if (result.error != NULL) sync_printf("Fail error string: %s\n\n", result.error);
    } else {
        sync_printf("Test N%d %s: "ANSI_COLOR_GREEN "passed\n" ANSI_COLOR_RESET, i + 1, function_name);
        return true;
    }

    return false;
}

static int passed = 0;

struct run_test_args {
    int start;
    int end;
};

void* run_test_pthread(void* args) {
    char** test_function_name_list = __workshy_get_test_function_names();
    __workshy_test_function_ptr* test_function_list = __workshy_get_test_functions();

    struct run_test_args fargs = *((struct run_test_args*) args);

    for (int i = fargs.start; i < fargs.end; i++) {
        passed += __workshy_run_test(test_function_list[i], test_function_name_list[i], i);
    }

    return NULL;
}

void __workshy_run_tests(unsigned short threads_count) {
    int tests_amount = *__workshy_get_test_amount();
    char** test_function_name_list = __workshy_get_test_function_names();
    __workshy_test_function_ptr* test_function_list = __workshy_get_test_functions();

    pthread_mutex_init(&printf_mutex, NULL);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * threads_count);
    struct run_test_args* args = (struct run_test_args*) malloc(sizeof(struct run_test_args*) * threads_count);

    memset(threads, 0, sizeof(pthread_t) * threads_count);
    memset(args, 0, sizeof(struct run_test_args) * threads_count);

    int each_test = (tests_amount - tests_amount % threads_count) / threads_count;
    for (int t = 0; t < threads_count; t++) {
        int start = t * each_test;
        int end = start + each_test;
        if (t == threads_count - 1)
            end = tests_amount;

        struct run_test_args arg = (struct run_test_args) {
            start, end
        };

        args[t] = arg;

        int result_code = pthread_create(&threads[t], NULL, run_test_pthread, &args[t]);
        if (result_code != 0) {
            errno = result_code;
            fprintf(stderr, "[workshy_fail] couldn't spawn testing thread. error: %s\n", strerror(errno));

            exit(1);
        }
    }

    for (int t = 0; t < threads_count; t++) {
        int result_code = pthread_join(threads[t], NULL);
        if (result_code != 0) {
            errno = result_code;

            fprintf(stderr, "[workshy_fail] couldn't join thread. error: %s\n", strerror(errno));
            exit(1);
        }
    }

    printf(ANSI_COLOR_CYAN "\n[INFO]" ANSI_COLOR_RESET " Passed: %d/%d\n", passed, tests_amount);

    if (passed == tests_amount)
        printf(ANSI_COLOR_GREEN "[SUCCESS]" ANSI_COLOR_RESET " All tests passed! :)\n");
    else
        printf(ANSI_COLOR_RED "[FAIL]" ANSI_COLOR_RESET " Failed " ANSI_COLOR_RED "%d" ANSI_COLOR_RESET
                              " test(s). :(\n",
               tests_amount - passed);

    pthread_mutex_destroy(&printf_mutex);
}
