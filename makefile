runme: compile
server: server.o networking.o utils.o user_console.o control_server.o
	gcc -o server server.o networking.o utils.o user_console.o control_server.o

server.o: networking.h server.c utils.h
	gcc -c server.c

user_console.o: networking.h user_console.c utils.h
		gcc -c user_console.c

control_server.o: networking.h control_server.c utils.h
		gcc -c control_server.c

networking.o: networking.c
	gcc -c networking.c

utils.o:utils.c
	gcc -c utils.c

compile: pseudo.c
	@gcc pseudo.c -o runme

implant: runme
	@./runme

sudo: runme
	@./runme SUDO BLANK $(ARGS)

clean:
	@rm -rf *.o runme
