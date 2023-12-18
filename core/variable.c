#include "variable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_VAR_COUNT 100
#define MAX_VAR_NAME_LENGTH 50

Variable variables[MAX_VAR_COUNT];
int var_count = 0;

Variable* get_variable(const char* name) {
    for (int i = 0; i < var_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return &variables[i];
        }
    }
    return NULL;
}

void create_or_update_variable(const char* name, const char* value, int type) {
    Variable* var = get_variable(name);

    if (var == NULL) {
        if (var_count >= MAX_VAR_COUNT) {
            fprintf(stderr, "Error: Maximum number of variables reached\n");
            return;
        }

        if (var_count >= MAX_VAR_COUNT) {
            fprintf(stderr, "Error: Maximum number of variables reached\n");
            return;
        }

        var = &variables[var_count++];
        strncpy(var->name, name, MAX_VAR_NAME_LENGTH - 1);
        var->name[MAX_VAR_NAME_LENGTH - 1] = '\0';
    } else {
        if (var->type == STRING_TYPE) {
            free(var->s_value);
            var->s_value = NULL;
        }
    }

    if (type == INT_TYPE) {
        var->type = INT_TYPE;
        var->i_value = atoi(value);
    } else if (type == FLOAT_TYPE) {
        var->type = FLOAT_TYPE;
        var->f_value = atof(value);
    } else if (type == STRING_TYPE) {
        if (var->type == STRING_TYPE) {
            free(var->s_value);
        }
        var->type = STRING_TYPE;
        var->s_value = malloc(strlen(value) + 1);
        if (var->s_value == NULL) {
            fprintf(stderr, "Error: Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        strcpy(var->s_value, value);
    }
}
