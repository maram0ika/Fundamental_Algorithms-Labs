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

typedef enum ChangeCaseOddStatus  {
    CHANGE_CASE_ODD_OK,
    CHANGE_CASE_ODD_NULL,
    CHANGE_CASE_ODD_MEMORY_ERROR
} ChangeCaseOddStatus;

typedef enum ChangeOrderCharStatus {
    CHANGE_ORDER_CHAR_OK,
    CHANGE_ORDER_CHAR_NULL,
    CHANGE_ORDER_CHAR_MEMORY_ERROR
} ChangeOrderCharStatus;

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result); 
CheckFlagStatus check_flag(const char flag[]);
CheckLenStatus len_of_string(const char str[], int *len_string);
GetReverseStringStatus get_reverse_string(const char str[], char **result);
ChangeCaseOddStatus change_case_odd (const char str[], char **result);
ChangeOrderCharStatus change_order_char(const char str[], char **result);

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
                printf("REFUSED - Func len_of_string - Error while retrieving the length\n");
                return 1;
            case  KEEP_IT_UP_BRO:
                printf("Length - %d\n", len_of_main_string);
                return 0;
        }
    }

    if (argv[1][1] == 'r') {
        char *reverse_string = NULL;
        GetReverseStringStatus reverse_string_status = get_reverse_string(argv[2], &reverse_string);
        switch (reverse_string_status) {
            case GET_REVERSE_STRING_OK:
                printf("Reverse string - %s\n", reverse_string);
                free(reverse_string);
                return 0;
            case GET_REVERSE_STRING_NULL:
                printf("REFUSED - Func reverse_string_status  - NULL received\n");
                free(reverse_string);
                return 1;
            case GET_REVERSE_STRING_MEMORY_ERROR:
                printf("REFUSED - Func reverse_string_status  - Memory problem\n");
                free(reverse_string);
                return 1;
        }
    }

    if (argv[1][1] == 'u') {
        char *changed_case_odd_string = NULL;
        ChangeCaseOddStatus changed_case_odd_string_status  =  change_case_odd(argv[2], &changed_case_odd_string);
        switch (changed_case_odd_string_status) {
            case CHANGE_CASE_ODD_NULL:
                printf("REFUSED - Func сhange_case_odd  - NULL received\n");
                free(changed_case_odd_string);
                return 1;
            
            case CHANGE_CASE_ODD_MEMORY_ERROR:
                printf("REFUSED - Func сhange_case_odd  - Memory problem\n");
                free(changed_case_odd_string);
                return 1;
            case  CHANGE_CASE_ODD_OK:
                printf("A string with the case changed at every odd position - %s\n", changed_case_odd_string);
                free(changed_case_odd_string);
                return 0;
        }

    }

    if (argv[1][1] == 'n') {
        char *change_order_string = NULL;
        ChangeOrderCharStatus changed_order_string_status  =  change_order_char(argv[2], &change_order_string);
        switch (changed_order_string_status) {
            case CHANGE_ORDER_CHAR_NULL:
                printf("REFUSED - Func change_case_odd  - NULL received\n");
                free(change_order_string);
                return 1;
            case CHANGE_ORDER_CHAR_MEMORY_ERROR:
                printf("REFUSED - Func change_case_odd  - Memory problem\n");
                free(change_order_string);
                return 1;
            case CHANGE_ORDER_CHAR_OK:
                printf("The positions of the characters have been successfully swapped - %s\n",change_order_string);
                free(change_order_string);
                return 0;
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
        return PARSE_UNSIGNED_INT_OUT_OF_RANGE;
    }
    *result = (unsigned int)long_result;
    return PARSE_UNSIGNED_INT_OK;
}


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
    if (str == NULL || result == NULL) {
        return GET_REVERSE_STRING_NULL;
    }

    int len;
    CheckLenStatus status_len = len_of_string(str, &len);
    if (status_len == WHAT_THE_HECK_MAN) return GET_REVERSE_STRING_NULL;

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

ChangeCaseOddStatus change_case_odd (const char str[], char **result) {
    if (str == NULL || result == NULL) {
        return CHANGE_CASE_ODD_NULL;
    }
    int len;
    CheckLenStatus status_len = len_of_string(str, &len);
    if (status_len == WHAT_THE_HECK_MAN) return CHANGE_CASE_ODD_NULL;

    char *result1 = NULL;
    result1 = malloc((len + 1) * sizeof(*result1));
    if (result1 == NULL) {
        free(result1);
        return CHANGE_CASE_ODD_MEMORY_ERROR;
    }
    for (int i = 0; i < len; i ++) {
        if (i % 2 == 0) {
            result1[i] = str[i];
        } else if (96 < str[i] && str[i] < 123) {
            result1[i] = str[i] - 32;
        } else result1[i] = str[i];
    }
    result1[len] = '\0';
    *result = result1;
    return CHANGE_CASE_ODD_OK;
}

ChangeOrderCharStatus change_order_char (const char str[], char **result) {
     if (str == NULL || result == NULL) {
        return CHANGE_ORDER_CHAR_NULL;
    }
    int len;

    CheckLenStatus status_len = len_of_string(str, &len);
    if (status_len == WHAT_THE_HECK_MAN) return CHANGE_ORDER_CHAR_NULL;

    char *result1 = NULL;
    result1 = malloc((len + 1) * sizeof(*result1));
    if (result1 == NULL) {
        free(result1);
        return CHANGE_ORDER_CHAR_MEMORY_ERROR;
    }

    int flag = 0;

    for (int i = 0; i < len; i ++) {
        if (str[i] > 47 && str[i] < 58) {
            result1[flag] = str[i];
            flag ++;
        }
    }
    for (int i = 0; i < len; i ++) {
        if ((str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123)) {
            result1[flag] = str[i];
            flag ++;
        }
    }
    for (int i = 0; i < len; i ++) {
        if (!(str[i] > 47 && str[i] < 58) && 
        !((str[i] > 64 && str[i] < 91) || (str[i] > 96 && str[i] < 123))) {
            result1[flag] = str[i];
            flag ++;
        }
    }
    result1[len] = '\0';
    *result = result1;
    return CHANGE_ORDER_CHAR_OK;
}