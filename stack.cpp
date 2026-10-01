#include "stack.h"

error_code_e stack_verify(stack_s* const stack) {
    if (stack == NULL) {
        log_print_error(NULL_STACK, "stack_verify: ERROR null stack\n");
        return NULL_STACK;
    }

    ON_DBG(
    if (stack->_left_canary != LEFT_CANARY) {
        log_print_error(NULL_STACK, "stack_verify: ERROR first canary in struct IS DEAD(((\n");
        return CANARY_IS_DEAD;
    }

    if (stack->_right_canary != RIGHT_CANARY) {
        log_print_error(NULL_STACK, "stack_verify: ERROR second canary in struct IS DEAD(((\n");
        return CANARY_IS_DEAD;
    }

    unsigned long old_hash = stack->hash;
    stack->hash = 0;
    unsigned long new_hash = djb2_hash((const char*)stack, sizeof(*stack));

    if (old_hash != new_hash) {
        log_print_error(HASH_CHANGED, "stack_verify: ERROR new_hash != old_hash\n");
        return HASH_CHANGED;
    }

    stack->hash = new_hash;

    if (stack->_real_data[0] != LEFT_CANARY) {
        log_print_error(NULL_STACK, "stack_verify: ERROR first canary in data IS DEAD(((\n");
        return CANARY_IS_DEAD;
    }

    if (stack->data[stack->capacity] != RIGHT_CANARY) {
        log_print_error(NULL_STACK, "stack_verify: ERROR second canary in data IS DEAD(((\n");
        return CANARY_IS_DEAD;
    }
    )

    if (stack->capacity == 0) {
        log_print_error(ZERO_CAPACITY, "stack_verify: ERROR zero capacity\n");
        return ZERO_CAPACITY;
    }

    if (stack->capacity < stack->size) {
        log_print_error(SIZE_HIGHER_CAPACITY, "stack_verify: ERROR size higher capacity\n");
        log_dump_stack(stack,                 "stack_verify: ERROR size higher capacity\n");
        return SIZE_HIGHER_CAPACITY;
    }

    return SUCCESS;
}


error_code_e stack_init(stack_s* const stack, const size_t capacity ON_DBG(, const char* const name, const char* const file, const char* const function, const size_t line)) {
    assert(capacity != 0);

    if (is_stack_init(stack)) {
        log_print_error(REINITIALIZATION, "stack_init: ERROR try to init already initialized stack\n");
        return REINITIALIZATION;
    }

    stack->_real_data = (stack_element*)calloc(capacity + COUNT_CANARY * CANARY_SIZE, sizeof(double));

    if (stack->_real_data == NULL) {
        log_print_error(ALLOCATION_ERROR, "stack_init: ERROR during allocation\n");
        return ALLOCATION_ERROR;
    }


    stack->data     = stack->_real_data + COUNT_LEFT_CANARY * CANARY_SIZE;
    stack->capacity = capacity;
    stack->size     = 0;

    ON_DBG(
        stack->name     = name;
        stack->file     = file;
        stack->function = function;
        stack->line     = line;
    )

    for (size_t ind = 0; ind < stack->capacity; ind++) {
        ASSERT_FOR_ARR(ind, stack->capacity); // TODO malloc_size
        stack->data[ind] = POISON;
    }

    stack->_real_data[0]         = LEFT_CANARY;
    stack->data[stack->capacity] = RIGHT_CANARY;

    ON_DBG(
    stack->_left_canary = LEFT_CANARY;

    stack->hash = 0;
    stack->hash = djb2_hash((const char*)stack, sizeof(*stack));
    )

    return SUCCESS;
}


error_code_e stack_push(stack_s* const stack, const stack_element value) {
    error_code_e error_code = INIT_VALUE;

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "stack_push: ERROR before push during stack_verify\n");

        if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED && error_code != HASH_CHANGED) {
            log_dump_stack (stack,  "stack_push: ERROR before push during stack_verify\n");
        }
        return error_code;
    }

    log_dump_stack(stack, "stack before push\n");

    if (stack->size == stack->capacity) {
        stack_reсalloc(stack, stack->size * HIGHER_COEF);

        error_code = stack_verify(stack);
        if (error_code) {
            log_print_error(error_code, "stack_push: ERROR after recalloc during stack_verify\n");

            if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED) {
                log_dump_stack (stack,  "stack_push: ERROR after recalloc during stack_verify\n");
            }
            return error_code;
        }
    }

    stack->data[stack->size++] = value;

    ON_DBG(
    stack->hash = 0;
    stack->hash = djb2_hash((const char*)stack, sizeof(*stack));
    )

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "stack_push: ERROR after push during stack_verify\n");

        if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED) {
            log_dump_stack (stack,  "stack_push: ERROR after push during stack_verify\n");
        }

        return error_code;
    }

    log_dump_stack(stack, "stack after push\n");

    return SUCCESS;
}


