#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
    PARSE_UNSIGNED_INT_OUT_OF_RANGE
} ParseUnsignedIntStatus;

typedef enum CheckFlagStatus {
    CHECK_FLAG_OK,
    CHECK_FLAG_NULL,
    CHECK_FLAG_INVALID_VALUE
} CheckFlagStatus;

typedef enum CheckLenStatus {
    WHAT_THE_HECK_MAN,
    KEEP_IT_UP_BRO
} CheckLenStatus;

typedef enum GetReverseStringStatus {
    GET_REVERSE_STRING_OK,
    GET_REVERSE_STRING_NULL,
    GET_REVERSE_STRING_MEMORY_ERROR
} GetReverseStringStatus;

//typedef enum GetArrOfStringStatus {
//} GetArrOfStringStatus;

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result); 
CheckFlagStatus check_flag(const char flag[]);
CheckLenStatus len_of_string(const char str[], int *len_string);
GetReverseStringStatus get_reverse_string(const char str[], char **result);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("REFUSED - Invalid count of arguments!\n");
        return 1;
    }

    CheckFlagStatus flag_status = check_flag(argv[1]);
    switch (flag_status) {
        case CHECK_FLAG_OK:
            break;
        case CHECK_FLAG_NULL:
            printf("REFUSED - Invalid value of flag - NULL received\n");
            return 1;
        case CHECK_FLAG_INVALID_VALUE:
            printf("REFUSED - Invalid value of flag\n");
            return 1;
    }

    if (argv[1][1] == 'l') {
        int len_of_main_string;
        CheckLenStatus status_check_len = len_of_string(argv[2], &len_of_main_string);
        switch (status_check_len){
            case  WHAT_THE_HECK_MAN:
                printf("REFUSED - Error while retrieving the length\n");
                return 1;
            case  KEEP_IT_UP_BRO:
                printf("Length - %d\n", len_of_main_string);
                return 0;
        }
    }

    if (argv[1][1] == 'r') {
        char *reverse_string;
        GetReverseStringStatus reverse_string_status = get_reverse_string(argv[2], &reverse_string);
        switch (reverse_string_status) {
            case GET_REVERSE_STRING_OK:
                printf("Reverse string - %s\n", reverse_string);
                return 0;
            case
        }
    }

    // printf("Flag - %s\nString - %s\n", argv[1], argv[2]);


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
        return PARSE_UNSIGNED_INT_OUT_OF_RANGE;
    }
    *result = (unsigned int)long_result;
    return PARSE_UNSIGNED_INT_OK;
}

//GetArrOfStringStatus get_string(const char *str, ) {

CheckFlagStatus check_flag(const char flag[]) {
    if (flag == NULL) {
        return CHECK_FLAG_NULL;
    }
    if (flag[0] != '-'|| flag[1] == '\0' || flag[2] != '\0') {
        return CHECK_FLAG_INVALID_VALUE;
    }
    if (
        flag[1] == 'l' ||
        flag[1] == 'r' ||
        flag[1] == 'u' ||
        flag[1] == 'n' ||
        flag[1] == 'c'
    ) return CHECK_FLAG_OK;
    return CHECK_FLAG_INVALID_VALUE;
}

CheckLenStatus len_of_string(const char str[], int *len_string) {
    if (str == NULL || len_string == NULL) {
        return WHAT_THE_HECK_MAN;
    }
    int len = 0;
    char x = str[0];
    while (x != '\0') {
        len ++;
        x = str[len];
    }
    *len_string = len;
    return KEEP_IT_UP_BRO;
}

GetReverseStringStatus get_reverse_string(const char str[], char **result) {
    if (str == NULL || *result == NULL) {
        return GET_REVERSE_STRING_NULL;
    }

    int len;
    CheckLenStatus status_len = len_of_string(str, &len);
    char *result1 = NULL;
    result1 = malloc((len + 1) * sizeof(*result1));
    if (result1 == NULL) {
        free(result1);
        return GET_REVERSE_STRING_MEMORY_ERROR;
    }
    for (int i = 0; i < len; i ++) {
        result1[len - i - 1] = str[i];
    }
    result1[len] = '\0';
    *result = result1;
    return GET_REVERSE_STRING_OK;
}