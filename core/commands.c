#include "commands.h"
#include <stdio.h>


void verify_runtime() {
    printf("Imp Runtime Version: %s\n", VERSION);
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