error_code_e stack_pop(stack_s* const stack, stack_element* const value) {
    error_code_e error_code = INIT_VALUE;

    if (stack->size == 0) {
        log_print_error(POP_VOID_STACK, "stack_push: ERROR before pop try pop void stack\n");
        log_dump_stack (stack,          "stack_push: ERROR before pop try pop void stack\n");
        return POP_VOID_STACK;
    }

    log_dump_stack(stack, "stack before pop\n");

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "stack_push: ERROR before pop during stack_verify\n");

        if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED) {
            log_dump_stack (stack,  "stack_push: ERROR before pop during stack_verify\n");
        }
        return error_code;
    }

    if (stack->size <= stack->capacity / LOWER_COEF && stack->capacity / LOWER_COEF ) {
        stack_reсalloc(stack, stack->capacity / LOWER_COEF);

        error_code = stack_verify(stack);
        if (error_code) {
            log_print_error(error_code, "stack_push: ERROR after recalloc during stack_verify\n");

            if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED) {
                log_dump_stack (stack,  "stack_push: ERROR after recalloc during stack_verify\n");
            }
            return error_code;
        }
    }

    *value = stack->data[stack->size];
    stack->size--;
    stack->data[stack->size] = POISON;

    ON_DBG(
    stack->hash = 0;
    stack->hash = djb2_hash((const char*)stack, sizeof(*stack));
    )

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "ERROR after pop during stack_verify\n");

        if (error_code != CANARY_IS_DEAD && error_code != HASH_CHANGED) {
            log_dump_stack (stack,  "ERROR after pop during stack_verify\n");
        }
        return error_code;
    }

    log_dump_stack(stack, "stack after pop\n");

    return SUCCESS;
}


error_code_e stack_destroy(stack_s* const stack) {
    error_code_e error_code = stack_verify(stack);

    if (error_code == CANARY_IS_DEAD) {
        log_print_error(error_code, "stack_destroy: ERROR possible damage to the stack, including data. Stack cant be destroy\n");
        return error_code;
    }

    free(stack->_real_data);
    stack->_real_data = NULL;
    stack->data       = NULL;
    stack->capacity   = 0;
    stack->size       = 0;

    ON_DBG(
        stack->hash       = 0;
        stack->name     = NULL;
        stack->file     = NULL;
        stack->function = NULL;
        stack->line     = 0;
    )

    return error_code;
}


error_code_e stack_reсalloc(stack_s* const stack, const size_t new_capacity) {
    assert(stack != NULL);
    assert(new_capacity != 0);

    error_code_e error_code = INIT_VALUE;

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "stack_recalloc: ERROR before recalloc during stack_verify\n");
        return error_code;
    }

    stack->_real_data = (stack_element*)realloc((void*)stack->_real_data, new_capacity);

    if (stack->data == NULL) {
        log_print_error(error_code, "stack_recalloc: ERROR during recalloc\n");
        return ALLOCATION_ERROR;
    }

    stack->_real_data[0]      = LEFT_CANARY;
    stack->data[new_capacity] = RIGHT_CANARY;

    for (size_t ind = stack->size; ind < new_capacity; ind++) {
        ASSERT_FOR_ARR(ind, new_capacity);
        stack->data[ind] = POISON;
    }

    stack->capacity = new_capacity;

    ON_DBG(
    stack->hash = 0;
    stack->hash = djb2_hash((const char*)stack, sizeof(*stack));
    )

    error_code = stack_verify(stack);
    if (error_code) {
        log_print_error(error_code, "stack_recalloc: ERROR after recalloc during stack_verify\n");
        return error_code;
    }

    return SUCCESS;
}



