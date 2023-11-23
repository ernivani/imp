core_name = imp
core_install_location = /usr/local/bin
core_libraries_folder = /etc/imp-lib
core_libraries_name = libraries



make:
	gcc -o $(core_name)  main.c core/run.c -I./core -I./runtime -I./compiler 

install:
	cp $(core_name) $(core_install_location)
	sudo rm -rf $(core_libraries_folder)
	cp -r $(core_libraries_name) $(core_libraries_folder)