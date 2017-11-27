#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>

char *getprompt();
int execline(char *line);


//forms prompt for user input in shell session
char *getprompt() {
  char host[32];
  gethostname(host, 32);

  char cwd[64];
  getcwd(cwd, 64);

  char *prompt = malloc(96);
  sprintf(prompt, "CUSTOM_SHELL@%s:%s$ ", host, cwd);
  return prompt;
}

//executes a single command (possibly with multiple terms)
int execline(char *line) {
  char *terms[16];
  int n = 0;

  const char *delimiters = " \t\n";
  terms[0] = strtok(line, delimiters);
  while (terms[++n] = strtok(NULL, delimiters));
  
  //parses terms by terms[0], the file/command to be executed
  if (terms[0])
    if (strcmp(terms[0], "exit") == 0) {
      kill(0, SIGINT);
    } else if (strcmp(terms[0], "cd") == 0) {
      if (chdir(terms[1]) != 0) {
	char msg[32];
	sprintf(msg, "shell: cd: %s", terms[1]);
	perror(msg);
      }
    } else if (execvp(terms[0], terms) == -1)
      printf("No command '%s' found\n", terms[0]);
  return 0;
}
 
int main () {
  
  while (1) {
    printf("%s", getprompt());
    char line[16 * 32]; //going to store one line of text input
    fgets(line, 16 * 32, stdin);

    //splits line by semicolons and executes each subline
    char *subline = strtok(line, ";");
    int pid;
    do
      if ((pid = fork()) == 0)
	execline(subline);
      else wait(NULL);
    while (subline = strtok(NULL, ";"));
  }
}
