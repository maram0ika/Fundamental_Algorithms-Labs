#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>



typedef enum ParseIntStatus{
    PARSE_INT_OK,
    PARSE_INT_NULL,
    PARSE_INT_ERROR
} ParseIntStatus;

typedef enum GuslikStatus {
    GUSLIK_NONONO,
    GUSLIK_YESYESYES
} GuslikStatus;


ParseIntStatus parse_int(const char *string_num, int *result);
GuslikStatus guslik(int guslik_num, const int guslik_r, char *result);

int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Refused - invalid count of arguments!\n");
        return 1;
    }

    int num;
    int r;

    ParseIntStatus num_status = parse_int(argv[1], &num);
    ParseIntStatus r_status = parse_int(argv[2], &r);

    //#обработка ошибок #мне не похуй #стараюсь над лабой
    if (num_status != PARSE_INT_OK || r_status != PARSE_INT_OK ) {
        printf("AAAAAAAAAAA - Error =(");
        return 1;
    }

    if (r < 1 || r > 5) {
        printf("Refused - Invalid value of r!\n R must be in [1, 5]\n");
        return 1;
    }

    //тк макс длина - 32 + '-' + '\0'
    char result[34];

    GuslikStatus result_status =  guslik(num, r, result);

    switch (result_status) {

        case GUSLIK_NONONO:
            printf("Guslik =(\n");
            return 1;

        case GUSLIK_YESYESYES:
            printf("10 -> %d\n", 2^r);
            printf("%d -> %s", num, result);
    }
    






    return 0;
}

ParseIntStatus parse_int(const char *string_num, int *result) {
    if (string_num == NULL || result == NULL) {
        return PARSE_INT_NULL;
    }

    char *end;
    long buffer_result = strtol(string_num, &end, 10);

    if (end == string_num || *end != '\0') {
        return PARSE_INT_ERROR;
    }

    if (INT_MAX < buffer_result || INT_MIN > buffer_result) {
        return PARSE_INT_ERROR;
    }

    *result = (int)buffer_result;
    return PARSE_INT_OK;
}

GuslikStatus guslik(int guslik_num, const int guslik_r, char *guslik_result) {

    if (guslik_result = NULL) {
        return GUSLIK_NONONO;
    }

    int guslik_mask;
    char buffer_guslik_result[34];
    int count = 1;
    int guslik_num_buffer = abs(guslik_num);

    switch(guslik_r) {

        //кароче если что я хотела тут чвести все к 2 случаем : r=1,2,3 и r=4,5 чтоб не дублировать код
        //но тогда было б guslik_mask = 2^guslik_r - 1 а у меня и так как будто тут хвататет арифм операций =/

        case 1:

            guslik_mask = 1;

            while (guslik_num_buffer != 0) {
                buffer_guslik_result[count] = (char)((guslik_mask & guslik_num_buffer) + '0');
                guslik_num_buffer >> 1;
                count++;
            }

        case 2:

            guslik_mask = 3;

            while (guslik_num_buffer != 0) {
                buffer_guslik_result[count] = (char)((guslik_mask & guslik_num_buffer) + '0');
                guslik_num_buffer >> 2;
                count++;
            }
        
        case 3:

            guslik_mask = 7;

            while (guslik_num_buffer != 0) {
                buffer_guslik_result[count] = (char)((guslik_mask & guslik_num_buffer) + '0');
                guslik_num_buffer >> 3;
                count++;
            }
        
        case 4:

            guslik_mask = 15;

            while (guslik_num_buffer != 0) {
                if ((guslik_mask & guslik_num_buffer) < 10) {
                buffer_guslik_result[count] = (char)((guslik_mask & guslik_num_buffer) + '0');
                } else {
                    buffer_guslik_result[count] = (char)(((guslik_mask & guslik_num_buffer) - 10) + 'A');
                }
                guslik_num_buffer >> 4;
                count++;
            }

        case 5:

            guslik_mask = 31;

            while (guslik_num_buffer != 0) {
                if ((guslik_mask & guslik_num_buffer) < 10) {
                buffer_guslik_result[count] = (char)((guslik_mask & guslik_num_buffer) + '0');
                } else {
                    buffer_guslik_result[count] = (char)(((guslik_mask & guslik_num_buffer) - 10) + 'A');
                }
                guslik_num_buffer >> 5;
                count++;
            }
    }

    buffer_guslik_result[count] = '\0';

    if (guslik_num < 0) {
        guslik_result = buffer_guslik_result;
        return GUSLIK_YESYESYES;
    }

     memcpy(guslik_result, buffer_guslik_result + 1, count);
     return GUSLIK_YESYESYES;

}