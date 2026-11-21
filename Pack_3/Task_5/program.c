#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct Student {
    unsigned int id;
    char *name;
    char *surname;
    char *group;
    unsigned char *exam_grade; 
} Student;

typedef enum ReadLineStatus{
    READ_LINE_OK,
    READ_LINE_EOF,
    READ_LINE_NULL,
    READ_LINE_MEMORY_ERROR,
    READ_LINE_FILE_ERROR
} ReadLineStatus;

typedef enum ParseStudentStatus {
    PARSE_STUDENT_OK,
    PARSE_STUDENT_NULL,
    PARSE_STUDENT_INVALID_VALUE,
} ParseStudentStatus;

typedef enum ParseUnsignedIntStatus{
    PARSE_UNSIGNED_INT_OK,
    PARSE_UNSIGNED_INT_NULL,
    PARSE_UNSIGNED_INT_INVALID_VALUE,
    PARSE_UNSIGNED_INT_OUT_OF_RANGE
} ParseUnsignedIntStatus;

ReadLineStatus read_line(FILE *file, char **line);
ParseStudentStatus parse_student(const char *line, Student *result);
ParseUnsignedIntStatus parse_unsigned_int(const char *num, unsigned int *result);


void free_students(Student *students, size_t count);

int main(int argc, char *argv[]){
    if(argc != 3) {
        printf("REFUSED - Invalid counts of args!\n");
        return 1;
    }

    if (strcmp(argv[1], argv[3]) == 0) {
        printf("REFUSED — the input and output file names are the same. This shouldn t happen =(\n");
        return 1;       
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        printf("REFUSED - Error opening the file with input data\n");
        return 1;
    }

    Student *students = NULL;
    size_t size_students = 0;

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
            case READ_LINE_MEMORY_ERROR:
            case READ_LINE_FILE_ERROR:
                fclose(input_file);
                free(file_line);
                free_students(students, size_students);
                printf("REFUSED - Error reading the file - READ_LINE_FILE_ERROR\n");
                return 1;
        }

        if (file_line_status == READ_LINE_EOF) break;

        Student student_creep = {
             .id = 0,
             .name = NULL,
             .surname = NULL,
             .group = NULL,
             .exam_grade = calloc(5, sizeof(unsigned char))
        };

        ParseStudentStatus student_status = parse_employee(file_line, &student_creep);

        switch (student_status) {
            case PARSE_STUDENT_OK:
                break;
            case PARSE_STUDENT_NULL:
            case PARSE_STUDENT_INVALID_VALUE:
                fclose(input_file);
                free(file_line);
                free_students(students, size_students);
                printf("REFUSED - PARSE STUDENT ERROR");
                return 1;
        }

        Student *temp = realloc(students, (size_students + 1) * sizeof(*students));
        if (temp == NULL) {
            fclose(input_file);
            free(file_line);
            free_students(students, size_students);
            printf("REFUSED - Memory error");  
            return 1;
        }

        students = temp;

        students[size_students] = student_creep;
        size_students ++;

        free(file_line);

    } while (file_line_status == READ_LINE_OK);


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

void free_students(Student *students, size_t count) {
    if (students == NULL) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        free(students[i].name);
        free(students[i].surname);
        free(students[i].group);
    }

    free(students);
}


ParseStudentStatus parse_student(const char *line, Student *result) {

    if (line == NULL || result == NULL) {
        return PARSE_STUDENT_NULL;
    }

    char *student_buffer[9];
    
    char *mini_student_buffer = strtok(line, " ");
    int count = 0;

    while (mini_student_buffer != NULL && count < 9) {
        student_buffer[count] = mini_student_buffer;
        count ++;
        mini_student_buffer = strtok(NULL, " ");
    }

    if (count != 9) {
        return PARSE_STUDENT_INVALID_VALUE;
    }

    Student student1;

    //id
    unsigned int student_id;
    ParseUnsignedIntStatus student_id_status = parse_unsigned_int(student_buffer[0], &student_id);
    if(student_id_status != PARSE_UNSIGNED_INT_OK) {
        return PARSE_STUDENT_INVALID_VALUE;
    }
    student1.id = (unsigned int)student_id;

    //name and surname

    size_t name_size = 0;
    size_t surname_size = 0;
    for (int p = 0; student_buffer[1][p] != '\0'; p ++) {
        name_size ++;
        if (!((student_buffer[1][p] >= 'A' && student_buffer[1][p] <= 'Z') || (student_buffer[1][p] >= 'a' && student_buffer[1][p] <= 'z'))) {
            return  PARSE_STUDENT_INVALID_VALUE;
    }
    if (name_size < 1)  {
         return PARSE_STUDENT_INVALID_VALUE;
    }
    student1.name = student_buffer[1];


    for (int p = 0; student_buffer[2][p] != '\0'; p ++) {
        surname_size ++;
        if (!((student_buffer[2][p] >= 'A' && student_buffer[2][p] <= 'Z') || (student_buffer[2][p] >= 'a' && student_buffer[2][p] <= 'z'))) {
            return PARSE_STUDENT_INVALID_VALUE;
        }
    }
    if (surname_size < 1)  {
        return PARSE_STUDENT_INVALID_VALUE;
    }
    student1.surname = student_buffer[2];


    //group - > M8O-***БВ-25
    if (strlen(student_buffer[3]) != 12 || 
    student_buffer[3][0] != 'М' || 
    student_buffer[3][1] != '8' ||  
    student_buffer[3][2] != 'О' || 
    student_buffer[3][3] != '-' || 
    (student_buffer[3][4] != '2') || 
    (student_buffer[3][5] != '0' && student_buffer[3][4] != '1') || 
    (student_buffer[3][6] < '0' || student_buffer[3][4] > '9') ||
    student_buffer[3][8] != 'В' || 
    student_buffer[3][9] != '-' || 
    student_buffer[3][10] != '2' || 
    student_buffer[3][11] != '5') {
        return PARSE_STUDENT_INVALID_VALUE;
    }
    student1.group = student_buffer[3];


    //оценки

    for (int i = 0; i < 5; i ++) {
        unsigned int pipka;
        ParseUnsignedIntStatus pipka_status = parse_unsigned_int(student_buffer[4 + i], &pipka);
        
        if (pipka > 5 || pipka < 2) {
            return PARSE_STUDENT_INVALID_VALUE;
        }
        student1.exam_grade[i] = (unsigned int)pipka;
    }

    *result = student1;

    return PARSE_STUDENT_OK;
    }
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

void free_students(Student *students, size_t count) {
    if (students == NULL || count == 0) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        free(students[i].exam_grade);
    }

    free(students);
}