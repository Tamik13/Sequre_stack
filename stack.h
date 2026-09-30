#include "debuging.h"
#include <string.h>

typedef int stack_element; // Введите между typedef и stack_element тип данных стека
#define STK_MODIFIER "%d"  // Введите после stk модификатор вывода типа данных стека
#define POISON 1488        // Введите редко (желательно никогда не) встречающиеся значение в стеке

// #define STACK_DEBUG // Закомментируйте для отключения DEBUG режима

#ifdef STACK_DEBUG
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif

#define LOWER_COEF 4
#define HIGHER_COEF 2

#define TO_STR(val) #val

struct stack_s {

    ON_DBG(
        const char* name     = NULL;
        const char* file     = NULL;
        const char* function = NULL;
        size_t line          = 0;
    )

    stack_element* data = NULL;
    size_t size = 0;
    size_t capacity = 0;
};

error_code_e stack_init   (stack_s* stack, size_t capacity ON_DBG(, const char* const name, const char* const file, const char* const function, const size_t line));
error_code_e stack_push   (stack_s* stack, stack_element  value);
error_code_e stack_pop    (stack_s* stack, stack_element* value);
error_code_e stack_destroy(stack_s* stack);

error_code_e stack_verify  (stack_s* stack);
void print_stack_error     (stack_s* stack, error_code_e error);
void print_stack           (stack_s* stack);
error_code_e stack_reсalloc(stack_s* stack, size_t new_capacity);

void log_dump_stack(stack_s* stack, const char* const reason);
