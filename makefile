all:
	gcc -o custom_shell shell.c

run: all
	./custom_shell
