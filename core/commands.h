#ifndef COMMANDS_H
#define COMMANDS_H

#define VERSION "0.1"

void verify_runtime();
void version();
void help();
void add_library(const char* filepath);
void update_enviroment();
void bundle_directory(const char* directory);

#endif
