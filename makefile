runme: compile
server: server.o networking.o utils.o user_console.o control_server.o init_struct.o
	@gcc -o server server.o networking.o utils.o user_console.o control_server.o init_struct.o

server.o: networking.h server.c utils.h init_struct.h
	@gcc -c server.c

control_server.o: networking.h control_server.c utils.h
	@gcc -c control_server.c

networking.o: networking.c
	@gcc -c networking.c

utils.o: utils.c
	@gcc -c utils.c

user_shell: user_shell.o user_console.o networking.o utils.o
	@gcc -o user_shell user_shell.o user_console.o networking.o utils.o 

user_console.o: user_console.c networking.h utils.h user_console.h
	@gcc -c user_console.c

user_shell.o: user_console.h user_shell.c
	@gcc -c user_shell.c

user: 
	@./user_shell $(ARGS)

compile: pseudo.c init_struct.o
	@gcc pseudo.c init_struct.o -o runme

init_struct.o: init_struct.c
	@gcc -c init_struct.c

implant: runme
	@./runme

sudo: runme
	@./runme SUDO BLANK $(ARGS)

clean:
	@rm -rf *.o runme
