#include "debugging.h"


void log_print(const char* const massage) {
    assert(massage != NULL);

    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    fprintf(log_file, "\nFUNCTION: log_print DATE:%s\n", __DATE__);
    fputs  (massage,  log_file);

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}

void log_print_error(error_code_e error_code, const char* const massage) {
    FILE* log_file = fopen(LOG_FILE_NAME, "a");

    if (log_file == NULL) {
        PRINT_ERROR(ERROR_DURING_OPEN);
        abort();
    }

    fprintf(log_file, "\nFUNCTION: log_print_error DATE:%s\n"    , __DATE__);
    fprintf(log_file, "%s"                                 , massage);
    fprintf(log_file, "ERROR error_code: %d errno_code: %s\n", error_code, strerror(errno));

    if (fclose(log_file) == EOF) {
        PRINT_ERROR(ERROR_DURING_CLOSE);
        abort();
    }
}


void $print_strptr_arr(const char* const arr[], const size_t size) {
    assert(arr != NULL);

    for (size_t block_ind = 0; block_ind < size; block_ind++) {
        ASSERT_FOR_ARR(block_ind, size);
        printf("<%s>\n", arr[block_ind]);
    }
}


void $print_str_matrix(const char* const arr, const size_t size_x, const size_t size_y) {
    assert(arr != NULL);

    for (size_t x = 0; x < size_x; x++) {
        ASSERT_FOR_ARR(x, size_x);
        printf("<%s>\n", ((arr + size_y * x)));
    }
}


void $print_int_arr(const int int_array[], const size_t size) {
    assert(int_array != NULL);

    for (size_t x = 0; x < size; x++) {
        ASSERT_FOR_ARR(x, size);
        printf("%d ", int_array[x]);
    }
    printf("\n");
}


void $print_intptr_arr(const int* const int_array[], const size_t size) {
    assert(int_array != NULL);

    for (size_t x = 0; x < size; x++) {
        ASSERT_FOR_ARR(x, size);

        printf("%d ", *int_array[x]);
    }
    printf("\n");
}


void $debug_qsort(const int* array, const size_t size, const size_t left, const size_t right, void* middle_el, const char* const reason) {
    for (size_t num_ind = 0; num_ind < left; num_ind++) {
        ASSERT_FOR_ARR(num_ind, size);

        printf(COLOR_TEXT("%4d ", BLUE), *((const int* const)array + num_ind ));
    }

    for (size_t num_ind = left; num_ind <= right; num_ind++) {
        ASSERT_FOR_ARR(num_ind, size);

        printf("%4d ", *((const int* const)array + num_ind));
    }

    for (size_t num_ind = right + 1; num_ind < size; num_ind++) {
        ASSERT_FOR_ARR(num_ind, size);

        printf(COLOR_TEXT("%4d ", RED), *((const int* const)array + num_ind));
    }


    printf(COLOR_TEXT(" left = %zu right = %zu middle_el = %d  reason: %s", VIOLET) "\n",
                        left,      right,*(int*)middle_el,             reason);

    getchar();
}


