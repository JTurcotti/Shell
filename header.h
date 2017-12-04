#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

// comments for functions are in shell.c and header descriptions are in README.md
char *getprompt();
void exec_piped_line(char * line);
void exec_line(char *line);
void exec_cmd(char **terms, int len);
void redir_in(char *filename);
void redir_out(char *filename);
void redir_out2(char *filename);
void redir_reset();
void print_arr(char **arr, int len);
