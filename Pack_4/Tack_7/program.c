#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>



typedef struct MemoryCell {
    char *name;
    int value;
} MemoryCell;



typedef enum InstructionParsingStatus {
    INSTRUCTION_PARSING_MEMORY_ERROR,//
    INSTRUCTION_PARSING_NULL,//
    INSTRUCTION_PARSING_SUM,
    INSTRUCTION_PARSING_SUB,
    INSTRUCTION_PARSING_MULT,
    INSTRUCTION_PARSING_DIV,
    INSTRUCTION_PARSING_MOD,
    INSTRUCTION_PARSING_ASSIGN,
    INSTRUCTION_PARSING_PRINT,//
    INSTRUCTION_PARSING_PRINT_ALL//
} InstructionParsingStatus;

typedef enum BinSearchMemoryCellStatus {
    BIN_SEARCH_OK,
    BIN_SEARCH_NON,
    BIN_SEARCH_NULL
} BinSearchMemoryCellStatus;

typedef enum ReadLineStatus{
    READ_LINE_OK,
    READ_LINE_EOF,
    READ_LINE_NULL,
    READ_LINE_MEMORY_ERROR,
    READ_LINE_FILE_ERROR
} ReadLineStatus;

typedef enum ParseIntStatus{
    PARSE_INT_OK,
    PARSE_INT_NULL,
    PARSE_INT_INVALID_VALUE,
    PARSE_INT_OUT_OF_RANGE
} ParseIntStatus;



InstructionParsingStatus instruction_parsing(const char *instruction, char *arr_vars[3]);
BinSearchMemoryCellStatus bin_search_memory_cell(const MemoryCell *memory_cell_arr, const char *item, const size_t len_memory_cell_arr, int *result);
ParseIntStatus parse_int(const char *num, int *result);
ReadLineStatus read_line(FILE *file, char **line);



int compare_memory_cell_ascending(const void *x1, const void *x2);
void free_func(MemoryCell *arr, const size_t size);


