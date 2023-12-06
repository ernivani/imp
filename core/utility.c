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
    // Handle both variable-based and literal comparisons
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

        right_value = atoi(right); // Assuming right side is always a literal

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
