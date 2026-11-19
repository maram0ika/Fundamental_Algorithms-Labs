#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


typedef struct Application {
    unsigned int id;
    unsigned int start_time;
    unsigned int finish_time;
} Application;

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
    PARSE_UNSIGNED_INT_OUT_OF_RANGE
} ParseUnsignedIntStatus;

typedef enum CheckLenStringStatus {
    CHECK_LEN_STRING_STATUS_OK,
    CHECK_LEN_STRING_STATUS_NULL,
    CHECK_LEN_STRING_STATUS_REFUSED,
} CheckLenStringStatus;

typedef enum ParseApplicationStatus {
    PARSE_APPLICATION_STATUS_OKS,
    PARSE_APPLICATION_STATUS_NULL,
    PARSE_APPLICATION_STATUS_INVALID_VALUE
} ParseApplicationStatus;

typedef enum GetTimeStatus {
    GET_TIME_STATUS_NULL,
    GET_TIME_STATUS_ERROR,
    GET_TIME_STATUS_OK,
} GetTimeStatus;

typedef enum GreedFuncStatus {
    GREED_FUNC_STATUS_NULL,
    GREED_FUNC_STATUS_ERROR,
    GREED_FUNC_STATUS_OK,
} GreedFuncStatus;


ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result);
CheckLenStringStatus check_len_string(char *strochka, size_t strochka_len, const unsigned int min_value, const unsigned int max_value);
ParseApplicationStatus parse_application(char *application_string, Application *application_result);
GetTimeStatus get_time(char *time_string, unsigned int *time_result);
GreedFuncStatus greed_func(const Application *arr, const unsigned int n, const unsigned int k, unsigned int *result);

int comporator_application_ascending(const void *x1, const void *x2);
int comporator_free_time_hall_descending(const void *x1, const void *x2);

int main(void) {

    unsigned int n;
    unsigned int k;

    //в моменте пришло осознание что грамотнее это было бы делать через fgets..
    /*if (scanf("%d %d", &n, &k) != 2) {
        printf("Refused - input error\n");
        return 1;
    } */

    //тк макс длина unsigned инт 10 + 1 + 10 + \n + \0
    char buffer1[23];
    if (fgets(buffer1, sizeof(buffer1), stdin) == NULL) {
        printf("Refused - Input error\n");
        return 1;
    }

    CheckLenStringStatus buffer1_status = check_len_string(buffer1, strlen(buffer1), 4, 23);

    if (buffer1_status != CHECK_LEN_STRING_STATUS_OK) {
        printf("Refused - Input error - CHECK_LEN_STRING_STATUS\n");
        return 1;
    }


    //strtok - новое открытие. прошлые лабы я строки разбивала вручную. развиваемся    
    char *mini_buffer1 = strtok(buffer1, " ");
    char *mini_buffer2 = strtok(NULL, " "); //тк продолжает с того же места на котором остановитлся
    char *mini_buffer3 = strtok(NULL, " ");

    if(mini_buffer2 == NULL || mini_buffer3 != NULL) {
        printf("Refused - Invalid input\n");
        return 1;
    }

    ParseUnsignedIntStatus parse_n_status = parse_unsigned_int(mini_buffer1, &n);
    ParseUnsignedIntStatus parse_k_status = parse_unsigned_int(mini_buffer2, &k);

    if (parse_n_status != PARSE_UNSIGNED_INT_OK || parse_k_status != PARSE_UNSIGNED_INT_OK) {
        printf("Refused - Parse int error (n, k)\n");
        return 1;
    }

    Application *application_arr = NULL;
    Application *temp = malloc(n * sizeof(*application_arr));
    if (temp == NULL) {
        printf("Refused - Memory error\n");
        return 1;
    }
    application_arr = temp;


    for (int i = 0; i < n; i ++) {
            //инт + 1 + 5 + 1 + 5 + \n + \0
            char buffer2[24];
            if (fgets(buffer2, sizeof(buffer2), stdin) == NULL) {
                free(application_arr);
                printf("Refused - input error\n");
                return 1;
            }

            ParseApplicationStatus buffer2_status = parse_application(buffer2, &application_arr[i]);
            if (buffer2_status != PARSE_APPLICATION_STATUS_OKS) {
                printf("Refused - Parsed application error\n");
                free(application_arr);
                return 1;
            }
    }

    qsort(application_arr, n, sizeof(*application_arr),  comporator_application_ascending);

    unsigned int mega_result;
    GreedFuncStatus mega_result_status = greed_func(application_arr, n, k, &mega_result);

    if (mega_result_status != GREED_FUNC_STATUS_OK) {
        free(application_arr);
        printf("Refused - error GREED_FUNC_STATUS\n");
        return 1;
    }

    printf("OTVET OMG o==3 %u\n", mega_result);

    free(application_arr);
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
        return CHECK_LEN_STRING_STATUS_NULL;
    }

    if (strochka_len < min_value || strochka_len > max_value) {
        return CHECK_LEN_STRING_STATUS_REFUSED;
    }

    if (strochka_len == 0 || max_value < 1 || min_value >= max_value) {
        return CHECK_LEN_STRING_STATUS_REFUSED;

    }

    if (strochka[strochka_len - 1] == '\n') {
        strochka[--strochka_len] = '\0';
        return CHECK_LEN_STRING_STATUS_OK;
    }

    return CHECK_LEN_STRING_STATUS_REFUSED;

}



