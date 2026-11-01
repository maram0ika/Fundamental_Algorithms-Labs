#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
} ParseUnsignedIntStatus;

typedef enum MagnumOpusStatus{
    OK,
    ERROR
} MagnumOpusStatus;

typedef struct OTVET {
    char *file_name;
    unsigned int line_number;
    unsigned int character_number;
} OTVET;

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result); 

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Invalid count of arguments!\n");
        return 1;
    }

    int count_of_files;

    ParseUnsignedIntStatus count_of_files_status = parse_unsigned_int(argv[2], &count_of_files);
    switch( count_of_files_status) {
        case PARSE_UNSIGNED_INT_OK:
            break;
        case PARSE_UNSIGNED_INT_NULL:
            printf("REFUSED - PARSE_UNSIGNED_INT_NULL\n");
            return 1;
        case PARSE_UNSIGNED_INT_INVALID_VALUE:
            printf("REFUSED - PARSE_UNSIGNED_INT_INVALID_VALUE\n");
            return 1;
    }
    
    if (argc != count_of_files + 3) {
        if (argc < 3) {
        printf("Invalid count of arguments (files)!\n");
        return 1;
    }
    }


    return 0;
}

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result) {
    if (num == NULL || result == NULL) {
        return PARSE_UNSIGNED_INT_NULL;
    }

    char *end;
    long long_result = strtol(num, &end, 10);
    if (end == num || *end != '\0') {
        return PARSE_UNSIGNED_INT_INVALID_VALUE;
    }
    if (long_result > UINT_MAX || long_result < 0) {
        return PARSE_UNSIGNED_INT_INVALID_VALUE;
    }
    *result = (unsigned int)long_result;
    return PARSE_UNSIGNED_INT_OK;
}


MagnumOpusStatus magnum_opus(const char *substring, const int count, OTVET **result, ...) {

    va_list list_of_file_names;
    va_start(list_of_file_names, count);
}  