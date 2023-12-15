#include "variable.h"
#include "utility.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


char* trim_whitespace(char* str) {
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

int evaluate_condition(const char* condition) {
    char condition_copy[strlen(condition) + 1];
    strcpy(condition_copy, condition);

    char* left = strtok(condition_copy, " ");    char* operator = strtok(NULL, " ");
    char* right = strtok(NULL, " ");
    if (left[0] == '(') {
        left++;
        left[strlen(left) ] = '\0';
    }
    if (right[strlen(right) - 1] == ')') {
        right[strlen(right) - 1] = '\0';
    }



    if (left && operator && right) {
        int left_value, right_value;
        Variable* var = get_variable(left);
        left_value = var ? var->i_value : atoi(left);

        right_value = atoi(right);

        if (strcmp(operator, "==") == 0) {
            return left_value == right_value;
        } else if (strcmp(operator, "!=") == 0) {
            return left_value != right_value;
        } else if (strcmp(operator, ">") == 0) {
            return left_value > right_value;
        } else if (strcmp(operator, "<") == 0) {
            return left_value < right_value;
        } else if (strcmp(operator, ">=") == 0) {
            return left_value >= right_value;
        } else if (strcmp(operator, "<=") == 0) {
            return left_value <= right_value;
        }
    }
    return 0; 
}

char* process_formatted_string(const char* str, const char* end) {
    size_t estimatedSize = strlen(str) + 1;
    char* buffer = malloc(estimatedSize); // Initial buffer allocation
    if (!buffer) {
        perror("Memory allocation failed");
        return NULL;
    }

    char* p = buffer;
    while (*str && str < end) { // Check against end pointer
        if (*str == '{') {
            str++; // Skip the '{'
            char varName[100]; // Assuming variable names won't exceed 99 characters
            int i = 0;
            while (*str != '}' && *str != '\0' && i < 99) {
                varName[i++] = *str++;
            }
            varName[i] = '\0'; // Null-terminate the variable name

            if (*str == '}') {
                str++; // Skip the '}'
                Variable* var = get_variable(varName);
                if (var) {
                    char varValue[256]; // Temporary buffer for variable value
                    size_t len = 0;
                    if (var->type == INT_TYPE) {
                        len = sprintf(varValue, "%d", var->i_value);
                    } else if (var->type == FLOAT_TYPE) {
                        len = sprintf(varValue, "%g", var->f_value);
                    } else if (var->type == STRING_TYPE) {
                        len = sprintf(varValue, "%s", var->s_value);
                    }
                    if (len + (p - buffer) >= estimatedSize) {
                        estimatedSize += len + 256; // Increase buffer size
                        size_t offset = p - buffer;
                        buffer = realloc(buffer, estimatedSize);
                        if (!buffer) {
                            perror("Memory reallocation failed");
                            return NULL;
                        }
                        p = buffer + offset; // Update pointer after reallocation
                    }
                    strcpy(p, varValue);
                    p += len;
                }
            }
        } else {
            if (p - buffer >= estimatedSize - 1) {
                estimatedSize += 256; // Increase buffer size
                size_t offset = p - buffer;
                buffer = realloc(buffer, estimatedSize);
                if (!buffer) {
                    perror("Memory reallocation failed");
                    return NULL;
                }
                p = buffer + offset; // Update pointer after reallocation
            }
            *p++ = *str++; // Copy the character as is
        }
    }
    *p = '\0'; // Null-terminate the buffer
    return buffer;
}
