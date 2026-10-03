#include <stdio.h>
#include <math.h>
#include "stack.h"

void very_smart_function(stack_s* const    stack);
int  char_cmp           (const void* const first_elem, const void* const second_elem);

int main() {
    start_logs();
    error_code_e error_code = INIT_VALUE;
    stack_s      stack      = {};
    size_t       size       = 1;

    error_code = stack_init(&stack, size ON_DBG(, "stack", __FILE__, __FUNCTION__, __LINE__));
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }

    error_code = stack_push(&stack, 10);
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }

    error_code = stack_push(&stack, 20);
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }

    error_code = stack_push(&stack, 30);
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }



//     stack_element pop_element = 0;
//
//     error_code = stack_pop(&stack, &pop_element);
//     if (error_code != SUCCESS) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }
//
//     error_code = stack_pop(&stack, &pop_element);
//     if (error_code != SUCCESS) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }
//
//     error_code = stack_pop(&stack, &pop_element);
//     if (error_code != SUCCESS) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }
//
//     error_code = stack_pop(&stack, &pop_element);
//     if (error_code != SUCCESS) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }

//     very_smart_function(&stack);
//
//     error_code = stack_push(&stack, 2);
//     if (error_code) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }


//     stack.name = "";
//
//     error_code = stack_push(&stack, 2);
//     if (error_code) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }

//     stack.data = NULL;
//
//     error_code = stack_push(&stack, 2);
//     if (error_code) {
//         PRINT_ERROR(error_code)
//         return error_code;
//     }

    stack.data[1] = 148;

    error_code = stack_push(&stack, 2);
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }

    stack.data[4] = 148;

    error_code = stack_push(&stack, 2);
    if (error_code) {
        PRINT_ERROR(error_code)
        return error_code;
    }

    printf(COLOR_TEXT("SUCCESS\n", GREEN));

    return 0;
}


void very_smart_function(stack_s* const stack) {
    assert(stack != NULL);

    qsort(stack, sizeof(*stack), sizeof(char), char_cmp);
}

int char_cmp(const void* const first_elem, const void* const second_elem) {
    assert(first_elem  != NULL);
    assert(second_elem != NULL);

    return *(const char* const)first_elem - *(const char* const)second_elem;
}


