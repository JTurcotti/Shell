#include "header.h"

// copy of stdin and stdout to copy back to
int stdin_temp;
int stdout_temp;

// creates the prompt for user
char *getprompt() {
  char host[32];
  gethostname(host, 32);

  char cwd[64];
  getcwd(cwd, 64);

  char *prompt = malloc(96);
  sprintf(prompt, "CUSTOM_SHELL@%s:%s$ ", host, cwd);
  return prompt;
}

int exec_piped_line(char *line) {
  char *pipechr;
  if (!(pipechr = strchr(line, '|')))
    exec_line(line); //line contains no pipes
  else {
    char *left = malloc(32);
    char *right = malloc (32);
    strcpy(left, strtok(line, "|"));
    strcpy(right, strtok(line, "|"));

    int piper[2];
    pipe(piper);

    if (fork() == 0) {
      close(piper[0]);
      dup2(piper[0], 1);
      exec_line(line);
    } else {
      close(piper[1]);
      dup2(piper[1], 0);
      exec_line(line);
    }
  }
}

//executes a single line (possibly with multiple terms, but no semicolons)
int exec_line(char *line) {
  // terms is the set up for execlp
  char **terms;
  const char *delimiters = " \t\n";

  terms[0] = strtok(line, delimiters);

  // checks if redirection is needed
  int redir_in_active = 0;
  int redir_out_active = 0;
  char * term;
  int n = 1;
  // parses through the rest of the line (if )
  while (term = strtok(NULL, delimiters)) {
    // check if redirection is required else it's a valid flag
    if (redir_in_active) {
      redir_in(term);
      redir_in_active = 0;
    } else if (redir_out_active) {
      redir_out(term);
      redir_out_active = 0;
    } else if (strcmp(term, "<") == 0)
      redir_in_active = 1;
    else if (strcmp(term, ">") == 0)
      redir_out_active = 1;
    else
      terms[n++] = term;
  }
  // null delimiting
  terms[n] = NULL;
  exec_cmd(terms, n);
  //cleanup if execvp fails
  redir_reset();
}

// executes a single command without piping or redirection, parses by terms[0]
int exec_cmd(char **terms, int len) {
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
      printf("%s: command not found\n", terms[0]);

  return 0;
}

// redirect stdin to the file
void redir_in(char *filename) {
  int file = fileno(fopen(filename, "r"));
  dup2(file, 0);
}

// redirect stdout to the file
void redir_out(char *filename) {
  int file = fileno(fopen(filename, "w"));
  dup2(file, 1);
}

void redir_reset() {
  dup2(stdin_temp, 0);
  dup2(stdout_temp, 1);
}


int main() {
  char * prompt = getprompt();
  // this is always running
  while (1) {
    // print the prompt
    printf("%s", prompt);

    // get user input (big size for big inputs)
    char line[16 * 32];
    fgets(line, sizeof(line), stdin);

    // get the first command (commands are split by ; in line)
    char *subline = strtok(line, ";");
    int pid;
    // have a child run the command while the next token isn't null
    do
      if ((pid = fork()) == 0)
	exec_piped_line(subline);
      else wait(NULL);
    while (subline = strtok(NULL, ";"));
  }
}

void print_arr(char **arr, int len) {
  printf("[");
  int n;
  for (n = 0; n < len; n++)
    printf("%s, ", arr[n]);
  printf("%d]\n", len);
}
