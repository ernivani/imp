#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define VERSION "0.1"
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
            printf("Error: Maximum number of variables reached\n");
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
        strcpy(var->s_value, value);
    }
}


void verify_runtime();
void version();
void help();
void run_file(const char* filename);
void add_library(const char* filepath);
void update_enviroment();
void bundle_directory(const char* directory);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Error: No command provided \nTry 'imp --help' for more information.\n");
        return 1;
    }

    if (strcmp(argv[1], "--version") == 0 || strcmp(argv[1], "-v") == 0) {
        version();
    }
    else if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        help();
    }
    else if (strcmp(argv[1], "--verify") == 0) {
        verify_runtime();
    } else if ((strcmp(argv[1], "--run") == 0 || strcmp(argv[1], "-r") == 0) && argc == 3 && strstr(argv[2], ".imp") != NULL) {
        run_file(argv[2]);
    } else if (strcmp(argv[1], "imp-package") == 0 && argc == 3) {
        if (strcmp(argv[2], "--add-library") == 0) {
            add_library(argv[3]);
        } else if (strcmp(argv[2], "--update") == 0) {
            update_enviroment();
        }
    } else if (strcmp(argv[1], "--bundle") == 0 && argc == 3) {
        bundle_directory(argv[2]);
    } else if (strstr(argv[1], ".imp") != NULL) {
        run_file(argv[1]);
    }

    else {
        printf("Error: Invalid command\n");
        return 1;
    }

    return 0;
     
}

void verify_runtime() {
    printf("Imp Runtime Version: %s\n", VERSION);
}

char* trim_whitespace(char* str) {
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)
        return str;

    char* end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    end[1] = '\0';

    return str;
}

void run_file(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char* trimmed_line = trim_whitespace(line);

        // printf("Ligne traitée : '%s'\n", trimmed_line); // Pour le débogage

        if (strlen(trimmed_line) == 0) {
            continue;
        }

        // if line starts with a comment : //
        if (trimmed_line[0] == '/' && trimmed_line[1] == '/') {
            continue;
        }

        if (trimmed_line[strlen(trimmed_line) - 1] == '{' || trimmed_line[strlen(trimmed_line) - 1] == '}') {
            // TODO: Logique pour gérer les blocs de code
            continue;
        }

        if (trimmed_line[strlen(trimmed_line) - 1] != ';') {
            printf("Error: Line does not end with ';'\n");
            continue;
        }

        trimmed_line[strlen(trimmed_line) - 1] = '\0';


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
                printf("%s", start);
            }
        }
        else if (strncmp(trimmed_line, "println(\"", 9) == 0) {
            char* start = trimmed_line + 9;
            char* end = strchr(start, '\"');
            if (end) {
                *end = '\0';
                printf("%s\n", start);
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
    }

    free(line);
    fclose(file);
}


void add_library(const char* filepath) {
    printf("Adding library: %s\n", filepath);
}

void update_enviroment() {
    printf("Updating enviroment\n");
}

void bundle_directory(const char* directory) {
    printf("Bundling directory: %s\n", directory);
}

void version() {
    printf("Imp Runtime Version: %s\n", VERSION);
}

void help() {
    printf("Usage: imp [command]\n");
    printf("Commands:\n");
    printf("    --version, -v      Display the version of the Imp runtime\n");
    printf("    --help, -h         Display this message\n");
    printf("    --verify           Verify the Imp runtime\n");
    printf("    --run, -r [file]   Run the specified Imp file\n");
    printf("    imp-package [command]\n");
    printf("        --add-library [path]   Add a library to the Imp runtime\n");
    printf("        --update               Update the Imp runtime\n");
    printf("    --bundle [directory] Bundle a directory into an Imp package\n");
}
