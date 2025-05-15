runme: compile

compile: pseudo.c
	@gcc pseudo.c -o runme

implant: runme
	@./runme

sudo: runme
	@./runme SUDO

clean:
	@rm -rf *.o runme