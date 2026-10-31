#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h> //для проверки дабла

typedef enum CheckFileNameStatus {
    CHECK_FILE_NAME_OK,
    CHECK_FILE_NAME_NULL
} CheckFileNameStatus;

typedef enum CheckFlagStatus {
    CHECK_FLAG_OK,
    CHECK_FLAG_NULL,
    CHECK_FLAG_INVALID_VALUE
} CheckFlagStatus;

typedef struct Employee {
    unsigned int id;
    char *name;
    char *surname;
    double salary;
} Employee;

typedef enum ReadLineStatus{
    READ_LINE_OK,
    READ_LINE_EOF,
    READ_LINE_NULL,
    READ_LINE_MEMORY_ERROR,
    READ_LINE_FILE_ERROR
} ReadLineStatus;

typedef enum ParseEmployeeStatus {
    PARSE_EMPLOYEE_OK,
    PARSE_EMPLOYEE_NULL,
    PARSE_EMPLOYEE_INVALID_VALUE,
    PARSE_EMPLOYEE_MEMORY_ERROR
} ParseEmployeeStatus;


CheckFlagStatus check_flag(const char flag[]);
CheckFileNameStatus check_file_name(const char file_name[]);
ReadLineStatus read_line(FILE *file, char **line);
ParseEmployeeStatus parse_employee(const char *line, Employee *result);
void free_employees(Employee *employees, size_t count);
int compare_employees_ascending(const void *a, const void *b);
int compare_employees_descending(const void *a, const void *b);

int main (int argc, char *argv[]) {

    if (argc != 4) {
        printf("Invalid count of argumets\n");
        return 1;
    }

    CheckFileNameStatus file_input_name_status = check_file_name(argv[1]);
    switch (file_input_name_status) {
        case CHECK_FILE_NAME_OK:
            break;
        case CHECK_FILE_NAME_NULL:
            printf("REFUSED - Invalid value of input file - NULL received\n");
            return 1;
    }

    CheckFileNameStatus file_output_name_status = check_file_name(argv[3]);
    switch (file_output_name_status) {
        case CHECK_FILE_NAME_OK:
            break;
        case CHECK_FILE_NAME_NULL:
            printf("REFUSED - Invalid value of output file - NULL received\n");
            return 1;
    }

    if (strcmp(argv[1], argv[3]) == 0) {
        printf("REFUSED - Idiot\n");
        return 1;
    }

    CheckFlagStatus flag_status = check_flag(argv[2]);
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

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("REFUSED - Error opening the file with input data\n");
        return 1;
    }

    Employee *emloyeers = NULL;
    size_t size_employeers = 0;

    ReadLineStatus file_line_status;

    do {
        char *file_line = NULL;
        file_line_status =  read_line(input_file, &file_line);

        switch (file_line_status) {
            case READ_LINE_OK:
                break;
            case READ_LINE_EOF:
                break;
            case READ_LINE_NULL:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
            case READ_LINE_MEMORY_ERROR:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
            case READ_LINE_FILE_ERROR:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
        }

        if (file_line_status == READ_LINE_EOF) break;

        Employee employee1 = {
             .id = 0,
             .name = NULL,
             .surname = NULL,
             .salary = 0.0
        };

        ParseEmployeeStatus employeer1_status = parse_employee(file_line, &employee1);

        switch (employeer1_status) {
            case PARSE_EMPLOYEE_OK:
                break;
            case PARSE_EMPLOYEE_NULL:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                free(employee1.name);
                free(employee1.surname);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
            case PARSE_EMPLOYEE_MEMORY_ERROR:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                free(employee1.name);
                free(employee1.surname);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
            case PARSE_EMPLOYEE_INVALID_VALUE:
                fclose(input_file);
                free(file_line);
                free_employees(emloyeers, size_employeers);
                free(employee1.name);
                free(employee1.surname);
                printf("REFUSED - aaaaaaaaaaaa");
                return 1;
        }

        Employee *temp = realloc(emloyeers, (size_employeers + 1) * sizeof(*emloyeers));
        if (temp == NULL) {
            free_employees(emloyeers, size_employeers);
            free(employee1.name);
            free(employee1.surname);
            fclose(input_file);
            free(file_line);
            printf("----hui----");            
            return 1;
        }

        emloyeers = temp;

        emloyeers[size_employeers] = employee1;
        size_employeers ++;

        free(file_line);

    } while (file_line_status == READ_LINE_OK);

    if (argv[2][1] == 'a') {
        qsort(emloyeers, size_employeers, sizeof(*emloyeers), compare_employees_ascending);
    } else {
        qsort(emloyeers, size_employeers, sizeof(*emloyeers), compare_employees_descending);
    }


    fclose(input_file);

    FILE *output_file = fopen(argv[3], "w");
    if (output_file == NULL) {
        free_employees(emloyeers, size_employeers);
        printf("REFUSED - Error opening the file for output data\n");
        return 1;
    }

    for (int i = 0; i < size_employeers; i++) {
        fprintf(output_file, "%d %s %s %.2f\n", emloyeers[i].id, emloyeers[i].name, emloyeers[i].surname, emloyeers[i].salary);
    }

    free_employees(emloyeers, size_employeers);
    fclose(output_file);

    return 0;
}

