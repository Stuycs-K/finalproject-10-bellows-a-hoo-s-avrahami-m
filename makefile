runme: compile
server: server.o networking.o utils.o
	gcc -o server server.o networking.o utils.o

server.o: networking.h server.c utils.h
	gcc -c server.c

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
