#include <stdio.h>
#include <stdarg.h>
#include <limits.h>
#include <stdlib.h>

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
} ParseUnsignedIntStatus;

typedef enum MagnumOpusStatus{
    MAGNUM_OPUS_NULL,
    MAGNUM_OPUS_OK,
    MAGNUM_OPUS_FILE_ERROR,
    MAGNUM_OPUS_MEMORY_ERROR,
    MAGNUM_OPUS_INVALID_VALUE
} MagnumOpusStatus;

typedef struct OTVET {
    const char *file_name;
    unsigned int line_number;
    unsigned int character_number;
} OTVET;

ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result); 
MagnumOpusStatus magnum_opus(const char *substring, 
    const int count, 
    OTVET **result, 
    size_t *result_size, ...);

int main(int argc, char *argv[]) {

    if (argc < 3) {
        printf("Invalid count of arguments!\n");
        return 1;
    }

    unsigned int count_of_files;
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
        printf("Invalid count of arguments (files)!\n");
        return 1;
    }

    OTVET *arr_answers = NULL;
    size_t arr_answers_size = 0;

    //капец конечно........... тут только ручками менять мэйн или заниматься уродование кода через switch(count_of_files)
    MagnumOpusStatus status_arr_answer = magnum_opus(
        argv[1], 
        count_of_files,
        &arr_answers,
        &arr_answers_size, 
        argv[3], argv[4]);  //<----- кринж

    
    switch (status_arr_answer) {
        case MAGNUM_OPUS_OK:
            break;
        case MAGNUM_OPUS_NULL:
            printf("REFUSED -MAGNUM_OPUS_NULL\n");
            free(arr_answers);
            return 1;
        case MAGNUM_OPUS_FILE_ERROR:
            printf("REFUSED - MAGNUM_OPUS_FILE_ERROR\n");
            free(arr_answers);
            return 1;
        case MAGNUM_OPUS_MEMORY_ERROR:
            printf("REFUSED - MAGNUM_OPUS_MEMORY_ERROR\n");
            free(arr_answers);
            return 1;
        case MAGNUM_OPUS_INVALID_VALUE:
            printf("REFUSED - MAGNUM_OPUS_INVALID_VALUE\n");
            free(arr_answers);
            return 1;
    }

    for (int i = 0; i < arr_answers_size; i ++) {
        printf("File name - %s\nLine - %u\nChar - %u\n", 
            arr_answers[i].file_name, 
            arr_answers[i].line_number, 
            arr_answers[i].character_number);
        printf("-------------\n");
    }

    free(arr_answers);
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

//блят уже когда дописала поняла что сложность 2n если не больше тк 2 раза проходимся.
// но так как будто более читабельно и проще учесть все подстроки когда одна подстрока заходит на другую
MagnumOpusStatus magnum_opus(const char *substring, const int count, OTVET **result, size_t *result_size, ...) {

    if (substring == NULL || result == NULL || result_size == NULL) {
        return MAGNUM_OPUS_NULL;
    }

    if (count < 1 || substring[0] == '\0') {
        return MAGNUM_OPUS_INVALID_VALUE;
    }
       
    *result = NULL;
    *result_size = 0;

    va_list list_of_file_names;
    va_start(list_of_file_names, result_size);

    unsigned int len_substring = 0;
    for (int i = 0; substring[i] != '\0'; i++) {
        len_substring ++;
    }

    OTVET *buffer = NULL;
    size_t buffer_size = 0;

    for (int i = 0; i < count; i++) {
         const char *file_name1 = va_arg(list_of_file_names, const char *);
         //^и вот этот мини кусочек стоил некрасивогоо нефункционального мэйна???? черти

         if (file_name1 == NULL) {
            free(buffer);
            va_end(list_of_file_names);
            return MAGNUM_OPUS_NULL;
        }
        FILE *file = fopen(file_name1, "r");
        if (file == NULL) {
            free(buffer);
            va_end(list_of_file_names);
            return MAGNUM_OPUS_FILE_ERROR;
        }
        
        char *buffer_file_arr = NULL;
        size_t size_buffer_file_arr = 0;
        int c = fgetc(file);

        while (c != EOF) {
            char *temp = realloc(buffer_file_arr, (size_buffer_file_arr + 1) * sizeof(*buffer_file_arr));
            if (temp == NULL) {
                fclose(file);
                free(buffer);
                free(buffer_file_arr);
                va_end(list_of_file_names);
                return MAGNUM_OPUS_MEMORY_ERROR;
            }

            buffer_file_arr = temp;
            buffer_file_arr[size_buffer_file_arr] = c;
            size_buffer_file_arr ++;

            c = fgetc(file);
        }

        if (ferror(file) != 0) {
            fclose(file);
            free(buffer);
            free(buffer_file_arr);
            va_end(list_of_file_names);
            return MAGNUM_OPUS_FILE_ERROR;
        }



        int count_line = 1;
        int count_char = 1;

        for (int i = 0; i < size_buffer_file_arr; i ++) {
            int flag = 0;
            if (buffer_file_arr[i] == substring[0]) {

            if (size_buffer_file_arr - i >= len_substring) {
            flag = 1;
                for (int j = 1; j < len_substring; j ++) {
                    
                        if (buffer_file_arr[i + j] != substring[j]) {
                            flag = 0;
                            break;
                        }
                }
            }
            }
            if (flag) {
                OTVET *temp = realloc(buffer, (buffer_size + 1) * sizeof(*buffer));
                if (temp == NULL) {
                    free(buffer_file_arr);
                    fclose(file);
                    free(buffer);
                    va_end(list_of_file_names);
                    return MAGNUM_OPUS_MEMORY_ERROR;
                }
                buffer = temp;
                buffer[buffer_size] = (OTVET){
                    .file_name = file_name1,
                    .line_number = count_line,
                    .character_number = count_char
                };
                buffer_size ++;
            }

            count_char ++;
            if (buffer_file_arr[i] == '\n') {
                count_line ++;
                count_char = 1;
            }
        }
        free(buffer_file_arr);
        fclose(file);
    }

    va_end(list_of_file_names);

    *result = buffer;
    *result_size = buffer_size;
    return MAGNUM_OPUS_OK;

}
