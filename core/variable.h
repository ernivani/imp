#ifndef VARIABLE_H
#define VARIABLE_H

#define MAX_VAR_NAME_LENGTH 256
#define MAX_VAR_COUNT 256

typedef struct {
    char name[MAX_VAR_NAME_LENGTH];
    union {
        int i_value;
        float f_value;
        char* s_value;
    };
    enum {
        INT_TYPE,
        STRING_TYPE,
        FLOAT_TYPE,
    } type;
} Variable;

Variable* get_variable(const char* name);
void create_or_update_variable(const char* name, const char* value, int type);

#endif
