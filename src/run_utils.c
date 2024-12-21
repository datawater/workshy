#include "run_utils.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int __workshy_stdout;
static int __workshy_stderr;

#if defined(_WIN32)
#define __WORKSHY_DEV_NULL "nul"
#elif defined(__unix__)
#define __WORKSHY_DEV_NULL "/dev/null"
#endif

void swap_with_dev_null(int x, int* x_backup) {
    (void)x_backup;

    int dev_null = open(__WORKSHY_DEV_NULL, O_WRONLY);

    if (dev_null == -1) {
        fprintf(stderr,
                "[workshy_err] Couldn't open '"__WORKSHY_DEV_NULL
                "'. error: %s\n",
                strerror(errno));
        abort();
    }

    *x_backup = dup(x);
    if (*x_backup == -1) {
        fprintf(stderr, "[workshy_err] Couldn't dup. error: %s\n", strerror(errno));
        abort();
    }

    if (dup2(dev_null, x) == -1) {
        fprintf(stderr, "[workshy_err] Couldn't dup2 block. error: %s\n", strerror(errno));
        abort();
    }
}

// TODO: workshy: Add flag to disallow stdout blocking
void __workshy_block_stdout() { swap_with_dev_null(STDOUT_FILENO, &__workshy_stdout); }

void __workshy_block_stderr() { swap_with_dev_null(STDERR_FILENO, &__workshy_stderr); }

void __workshy_unblock_stdout() {
    if (!__workshy_stdout) return;

    if (dup2(__workshy_stdout, STDOUT_FILENO) == -1) {
        fprintf(stderr, "[workshy_err] Couldn't dup2 restore. error: %s\n", strerror(errno));
        abort();
    }
}

void __workshy_unblock_stderr() {
    if (!__workshy_stderr) return;

    if (dup2(__workshy_stderr, STDERR_FILENO) == -1) {
        fprintf(stderr, "[workshy_err] Couldn't dup2 restore. error: %s\n", strerror(errno));
        abort();
    }
}