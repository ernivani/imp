core_name = imp
core_install_location = /usr/local/bin
core_libraries_folder = /etc/imp-lib
core_libraries_name = libraries

SRC = main.c core/variable.c core/execution.c core/utility.c core/commands.c
INCLUDES = -I./core -I./runtime -I./compiler 

OBJ = $(SRC:.c=.o)

CC = gcc
CFLAGS = -Wall -g

.PHONY: all clean install

all: $(core_name)
	@echo "Compilation terminée avec succès."

$(core_name): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	
install:
	sudo rm -f $(core_install_location)/$(core_name)
	sudo cp $(core_name) $(core_install_location)
	sudo rm -rf $(core_libraries_folder)
	sudo mkdir -p $(core_libraries_folder)
	-sudo cp -r $(core_libraries_name)/* $(core_libraries_folder)/
	@echo "Installation terminée avec succès."

clean:
	rm -f $(core_name) $(OBJ)
	@echo "Nettoyage terminé avec succès."
