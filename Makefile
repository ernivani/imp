core_name = imp
core_install_location = /usr/local/bin
core_libraries_folder = /etc/imp-lib
core_libraries_name = libraries

# Source files with the core directory for the files moved to /core/
SRC = main.c core/variable.c core/execution.c core/utility.c core/commands.c
# Include the core directory
INCLUDES = -I./core -I./runtime -I./compiler 

# Object files
OBJ = $(SRC:.c=.o)

# Compiler
CC = gcc
# Compiler flags
CFLAGS = -Wall -g

.PHONY: all clean install

all: $(core_name)

$(core_name): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
install:
	# Remove existing binary if it exists
	sudo rm -f $(core_install_location)/$(core_name)
	# Copy the new binary
	sudo cp $(core_name) $(core_install_location)
	# Remove existing libraries and recreate the directory
	sudo rm -rf $(core_libraries_folder)
	sudo mkdir -p $(core_libraries_folder)
	# Copy new libraries
	sudo cp -r $(core_libraries_name)/* $(core_libraries_folder)/

clean:
	rm -f $(core_name) $(OBJ)
