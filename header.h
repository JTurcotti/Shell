#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

char *getprompt();
int exec_line(char *line);
int exec_cmd(char **terms, int len);
void redir_in(char *filename);
void redir_out(char *filename);
void redir_reset();
void print_arr(char **arr, int len);

