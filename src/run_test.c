#include "run.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "console.h"
#include "register.h"
#include "run_utils.h"

#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

bool __workshy_run_test(__workshy_test_function_ptr function, char* function_name, int i) {
    printf("Test N%d %s: ", i, function_name);

    __workshy_block_stdout();
    __workshy_block_stderr();

    // TODO: workshy: refactor test running to fork the main process and call
    // the test function from there, instead of raw-calling
    result_t result = function();

    __workshy_unblock_stdout();
    __workshy_unblock_stderr();

    if (result.result == fail) {
        printf(ANSI_COLOR_RED "failed\n" ANSI_COLOR_RESET);

        if (result.error != NULL) printf("Fail error string: %s\n\n", result.error);
    } else {
        printf(ANSI_COLOR_GREEN "passed\n" ANSI_COLOR_RESET);
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

    printf(ANSI_COLOR_CYAN "[INFO]" ANSI_COLOR_RESET " Amount of tests: %d\n\n", tests_amount);

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * threads_count);
    memset(threads, 0, sizeof(pthread_t) * threads_count);

    int test_amount_floored = tests_amount % threads_count;
    int each_test = (tests_amount - test_amount_floored) / threads_count;
    for (int t = 0; t < threads_count; t++) {
        int start = t * threads_count;
        int end = start + threads_count + (!(t == threads_count - 1)) * (tests_amount - test_amount_floored);

        struct run_test_args args = (struct run_test_args) {
            start, end
        };

        int result_code = pthread_create(&threads[t], NULL, run_test_pthread, &args);
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
}