ParseApplicationStatus parse_application(char *application_string, Application *application_result) {
    if (application_string == NULL || application_result == NULL) {
        return PARSE_APPLICATION_STATUS_NULL;
    }

    CheckLenStringStatus check_len_application_string_status = check_len_string(application_string, strlen(application_string), 13, 24);

    if (check_len_application_string_status != CHECK_LEN_STRING_STATUS_OK) {
        return PARSE_APPLICATION_STATUS_INVALID_VALUE;
    }
  
    char *application_id_string = strtok(application_string, " ");
    char *application_start_time_string = strtok(NULL, " ");
    char *application_finish_time_string = strtok(NULL, " ");
    char *application_error = strtok(NULL, " ");

     if(application_start_time_string == NULL || application_finish_time_string == NULL || application_error != NULL) {
        return PARSE_APPLICATION_STATUS_INVALID_VALUE;
    }

    unsigned int application_id;
    ParseUnsignedIntStatus application_id_status = parse_unsigned_int(application_id_string, &application_id);
    
    if (application_id_status != PARSE_UNSIGNED_INT_OK) {
        return PARSE_APPLICATION_STATUS_INVALID_VALUE;
    }

    unsigned int application_start_time;
    unsigned int application_finish_time;

    GetTimeStatus application_start_time_status = get_time(application_start_time_string, &application_start_time);
    GetTimeStatus application_finish_time_status = get_time(application_finish_time_string, &application_finish_time);

    if (application_start_time_status != GET_TIME_STATUS_OK || application_finish_time_status != GET_TIME_STATUS_OK) {
        return PARSE_APPLICATION_STATUS_INVALID_VALUE;
    }

    if (application_start_time > application_finish_time) {
        return PARSE_APPLICATION_STATUS_INVALID_VALUE;
    }
    
    application_result->id = application_id;
    application_result->start_time = application_start_time;
    application_result->finish_time = application_finish_time;

    return PARSE_APPLICATION_STATUS_OKS;

}

GetTimeStatus get_time(char *time_string, unsigned int *time_result) {
    if (time_string == NULL || time_result == NULL) {
        return GET_TIME_STATUS_NULL;
    }

    if (strlen(time_string) != 5 || time_string[2] != ':') {
        return GET_TIME_STATUS_ERROR;
    }

    if (!isdigit((unsigned char)time_string[0]) ||
        !isdigit((unsigned char)time_string[1]) ||
        !isdigit((unsigned char)time_string[3]) ||
        !isdigit((unsigned char)time_string[4])) {
        return GET_TIME_STATUS_ERROR;
    }

    int hours = (time_string[0] - '0') * 10 + (time_string[1] - '0');
    int minutes = (time_string[3] - '0') * 10 + (time_string[4] - '0');

    if (hours > 23 || minutes > 59) {
        return GET_TIME_STATUS_ERROR;
    }

    *time_result = hours * 60 + minutes;
    return GET_TIME_STATUS_OK;
}

int comporator_application_ascending(const void *x1, const void *x2) {

    const Application *a = x1;
    const Application *b = x2;

    if (a->finish_time > b->finish_time) {
        return 1;
    } else if (a->finish_time < b->finish_time) {
        return -1;
    }

    return 0;
}

GreedFuncStatus greed_func(const Application *arr, const unsigned int n, const unsigned int k, unsigned int *result) {
    if (arr == NULL || result == NULL) {
        return GREED_FUNC_STATUS_NULL;
    }

    unsigned int result_buffer = 0;

    if (n == 0 || k == 0) {
        *result = result_buffer;
        return GREED_FUNC_STATUS_OK;
    }

    unsigned int *hall_free_time = calloc(k, sizeof(*hall_free_time));
    if (hall_free_time == NULL) {
        return GREED_FUNC_STATUS_ERROR;
    }

    for (int i = 0; i < n; i ++) {
        for (int j = 0; j < k; j ++) {
            if (arr[i].start_time >= hall_free_time[j]) {
                hall_free_time[j] = arr[i].finish_time;
                result_buffer ++;
                break;
            }
        }
        qsort(hall_free_time, k, sizeof(unsigned int), comporator_free_time_hall_descending);
    }

    free(hall_free_time);

    *result = result_buffer;
    return GREED_FUNC_STATUS_OK;

}

int comporator_free_time_hall_descending(const void *x1, const void *x2) {

    const unsigned int *a = x1;
    const unsigned int *b = x2;

    if (*a > *b) {
        return -1;
    } else if (*a < *b) {
        return 1;
    }

    return 0;
}