void print_stack(const stack_s* const stack) {
    assert(stack != NULL);

    ON_DBG(fprintf(stderr, "stack_t \"%s\"[%p]] created by %s() at %s:%zu)", stack->name, stack, stack->function, stack->file, stack->line));

    fprintf(stderr, "{\n");
    fprintf(stderr, "\tsize = %zu \n capacity = %zu\n", stack->size, stack->capacity);
    fprintf(stderr,"\tdata[%p]\n", stack->data);
    fprintf(stderr, "\t{\n");

    for (size_t ind = 0; ind < stack->size; ind++) {
        ASSERT_FOR_ARR(ind, stack->size);
        fprintf(stderr ,"\t\t*[%zu] = " STK_MODIFIER "\n", ind, stack->data[ind]);
    }

    for (size_t ind = stack->size; ind < stack->capacity; ind++) {
        ASSERT_FOR_ARR(ind, stack->capacity);
        fprintf(stderr ,"\t\t [%zu] = " STK_MODIFIER " (POISON) \n", ind, stack->data[ind]);
    }

    fprintf(stderr ,"\t}\n");
    fprintf(stderr, "}\n");
}


bool is_stack_init(const stack_s* const stack) {
    assert(stack != NULL);

    ON_DBG(
    if (stack->file != NULL) {
        return true;
    }

    if (stack->function != NULL) {
        return true;
    }

    if (stack->line != 0) {
        return true;
    }

    if (stack->name != NULL) {
        return true;
    }
    )

    if (stack->_real_data != NULL) {
        return true;
    }

    if (stack->data != NULL) {
        return true;
    }

    if (stack->capacity != 0) {
        return true;
    }

    if (stack->size != 0) {
        return true;
    }

    return false;
}


void start_logs(void) {
    FILE* log_file = fopen(LOG_FILE_NAME, "w");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}


void log_dump_stack(const stack_s* const stack, const char* const reason) {
    assert(stack != NULL);

    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    fprintf(log_file, "\nFUNCTION: log_dump_stack DATE:%s REASON: %s", __DATE__, reason);

    ON_DBG(fprintf(log_file, "stack_t \"%s\"[%p] created by %s() at %s:%zu\n", stack->name, stack, stack->function, stack->file, stack->line));

    fprintf(log_file, "{\n");
    fprintf(log_file, "\tsize     = %zu\n", stack->size);
    fprintf(log_file, "\tcapacity = %zu\n", stack->capacity);

    ON_DBG(
    fprintf(log_file, "\thash     = %lu\n", stack->hash);
    )

    fprintf(log_file,"\t_real_data[%p]\n", stack->_real_data);
    fprintf(log_file, "\t{\n");

    fprintf(log_file, "\t\t [%zu] = " STK_MODIFIER " (CANARY!!!)\n", (size_t)0, stack->_real_data[0]); // ???(size_t)0 почему компилятор думает что 0 это int ???

    for (size_t ind = 0; ind < stack->size; ind++) {
        ASSERT_FOR_ARR(ind, stack->size);
        fprintf(log_file ,"\t\t*[%zu] = " STK_MODIFIER "\n", ind + 1, stack->data[ind]);
    }

    for (size_t ind = stack->size; ind < stack->capacity; ind++) {
        ASSERT_FOR_ARR(ind, stack->capacity);
        fprintf(log_file ,"\t\t [%zu] = " STK_MODIFIER " (POISON!!!) \n", ind + 1, stack->data[ind]);
    }

    fprintf(log_file, "\t\t [%zu] = " STK_MODIFIER " (CANARY!!!)\n", stack->capacity + 1, stack->data[stack->capacity]);

    fprintf(log_file ,"\t}\n");

    fprintf(log_file,"\tdata[%p]\n", stack->data);
    fprintf(log_file, "\t{\n");

    for (size_t ind = 0; ind < stack->size; ind++) {
        ASSERT_FOR_ARR(ind, stack->size);
        fprintf(log_file ,"\t\t*[%zu] = " STK_MODIFIER "\n", ind, stack->data[ind]);
    }

    for (size_t ind = stack->size; ind < stack->capacity; ind++) {
        ASSERT_FOR_ARR(ind, stack->capacity);
        fprintf(log_file ,"\t\t [%zu] = " STK_MODIFIER " (POISON!!!) \n", ind, stack->data[ind]);
    }

    fprintf(log_file ,"\t}\n");
    fprintf(log_file, "}\n");

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}





