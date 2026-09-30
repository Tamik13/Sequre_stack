#include "stack.h"

const char* const LOG_FILE_NAME = "log.txt";

error_code_e stack_verify(stack_s* stack) {
    if (stack == NULL) {
        log_print("stack_verify\n: NULL STACK");
        return NULL_STACK;
    }

    if (stack->capacity == 0) {
        log_print("stack_verify: ZERO CAPACITY\n");
        return ZERO_CAPACITY;
    }

    if (stack->capacity < stack->size) {
        log_dump_stack(stack, "stack_verify: size higher then capacity\n");
        return SIZE_HIGHER_CAPACITY;
    }

    return SUCCESS;
}


error_code_e stack_init(stack_s* stack, size_t capacity ON_DBG(, const char* const name, const char* const file, const char* const function, const size_t line)) {
    assert(capacity != 0);

    stack->data     = (stack_element*)calloc(capacity, sizeof(double));
    stack->capacity = capacity;
    stack->size     = 0;

    ON_DBG(
        stack->name = name;
        stack->file = file;
        stack->function = function;
        stack->line = line;
    )

    if (stack->data == NULL) {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}


error_code_e stack_push(stack_s* stack, stack_element value) {
    error_code_e error_code = INIT_VALUE;

    error_code = stack_verify(stack);
    if (error_code) {
        log_dump_stack(stack, "error before stack_push during stack_verify\n");
        return error_code;
    }

    log_dump_stack(stack, "stack before push\n");

    if (stack->size == stack->capacity) {
        stack_realloc(stack, stack->size * HIGHER_COEF);

        error_code = stack_verify(stack);
        if (error_code) {
            log_dump_stack(stack, "error in stack_push after stack_realloc during stack_verify\n");
            return error_code;
        }
    }

    stack->data[stack->size++] = value;

    error_code = stack_verify(stack);
    if (error_code) {
        log_dump_stack(stack, "error after stack_push during stack_verify\n");
        return error_code;
    }

    log_dump_stack(stack, "stack after push\n");

    return SUCCESS;
}


error_code_e stack_pop(stack_s* stack, stack_element* value) {
    error_code_e error_code = INIT_VALUE;

    if (stack->size == 0) {
        log_print("");
        return POP_VOID_STACK;
    }

    log_dump_stack(stack, "stack before pop\n");

    error_code = stack_verify(stack);
    if (error_code) {
        log_dump_stack(stack, "error before stack_pop during stack_verify\n");
        return error_code;
    }

    if (stack->size <= stack->capacity / LOWER_COEF && stack->capacity / LOWER_COEF ) {
        stack_realloc(stack, stack->capacity / LOWER_COEF);

        error_code = stack_verify(stack);
        if (error_code) {
            log_dump_stack(stack, "error in stack_pop after stack_realloc during stack_verify\n");
            return error_code;
        }
    }

    *value = stack->data[--stack->size];

    error_code = stack_verify(stack);
    if (error_code) {
        log_dump_stack(stack, "error after stack_pop during stack_verify\n");
        return error_code;
    }

    log_dump_stack(stack, "stack before pop\n");

    return SUCCESS;
}


error_code_e stack_realloc(stack_s* stack, size_t new_capacity) {
    assert(stack != NULL);
    assert(new_capacity != 0);

    error_code_e error_code = INIT_VALUE;

    error_code = stack_verify(stack);
    if (error_code) return error_code;

    stack->data = (stack_element*)realloc((void*)stack->data, new_capacity);

    if (stack->data == NULL) {
        return ALLOCATION_ERROR;
    }

    stack->capacity *= 2;

    error_code = stack_verify(stack);
    if (error_code) return error_code;

    return SUCCESS;
}



void print_stack(stack_s* stack) {
    assert(stack != NULL);

    ON_DBG(fprintf(stderr, "stack_t \"%s\"[%p]] created by %s() at %s:%zu)", stack->name, stack, stack->function, stack->file, stack->line));

    fprintf(stderr, "{\n");
    fprintf(stderr, "\tsize = %zu \n capacity = %zu\n", stack->size, stack->capacity);
    fprintf(stderr,"\tdata[%p]\n", stack->data);
    fprintf(stderr, "\t{\n");

    for (size_t i = 0; i < stack->size; i++) {
        fprintf(stderr ,"\t\t*[%zu] = " STK_MODIFIER "\n", i, stack->data[i]);
    }

    for (size_t i = stack->size; i < stack->capacity; i++) {
        fprintf(stderr ,"\t\t [%zu] = " STK_MODIFIER " (POISON) \n", i, stack->data[i]);
    }

    fprintf(stderr ,"\t}\n");
    fprintf(stderr, "}\n");
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


void log_dump_stack(stack_s* stack, const char* const reason) {
    assert(stack != NULL);

    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    fprintf(log_file, "\nFUNCTION: log_dump_stack DATE:%s REASON: %s", __DATE__, reason);

    ON_DBG(fprintf(log_file, "stack_t \"%s\"[%p] created by %s() at %s:%zu", stack->name, stack, stack->function, stack->file, stack->line));

    fprintf(log_file, "{\n");
    fprintf(log_file, "\tsize = %zu capacity = %zu\n", stack->size, stack->capacity);
    fprintf(log_file,"\tdata[%p]\n", stack->data);
    fprintf(log_file, "\t{\n");

    for (size_t i = 0; i < stack->size; i++) {
        fprintf(log_file ,"\t\t*[%zu] = " STK_MODIFIER "\n", i, stack->data[i]);
    }

    for (size_t i = stack->size; i < stack->capacity; i++) {
        fprintf(log_file ,"\t\t [%zu] = " STK_MODIFIER " (POISON!!!) \n", i, stack->data[i]);
    }

    fprintf(log_file ,"\t}\n");
    fprintf(log_file, "}\n");

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}


void log_print(const char* const massage) {
    assert(massage != NULL);

    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    fprintf(log_file ,"\nFUNCTION: log_print DATE:%s\n", __DATE__);
    fputs(massage, log_file);

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}