//кароче пока основную работу с файлом реализовываем в мейне 
//тк если делать это в отдельной функции хз как печатать =(

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("REFUSED - Invalid count of argumets\n");
        return 1;
    }


    FILE *instruction_file = fopen(argv[1], "r");
    if (instruction_file == NULL) {
        printf("REFUSED - Error opening the file with instructions\n");
        return 1;
    }



    size_t current_size = 0;
    size_t adjusted_size = 1;

    MemoryCell *memory_cell_arr = malloc(adjusted_size * sizeof(*memory_cell_arr));
    if (memory_cell_arr == NULL) {
        fclose(instruction_file);
        return 1;
    }
    //блят тут шиза с интракшн файл и файл  инстракшн
    //крч инстракшн файл - файл. файл инстракшн не распаршеная строка
    char *file_instruction;
    ReadLineStatus file_instruction_status = read_line(instruction_file, &file_instruction);


    while (file_instruction_status == READ_LINE_OK) {

        char *parsing_instruction_arr_vars[3] = {NULL};
        InstructionParsingStatus parsing_line_status = instruction_parsing(file_instruction, parsing_instruction_arr_vars);

        int value1_index;
        int value2_index;
        int value3_index;

        switch (parsing_line_status) {

            case INSTRUCTION_PARSING_NULL:

                printf("REFUSED - INSTRUCTION_PARSING_NULL\n");

                fclose(instruction_file);

                free_func(memory_cell_arr, current_size);
                free(file_instruction);
                free(parsing_instruction_arr_vars[0]);
                free(parsing_instruction_arr_vars[1]);
                free(parsing_instruction_arr_vars[2]);
                return 1;

           
           
            case INSTRUCTION_PARSING_MEMORY_ERROR:

                printf("REFUSED - INSTRUCTION_PARSING_MEMORY_ERROR\n");

                fclose(instruction_file);

                free_func(memory_cell_arr, current_size);
                free(file_instruction);
                free(parsing_instruction_arr_vars[0]);
                free(parsing_instruction_arr_vars[1]);
                free(parsing_instruction_arr_vars[2]);
                return 1;




            case INSTRUCTION_PARSING_PRINT_ALL:

                for (size_t i = 0; i < current_size; i ++) {
                    printf("%s = %d\n", memory_cell_arr[i].name, memory_cell_arr[i].value);
                }
                break;


            case INSTRUCTION_PARSING_PRINT: {

                BinSearchMemoryCellStatus status_bin1 = bin_search_memory_cell(memory_cell_arr, parsing_instruction_arr_vars[0], current_size, &value1_index);

                switch (status_bin1) {
                    case BIN_SEARCH_OK:
                        printf("%s = %d\n", memory_cell_arr[value1_index].name, memory_cell_arr[value1_index].value);
                        break;

                    case BIN_SEARCH_NON:

                         printf("REFUSED - BIN_SEARCH_NON\n");

                         fclose(instruction_file);
                         free_func(memory_cell_arr, current_size);
                         free(file_instruction);
                         free(parsing_instruction_arr_vars[0]);
                         free(parsing_instruction_arr_vars[1]);
                         free(parsing_instruction_arr_vars[2]);
                         return 1;

                    case BIN_SEARCH_NULL:

                         printf("REFUSED - BIN_SEARCH_NULL\n");

                         fclose(instruction_file);
                         free_func(memory_cell_arr, current_size);
                         free(file_instruction);
                         free(parsing_instruction_arr_vars[0]);
                         free(parsing_instruction_arr_vars[1]);
                         free(parsing_instruction_arr_vars[2]);
                         return 1;

                }

                break;

            }
            case INSTRUCTION_PARSING_ASSIGN: {

                int int_parsing_instruction_arr_vars1;
                ParseIntStatus status_int1 = parse_int(parsing_instruction_arr_vars[1], &int_parsing_instruction_arr_vars1);

                if (status_int1 != PARSE_INT_OK) {

                    BinSearchMemoryCellStatus status_bin2 = bin_search_memory_cell(memory_cell_arr,parsing_instruction_arr_vars[1], current_size, &value2_index);
                    if (status_bin2 != BIN_SEARCH_OK) {
                        printf("REFUSED - BIN_SEARCH_NON\n");
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    int_parsing_instruction_arr_vars1 = memory_cell_arr[value2_index].value;
                }



                //чека 1 арг в который записывается значение

                BinSearchMemoryCellStatus status_bin3 = bin_search_memory_cell(memory_cell_arr, parsing_instruction_arr_vars[0], current_size, &value1_index);
                if (status_bin3 == BIN_SEARCH_NULL) {
                    printf("REFUSED - BIN_SEARCH_NULL\n");
                    fclose(instruction_file);
                    free_func(memory_cell_arr, current_size);
                    free(file_instruction);
                    free(parsing_instruction_arr_vars[0]);
                    free(parsing_instruction_arr_vars[1]);
                    free(parsing_instruction_arr_vars[2]);
                    return 1;
                }

                if (status_bin3 == BIN_SEARCH_NON) {
                    if (adjusted_size == current_size) {
                        adjusted_size *= 2;



                        MemoryCell *temp = realloc(memory_cell_arr, adjusted_size * sizeof(*memory_cell_arr));
                        if (temp == NULL) {
                            printf("REFUSED - Memory error\n");
                            fclose(instruction_file);
                            free_func(memory_cell_arr, current_size);
                            free(file_instruction);
                            free(parsing_instruction_arr_vars[0]);
                            free(parsing_instruction_arr_vars[1]);
                            free(parsing_instruction_arr_vars[2]);
                            return 1;
                        }
                        memory_cell_arr = temp;
                    }
                    memory_cell_arr[current_size].name = malloc(strlen(parsing_instruction_arr_vars[0]) + 1);
                    if (memory_cell_arr[current_size].name == NULL) {
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    strcpy(memory_cell_arr[current_size].name, parsing_instruction_arr_vars[0]);
                    memory_cell_arr[current_size].value = int_parsing_instruction_arr_vars1;
                    current_size ++;
                    qsort(memory_cell_arr, current_size, sizeof(*memory_cell_arr), compare_memory_cell_ascending);
                } else {
                    memory_cell_arr[value1_index].value = int_parsing_instruction_arr_vars1;
                }
                break;


            }
                //ахуеть что так можно
            case INSTRUCTION_PARSING_SUM:
            case INSTRUCTION_PARSING_SUB:
            case INSTRUCTION_PARSING_MULT:
            case INSTRUCTION_PARSING_DIV:
            case INSTRUCTION_PARSING_MOD: {

                int value2;
                int value3;
                ParseIntStatus status_value2 = parse_int(parsing_instruction_arr_vars[1], &value2);
                if (status_value2 != PARSE_INT_OK) {
                    BinSearchMemoryCellStatus status_bin2 = bin_search_memory_cell(memory_cell_arr, parsing_instruction_arr_vars[1], current_size, &value2_index);
                    if (status_bin2 != BIN_SEARCH_OK) {
                        printf("REFUSED - BIN_SEARCH_NON\n");
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    value2 = memory_cell_arr[value2_index].value;
                }

                ParseIntStatus status_value3 = parse_int(parsing_instruction_arr_vars[2], &value3);
                if (status_value3 != PARSE_INT_OK) {
                    BinSearchMemoryCellStatus status_bin3 = bin_search_memory_cell(memory_cell_arr, parsing_instruction_arr_vars[2], current_size, &value3_index);
                    if (status_bin3 != BIN_SEARCH_OK) {
                        printf("REFUSED - BIN_SEARCH_NON\n");
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    value3 = memory_cell_arr[value3_index].value;
                }

                //ниже делаем проверку на диапазоны
                long long operation_result;
                if (parsing_line_status == INSTRUCTION_PARSING_SUM) operation_result = (long long)value2 + value3;
                else if (parsing_line_status == INSTRUCTION_PARSING_SUB) operation_result = (long long)value2 - value3;
                else if (parsing_line_status == INSTRUCTION_PARSING_MULT) operation_result = (long long)value2 * value3;
                else {
                    //диление на ноль
                    if (value3 == 0) {
                        printf("REFUSED \n");
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    if (parsing_line_status == INSTRUCTION_PARSING_DIV) operation_result = value2 / value3;
                    else operation_result = value2 % value3;
                }

                if (operation_result > INT_MAX || operation_result < INT_MIN) {
                    printf("REFUSED");
                    fclose(instruction_file);
                    free_func(memory_cell_arr, current_size);
                    free(file_instruction);
                    free(parsing_instruction_arr_vars[0]);
                    free(parsing_instruction_arr_vars[1]);
                    free(parsing_instruction_arr_vars[2]);
                    return 1;
                }

                BinSearchMemoryCellStatus status_bin_result = bin_search_memory_cell(memory_cell_arr, parsing_instruction_arr_vars[0], current_size, &value1_index);
                if (status_bin_result == BIN_SEARCH_NULL) {
                    printf("REFUSED - BIN_SEARCH_NULL\n");
                    fclose(instruction_file);
                    free_func(memory_cell_arr, current_size);
                    free(file_instruction);
                    free(parsing_instruction_arr_vars[0]);
                    free(parsing_instruction_arr_vars[1]);
                    free(parsing_instruction_arr_vars[2]);
                    return 1;
                }

                if (status_bin_result == BIN_SEARCH_NON) {
                    if (adjusted_size == current_size) {
                        adjusted_size *= 2;
                        MemoryCell *temp = realloc(memory_cell_arr, adjusted_size * sizeof(*memory_cell_arr));
                        if (temp == NULL) {
                            printf("REFUSED - Memory error\n");
                            fclose(instruction_file);
                            free_func(memory_cell_arr, current_size);
                            free(file_instruction);
                            free(parsing_instruction_arr_vars[0]);
                            free(parsing_instruction_arr_vars[1]);
                            free(parsing_instruction_arr_vars[2]);
                            return 1;
                        }
                        memory_cell_arr = temp;
                    }
                    memory_cell_arr[current_size].name = malloc(strlen(parsing_instruction_arr_vars[0]) + 1);
                    if (memory_cell_arr[current_size].name == NULL) {
                        fclose(instruction_file);
                        free_func(memory_cell_arr, current_size);
                        free(file_instruction);
                        free(parsing_instruction_arr_vars[0]);
                        free(parsing_instruction_arr_vars[1]);
                        free(parsing_instruction_arr_vars[2]);
                        return 1;
                    }
                    strcpy(memory_cell_arr[current_size].name, parsing_instruction_arr_vars[0]);
                    memory_cell_arr[current_size].value = (int)operation_result;
                    current_size ++;
                    qsort(memory_cell_arr, current_size, sizeof(*memory_cell_arr), compare_memory_cell_ascending);
                } else {
                    memory_cell_arr[value1_index].value = (int)operation_result;
                }
                break;



        }

        }





        free(file_instruction);
        free(parsing_instruction_arr_vars[0]);
        free(parsing_instruction_arr_vars[1]);
        free(parsing_instruction_arr_vars[2]);
        file_instruction_status = read_line(instruction_file, &file_instruction);

    }



    switch (file_instruction_status) {

        case READ_LINE_OK:
            break;
        case READ_LINE_EOF:
            break;

        case READ_LINE_NULL:
            printf("REFUSED - Error reading the file with instructions - READ_LINE_NULL\n");
            fclose(instruction_file);
            return 1;

        case READ_LINE_MEMORY_ERROR:
            printf("REFUSED - Error reading the file with instructions - READ_LINE_MEMORY_ERROR\n");
            fclose(instruction_file);
            return 1;

        case READ_LINE_FILE_ERROR:
            printf("REFUSED - Error reading the file with instructions - READ_LINE_FILE_ERROR\n");
            fclose(instruction_file);
            return 1;

    }

    fclose(instruction_file);
    free_func(memory_cell_arr, current_size);
    return 0;

}



ReadLineStatus read_line(FILE *file, char **line) {

    if (file == NULL || line == NULL) {
        return READ_LINE_NULL;
    }



    *line = NULL;

    char *buffer_line = NULL;
    size_t size_buffer_line = 0;
    int c = fgetc(file);

    while (c != EOF && isspace((unsigned char)c)) {
        c = fgetc(file);
    }

    if (c == EOF ) {
        if (feof(file) != 0 ) {
        return READ_LINE_EOF;
    } else if (ferror(file) != 0) {
        return READ_LINE_FILE_ERROR;

    }}

    while (c != ';' && c != EOF) {

        char *temp = realloc(buffer_line, (size_buffer_line + 1) * sizeof(*buffer_line));
        if (temp == NULL) {
            free(buffer_line);
            return READ_LINE_MEMORY_ERROR;

        }

        buffer_line = temp;
        buffer_line[size_buffer_line] = c;
        size_buffer_line ++;
        c = fgetc(file);

    }


    if (c == EOF && ferror(file) != 0) {
        free(buffer_line);
        return READ_LINE_FILE_ERROR;
    }



    char *temp = realloc(buffer_line, (size_buffer_line + 1) * sizeof(*buffer_line));
    if (temp == NULL) {
        free(buffer_line);
        return READ_LINE_MEMORY_ERROR;
    }

    buffer_line = temp;
    buffer_line[size_buffer_line] = '\0';


    if (c == EOF && ferror(file) != 0) {
        free(buffer_line);
        return READ_LINE_FILE_ERROR;
    }

    *line = buffer_line;
    return READ_LINE_OK;

}





// arr_vars [0 - которому присвиваем или печатаем; 1, 2 - переменные между котороми осуществляются действа]

InstructionParsingStatus instruction_parsing(const char *instruction, char *arr_varsss[3]) {

    if (instruction == NULL || arr_varsss == NULL) {
        return INSTRUCTION_PARSING_NULL;
    }



    arr_varsss[0] = NULL;
    arr_varsss[1] = NULL;
    arr_varsss[2] = NULL;

    char *arr_vars[3] = {NULL};


    size_t size_arr0 = 0;
    size_t size_arr1 = 0;
    size_t size_arr2 = 0;
    //print all

    if (strcmp("print", instruction) == 0) {
        return INSTRUCTION_PARSING_PRINT_ALL;
    }

    //print var
    if (strlen(instruction) > 6) {

        char result[6];

        memcpy(result, instruction, 5);
        result[5] = '\0';
        if (strcmp("print", result) == 0) {

            size_t start = 5;
            while (isspace((unsigned char)instruction[start])) start ++;
            size_arr0 = strlen(instruction + start);

            arr_vars[0] = malloc((size_arr0 + 1) * sizeof(*arr_vars[0]));
            if (arr_vars[0] == NULL) return INSTRUCTION_PARSING_MEMORY_ERROR;
            memcpy(arr_vars[0], instruction + start, size_arr0 + 1);
            arr_varsss[0] = arr_vars[0];

            return INSTRUCTION_PARSING_PRINT;

        }

    }



    size_arr0 = 0;

    //считываем arr[0]

    for (int i = 0; instruction[i] != '='; i ++) {
        char *temp = realloc(arr_vars[0], (size_arr0 + 1) * sizeof(*arr_vars[0]));
        if (temp == NULL) {
            free(arr_vars[0]);
            return INSTRUCTION_PARSING_MEMORY_ERROR;

        }

        arr_vars[0] = temp;
        arr_vars[0][size_arr0] = instruction[i];
        size_arr0 ++;
    }

    char *temp = realloc(arr_vars[0], (size_arr0 + 1) * sizeof(*arr_vars[0]));

    if (temp == NULL) {
        free(arr_vars[0]);
        return INSTRUCTION_PARSING_MEMORY_ERROR;
    }

    arr_vars[0] = temp;
    arr_vars[0][size_arr0] = '\0';


    //получаем arr[1]

    for (size_t i = size_arr0 + 1; isalnum((unsigned char)instruction[i]) != 0 || (i == size_arr0 + 1 && instruction[i] == '-' && isdigit((unsigned char)instruction[i + 1])); i ++) {

        char *temp1 = realloc(arr_vars[1], (size_arr1 + 1) * sizeof(*arr_vars[1]));

        if (temp1 == NULL) {
            free(arr_vars[0]);
            free(arr_vars[1]);
            return INSTRUCTION_PARSING_MEMORY_ERROR;
        }

        arr_vars[1] = temp1;
        arr_vars[1][size_arr1] = instruction[i];
        size_arr1 ++;

    }



    char *temp1 = realloc(arr_vars[1], (size_arr1 + 1) * sizeof(*arr_vars[1]));

    if (temp1 == NULL) {
        free(arr_vars[0]);
        free(arr_vars[1]);
        return INSTRUCTION_PARSING_MEMORY_ERROR;
    }



    arr_vars[1] = temp1;
    arr_vars[1][size_arr1] = '\0';

    //получаем последний аргумент и выясням что за операйшон

    size_t now_len = size_arr0 + 1 + size_arr1; 


    if (now_len != strlen(instruction)) {

        //получаем arr[2]

        for (int i = now_len + 1;  instruction[i] != '\0'; i ++) {

            char *temp2 = realloc(arr_vars[2], (size_arr2 + 1) * sizeof(*arr_vars[2]));
            if (temp2 == NULL) {
                free(arr_vars[0]);
                free(arr_vars[1]);
                free(arr_vars[2]);
                return INSTRUCTION_PARSING_MEMORY_ERROR;

            }

            arr_vars[2] = temp2;
            arr_vars[2][size_arr2] = instruction[i];
            size_arr2 ++;

        }



        char *temp2 = realloc(arr_vars[2], (size_arr2 + 1) * sizeof(*arr_vars[2]));

        if (temp2 == NULL) {
            free(arr_vars[0]);
            free(arr_vars[1]);
            free(arr_vars[2]);
            return INSTRUCTION_PARSING_MEMORY_ERROR;
        }

        arr_vars[2] = temp2;
        arr_vars[2][size_arr2] = '\0';



        arr_varsss[0] = arr_vars[0];
        arr_varsss[1] = arr_vars[1];
        arr_varsss[2] = arr_vars[2];

        if (instruction[now_len] == '+') {
            return INSTRUCTION_PARSING_SUM;
        }

        if (instruction[now_len] == '-') {
            return INSTRUCTION_PARSING_SUB;
        }

        if (instruction[now_len] == '*') {
            return INSTRUCTION_PARSING_MULT;
        }

        if (instruction[now_len] == '/') {
            return INSTRUCTION_PARSING_DIV;
        }

        if (instruction[now_len] == '%') {
            return INSTRUCTION_PARSING_MOD;
        }

    } else {

        arr_varsss[0] = arr_vars[0];
        arr_varsss[1] = arr_vars[1];
        return INSTRUCTION_PARSING_ASSIGN;

    }

    free(arr_vars[0]);
    free(arr_vars[1]);
    free(arr_vars[2]);
    return INSTRUCTION_PARSING_NULL;

 }







 BinSearchMemoryCellStatus bin_search_memory_cell(const MemoryCell *memory_cell_arr, const char *item, const size_t len_memory_cell_arr, int *result) {

        if (memory_cell_arr == NULL || result == NULL || item == NULL) {
            return  BIN_SEARCH_NULL;
        }

        if (len_memory_cell_arr == 0) {
            return BIN_SEARCH_NON;
        }

        int low = 0;
        int high = len_memory_cell_arr - 1;

        while (low <= high) {
            int mid = (low + high) / 2;
            MemoryCell guess = memory_cell_arr[mid];
            int cmp = strcmp(guess.name, item);

            if (cmp == 0) {
                *result = mid;
                return  BIN_SEARCH_OK;
            }

            if (cmp > 0) {
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        return BIN_SEARCH_NON;
}



int compare_memory_cell_ascending(const void *x1, const void *x2) {

    const MemoryCell *a = x1;
    const MemoryCell *b = x2;

    int status =  strcmp(a->name, b->name);
    if (status > 0) {
        return 1;
    } else if (status < 0) {
        return -1;
    }

    return 0;

}



ParseIntStatus parse_int(const char *num, int *result) {
    if (num == NULL || result == NULL) {
        return PARSE_INT_NULL;
    }

    char *end;
    long long_result = strtol(num, &end, 10);
    if (end == num || *end != '\0') {
        return PARSE_INT_INVALID_VALUE;
    }

    if (long_result > INT_MAX || long_result < INT_MIN) {
        return PARSE_INT_OUT_OF_RANGE;
    }

    *result = (int)long_result;
    return PARSE_INT_OK;

}



void free_func(MemoryCell *arr, const size_t size) {
    for (size_t i = 0; i < size; i ++) {
        free(arr[i].name);
    }
    free(arr);
}
