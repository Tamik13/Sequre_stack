#include "debugging.h"
#include <string.h>

typedef int stack_element; // Введите между typedef и stack_element тип данных стека
#define STK_MODIFIER "%d"  // Введите после stk модификатор вывода типа данных стека
#define POISON       1488  // Введите редко (желательно никогда не) встречающиеся значение в стеке

#define STACK_DEBUG        // Закомментируйте для отключения DEBUG режима

#ifdef STACK_DEBUG
    #define ON_DBG(...) __VA_ARGS__
#else
    #define ON_DBG(...)
#endif

#define LOWER_COEF  4
#define HIGHER_COEF 2

#define COUNT_CANARY      2
#define COUNT_LEFT_CANARY 1
#define CANARY_SIZE       1      // единица измерения - sizeof(stack_elemnet)
#define LEFT_CANARY       676767
#define RIGHT_CANARY      696969 // TODO: hex_speak

#define TO_STR(val) #val

struct stack_s {
    ON_DBG(stack_element _left_canary = LEFT_CANARY;)

    ON_DBG(
        const char* name     = NULL;
        const char* file     = NULL;
        const char* function = NULL;
        size_t line          = 0;
        unsigned long hash = 0;
    )

    stack_element* _real_data = NULL;
    stack_element* data       = NULL;
    size_t size               = 0;
    size_t capacity           = 0;

    ON_DBG(stack_element _right_canary = RIGHT_CANARY;)
};

error_code_e stack_init    (stack_s* const stack, const size_t capacity ON_DBG(, const char* const name, const char* const file, const char* const function, const size_t line));
error_code_e stack_push    (stack_s* const stack, const stack_element  value);
error_code_e stack_pop     (stack_s* const stack, stack_element* const value);
error_code_e stack_destroy (stack_s* const stack);
error_code_e stack_reсalloc(stack_s* const stack, const size_t new_capacity);

error_code_e stack_verify(stack_s* const stack);
void print_stack         (const stack_s* const stack);
bool is_stack_init       (const stack_s* const stack);

void log_dump_stack(const stack_s* const stack, const char* const reason);
