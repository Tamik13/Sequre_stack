#include <stdio.h>
#include <math.h>
#include "stack.h"

int main() {
    error_code_e error_code = INIT_VALUE;
    stack_s stack = {};
    size_t size = 1;

    start_logs();

    error_code = stack_init(&stack, size ON_DBG(, TO_STR(val), __FILE__, __FUNCTION__, __LINE__));
    if (error_code) {
        return error_code;
    }

    error_code = stack_push(&stack, 1);
    if (error_code) {
        return error_code;
    }

    error_code = stack_push(&stack, 2);
    if (error_code) {
        return error_code;
    }

    error_code = stack_push(&stack, 3);
    if (error_code) {
        return error_code;
    }


    stack_element pop_element = 0;

    error_code = stack_pop(&stack, &pop_element);
    if (error_code != SUCCESS) {
        return error_code;
    }

    error_code = stack_pop(&stack, &pop_element);
    if (error_code != SUCCESS) {
        return error_code;
    }

    error_code = stack_pop(&stack, &pop_element);
    if (error_code != SUCCESS) {
        return error_code;
    }

    error_code = stack_pop(&stack, &pop_element);
    if (error_code != SUCCESS) {
        return error_code;
    }



    return 0;
}
