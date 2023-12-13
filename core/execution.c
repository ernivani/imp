#include "execution.h"
#include "variable.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

void run_file(const char* filename, int debug_mode) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_number = 0;
    int execute_block = 1; 

    while ((read = getline(&line, &len, file)) != -1) {
        char* trimmed_line = trim_whitespace(line);
        line_number++;

        if (debug_mode) {
            printf("Ligne traitée : '%s'\n", trimmed_line);
        }

        if (strlen(trimmed_line) == 0) {
            continue;
        }

        if (trimmed_line[0] == '/' && trimmed_line[1] == '/') {
            continue;
        }

        if (trimmed_line[strlen(trimmed_line) - 1] != ';' && trimmed_line[strlen(trimmed_line) - 1] != '}' && trimmed_line[strlen(trimmed_line) - 1] != '{') {
            printf("Error: Line %d does not end with ';'\n", line_number);
            continue;
        }

        if (trimmed_line[strlen(trimmed_line) - 1] == ';') {
            trimmed_line[strlen(trimmed_line) - 1] = '\0';
        }


        if (strncmp(trimmed_line, "int ", 4) == 0) {
            char* var_name = strtok(trimmed_line + 4, " =");
            char* var_value = strtok(NULL, " =;");
            var_value = trim_whitespace(var_value);
            create_or_update_variable(var_name, var_value, INT_TYPE);
        }
        else if (strncmp(trimmed_line, "float ", 6) == 0) {
            char* var_name = strtok(trimmed_line + 6, " =");
            char* var_value = strtok(NULL, "=;");
            var_value = trim_whitespace(var_value);
            create_or_update_variable(var_name, var_value, FLOAT_TYPE);
        }
        else if (strncmp(trimmed_line, "string ", 7) == 0) {
            char* var_name = strtok(trimmed_line + 7, " =");
            char* var_value = strtok(NULL, "\"");
            var_value = strtok(NULL, "\"");  
            if (var_value) {
                create_or_update_variable(var_name, var_value, STRING_TYPE);
            } else {
                printf("Error: Invalid string syntax\n");
            }
        }
        else if (strncmp(trimmed_line, "print(\"", 7) == 0) {
            char* start = trimmed_line + 7;
            char* end = strchr(start, '\"');
            if (end) {
                *end = '\0';
                printf("%s", process_formatted_string(start, end));
            }
        }
        else if (strncmp(trimmed_line, "println(\"", 9) == 0) {
            char* start = trimmed_line + 9;
            char* end = strchr(start, '\"');
            if (end) {
                *end = '\0';
                printf("%s\n", process_formatted_string(start, end));
            }
        }
        else if (strstr(trimmed_line, "print(") == trimmed_line) {
            char* var_name = strtok(trimmed_line + 6, ")");
            Variable* var = get_variable(var_name);
            if (var != NULL) {
                if (var->type == INT_TYPE) {
                    printf("%d", var->i_value);
                } else if (var->type == FLOAT_TYPE) {
                    printf("%g\n", var->f_value);
                } else if (var->type == STRING_TYPE) {
                    printf("%s", var->s_value);
                }
            }
        }
        else if (strstr(trimmed_line, "println(") == trimmed_line) {
            char* var_name = strtok(trimmed_line + 8, ")");
            Variable* var = get_variable(var_name);
            if (var != NULL) {
                if (var->type == INT_TYPE) {
                    printf("%d\n", var->i_value);
                } else if (var->type == FLOAT_TYPE) {
                    printf("%g\n", var->f_value); 
                } else if (var->type == STRING_TYPE) {
                    printf("%s\n", var->s_value);
                }
            }
        }
        else if (strncmp(trimmed_line, "if", 2) == 0) {
            char* condition = trimmed_line + 2; 
            int condition_true = evaluate_condition(condition);
            execute_block = condition_true;
            int number_of_lines_to_skip = 0;
            if (trimmed_line[strlen(trimmed_line) - 1] != '{') {
                printf("Error: Invalid if condition at line %d\n", line_number);
                execute_block = 0;
            }
            else if (!condition_true) {
                execute_block = 0;
                number_of_lines_to_skip = 1;
                while (number_of_lines_to_skip > 0 && (read = getline(&line, &len, file)) != -1) {
                    char* trimmed_line = trim_whitespace(line);
                    if (trimmed_line[0] == '{') {
                        number_of_lines_to_skip++;
                    }
                    else if (trimmed_line[0] == '}') {
                        number_of_lines_to_skip--;
                    }
                }
            }
        }
        
        else if (trimmed_line[0] == '}') {
            execute_block = 1;
        }
        else if (execute_block) {
            execute_block = 0;
        }

        else {
            if (strchr(trimmed_line, '=')) {
                char* var_name = strtok(trimmed_line, " =");
                char* new_value = strtok(NULL, ";");
                if (var_name && new_value) {
                    new_value = trim_whitespace(new_value);
                    Variable* var = get_variable(var_name);
                    if (var) {
                        create_or_update_variable(var_name, new_value, var->type);
                    } else {
                        printf("Error: Undefined variable '%s' at line %d\n", var_name, line_number);
                    }
                } else {
                    printf("Error: Invalid syntax at line %d\n", line_number);
                }
            } else {
                printf("Error: Invalid syntax or unrecognized command at line %d\n", line_number);
            }
        }
    }

    free(line);
    fclose(file);
}