CheckFlagStatus check_flag(const char flag[]) {
    if (flag == NULL) {
        return CHECK_FLAG_NULL;
    }

    if (!(flag[0] == '-' || flag[0] == '/')
    || flag[1] == '\0' || 
    flag[2] != '\0') {
        return CHECK_FLAG_INVALID_VALUE;
    }
    if (
        flag[1] == 'a' ||
        flag[1] == 'd'
    ) return CHECK_FLAG_OK;
    return CHECK_FLAG_INVALID_VALUE;
}


CheckFileNameStatus check_file_name(const char file_name[]) {
    if (file_name == NULL) {
        return  CHECK_FILE_NAME_NULL;
    }
    return CHECK_FILE_NAME_OK;


}

ReadLineStatus read_line(FILE *file, char **line) {
    if (file == NULL || line == NULL) {
        return READ_LINE_NULL;
    }

    *line = NULL;

    char *buffer_line = NULL;
    size_t size_buffer_line = 0;
    int c = fgetc(file);

    if (c == EOF ) {
        if (feof(file) != 0 ) {
        return READ_LINE_EOF;
    } else if (ferror(file) != 0) {
        return READ_LINE_FILE_ERROR;
    }}

    while (c != '\n' && c != EOF) {
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

ParseEmployeeStatus parse_employee(const char *line, Employee *result) {
    if (line == NULL || result == NULL) {
        return PARSE_EMPLOYEE_NULL;
    }

    //я ебала эти махинации надо было реально просто через ебаный fgets все делать. 
    //крч делим строку по пробелам и закидываем каждое слово в массивчик

    char **employee_buffer = NULL;
    size_t i = 0;     //cлова
    size_t j = 0;      //символы в слове
    size_t pos = 0;    //обзтй счетчик
    char c = line[pos];

    while (c != '\0') {
        
        while (c == ' ' || c == '\t') {
            pos++;
            c = line[pos];
        }

        if (c == '\0') {
            break;
        }

        char **temp1 = realloc(employee_buffer, (i + 1) * sizeof(*employee_buffer));

        if (temp1 == NULL) {
            for (size_t k = 0; k < i; k++) {
                free(employee_buffer[k]);
            }
            free(employee_buffer);
            return PARSE_EMPLOYEE_MEMORY_ERROR;
        }

        employee_buffer = temp1;
        employee_buffer[i] = NULL;

        j = 0;

        while (c != ' ' && c != '\t' && c != '\0') {
            char *temp2 = realloc(employee_buffer[i],(j + 1) * sizeof(*employee_buffer[i]));
            if (temp2 == NULL) {
                for (size_t k = 0; k <= i; k++) {
                    free(employee_buffer[k]);
                }
                free(employee_buffer);
                return PARSE_EMPLOYEE_MEMORY_ERROR;
            }

            employee_buffer[i] = temp2;

            employee_buffer[i][j] = c;
            j++;
            pos++;

            c = line[pos];
        }
        char *temp2 = realloc(employee_buffer[i], (j + 1) * sizeof(*employee_buffer[i]));

        if (temp2 == NULL) {
            for (size_t k = 0; k <= i; k++) {
                free(employee_buffer[k]);
            }

            free(employee_buffer);
            return PARSE_EMPLOYEE_MEMORY_ERROR;
        }

        employee_buffer[i] = temp2;
        employee_buffer[i][j] = '\0';

        i++;
    }

    if (i != 4) {
        for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
        return PARSE_EMPLOYEE_INVALID_VALUE;
    }

    //парсим теперь каждую залупу
    ///////////////////////////////////////////////////////
    Employee employee1;

    //id -> unsigned int
    char *id_end;
    long long_result = strtol(employee_buffer[0], &id_end, 10);
    if (id_end == employee_buffer[0] || *id_end != '\0') {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
        return PARSE_EMPLOYEE_INVALID_VALUE;
    }
    if (long_result > UINT_MAX || long_result < 0) {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
        return PARSE_EMPLOYEE_INVALID_VALUE;
    }
    employee1.id = (unsigned int)long_result;

    //salary -> double
    char *salary_end;
    double double_result = strtod(employee_buffer[3], &salary_end);
    if (salary_end == employee_buffer[3] || *salary_end != '\0') {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
        return PARSE_EMPLOYEE_INVALID_VALUE;
    }
    if (!isfinite(double_result) || double_result < 0) {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
    return PARSE_EMPLOYEE_INVALID_VALUE;
    }
    employee1.salary = double_result;

    //name and surname

    size_t name_size = 0;
    size_t surname_size = 0;
    for (int p = 0; employee_buffer[1][p] != '\0'; p ++) {
        name_size ++;
        if (!((employee_buffer[1][p] >= 'A' && employee_buffer[1][p] <= 'Z') || (employee_buffer[1][p] >= 'a' && employee_buffer[1][p] <= 'z'))) {
              for (size_t k = 0; k < i; k++) {
                free(employee_buffer[k]);
            }
            free(employee_buffer);
            
            return PARSE_EMPLOYEE_INVALID_VALUE;
        }
    }
    if (name_size < 1)  {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
         return PARSE_EMPLOYEE_INVALID_VALUE;
    }
    employee1.name = employee_buffer[1];

    for (int p = 0; employee_buffer[2][p] != '\0'; p ++) {
        surname_size ++;
        if (!((employee_buffer[2][p] >= 'A' && employee_buffer[2][p] <= 'Z') || (employee_buffer[2][p] >= 'a' && employee_buffer[2][p] <= 'z'))) {
              for (size_t k = 0; k < i; k++) {
                free(employee_buffer[k]);
            }
            free(employee_buffer);
            return PARSE_EMPLOYEE_INVALID_VALUE;
        }

        }
    if (surname_size < 1)  {
         for (size_t k = 0; k < i; k++) {
            free(employee_buffer[k]);
        }
        free(employee_buffer);
        return PARSE_EMPLOYEE_INVALID_VALUE;
    }

    employee1.surname = employee_buffer[2];

    free(employee_buffer[0]);
    free(employee_buffer[3]);
    free(employee_buffer);
    *result = employee1;

    return PARSE_EMPLOYEE_OK;
    //ебать!! абсолютная победа я эту функцию писала в сумме часа 2 
}

void free_employees(Employee *employees, size_t count) {
    if (employees == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        free(employees[i].name);
        free(employees[i].surname);
    }

    free(employees);
}

int compare_employees_ascending(const void *x1, const void *x2) {
    const Employee *a = x1;
    const Employee *b = x2;
    if (a->salary > b->salary) {
        return 1;
    } 
    if (a->salary < b->salary) {
        return -1;
    }

    int surname_compare = strcmp(a->surname, b->surname);
    if (surname_compare != 0) {
        return surname_compare;
    }

    int name_compare = strcmp(a->name, b->name);
    if (name_compare != 0) {
        return name_compare;
    }

    if (a->id > b->id) {
        return 1;
    } 
    if (a->id < b->id) {
        return -1;
    }

    return 0;


}

int compare_employees_descending(const void *x1, const void *x2) {
    const Employee *a = x1;
    const Employee *b = x2;
    if (a->salary > b->salary) {
        return -1;
    } 
    if (a->salary < b->salary) {
        return 1;
    }

    int surname_compare = strcmp(a->surname, b->surname);
    if (surname_compare != 0) {
        return (-1) * surname_compare;
    }

    int name_compare = strcmp(a->name, b->name);
    if (name_compare != 0) {
        return (-1) * name_compare;
    }

    if (a->id > b->id) {
        return -1;
    } 
    if (a->id < b->id) {
        return 1;
    }

    return 0;


}