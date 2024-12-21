#include "args.h"
#include "workshy.h"

int __workshy_main(int argc, char** argv) {
    __workshy_args args = __workshy_args_from_array(argv, argc);
    __workshy_evaluate_args(&args);

    return 0;
}