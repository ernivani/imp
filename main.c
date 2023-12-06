#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#include "commands.h"
#include "execution.h"


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
        run_file(argv[2], 0);
    } else if ((strcmp(argv[1], "--debug") == 0 || strcmp(argv[1], "-d") == 0) && argc == 3 && strstr(argv[2], ".imp") != NULL) {
        run_file(argv[2], 1);
    } else if (strcmp(argv[1], "imp-package") == 0 && argc == 3) {
        if (strcmp(argv[2], "--add-library") == 0) {
            add_library(argv[3]);
        } else if (strcmp(argv[2], "--update") == 0) {
            update_enviroment();
        }
    } else if (strcmp(argv[1], "--bundle") == 0 && argc == 3) {
        bundle_directory(argv[2]);
    } else if (strstr(argv[1], ".imp") != NULL) {
        run_file(argv[1], 0);
    } 

    else {
        printf("Error: Invalid command\n");
        return 1;
    }

    return 0;
     
}
