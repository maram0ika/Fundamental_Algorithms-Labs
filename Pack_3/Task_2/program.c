#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <string.h>


typedef struct Application {
    unsigned int id;
    char strat_time[6];
    char strat_time[6];
} Application;

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
    PARSE_UNSIGNED_INT_OUT_OF_RANGE
} ParseUnsignedIntStatus;

typedef enum CheckLenStringStatus {
    CHECK_LEN_STING_STATUS_OK,
    CHECK_LEN_STING_STATUS_NULL,
    CHECK_LEN_STING_STATUS_REFUSED,
} CheckLenStringStatus;

typedef enum ParseApplicationStatus {
    PARSE_APPLICATION_STATUS_OKS,
    PARSE_APPLICATION_STATUS_NULL,
    PARSE_APPLICATION_STATUS_INVALID_VALUE
} ParseApplicationStatus;

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result);
CheckLenStringStatus check_len_string(char *strochka, size_t strochka_len, const unsigned int min_value, const unsigned int max_value);

int main(void) {

    unsigned int n;
    unsigned int k;

    //в моменте пришло осознание что грамотнее это было бы делать через fgets..
    /*if (scanf("%d %d", &n, &k) != 2) {
        printf("Refused - input error\n");
        return 1;
    } */

    //тк макс длина unsigned инт 10 + 
    char buffer1[23];
    if (fgets(buffer1, sizeof(buffer1), stdin) == NULL) {
        printf("Refused - Input error\n");
        return 1;
    }


    //strtok - новое открытие. прошлые лабы я строки разбивала вручную. развиваемся    
    char *mini_buffer1 = strtok(buffer1, " ");
    char *mini_buffer2 = strtok(NULL, " "); //тк продолжает с того же места на котором остановитлся
    char *mini_buffer3 = strtok(NULL, " ");

    if(mini_buffer2 == NULL || mini_buffer3 != NULL) {
        printf("Refused - Invalid input\n");
    }

    ParseUnsignedIntStatus parse_n_status = parse_unsigned_int(mini_buffer1, &n);
    ParseUnsignedIntStatus parse_k_status = parse_unsigned_int(mini_buffer2, &k);

    if (parse_n_status != PARSE_UNSIGNED_INT_OK || parse_k_status != PARSE_UNSIGNED_INT_OK) {
        printf("Refused - Parse int error (n, k)\n");
        return 1;
    }

    Application *application_arr = NULL;

    char buffer2[23];





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

CheckLenStringStatus check_len_string(char *strochka, size_t strochka_len, const unsigned int min_value, const unsigned int max_value) {
    if (strochka == NULL) {
        return CHECK_LEN_STING_STATUS_NULL;
    }

    if (strochka_len > max_value || strochka_len == 0 || max_value < 1 || min_value >= max_value) {
        return CHECK_LEN_STING_STATUS_REFUSED;

    }

    if (strochka[strochka_len - 1] == '\n') {
        strochka[--strochka_len] = '\0';
    }

}



ParseApplicationStatus parse_application() {
}

//проверка длины строк - дальше уже парсировка и к жадным пупупу