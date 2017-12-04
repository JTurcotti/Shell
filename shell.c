#include "header.h"

// copy of stdin and stdout to copy back to
const int stdin_temp;
const int stdout_temp;

// creates the prompt for user
char *getprompt() {
  char host[64];
  gethostname(host, 32);

  // //check if ~/ can be inserted
  // char *temp, *temp2;
  // strcpy(temp, host);
  // do
  //   temp2 = strsep(&temp, "/");
  // while(strcmp(temp2, "home"));
  // strsep(&temp, "/");


  char cwd[64];
  getcwd(cwd, 64);

  char *prompt = malloc(96);
  // if(temp)
    sprintf(prompt, "CUSTOM_SHELL@%s:%s$ ", host, cwd);
  // else sprintf(prompt, "CUSTOM_SHELL@~/%s:%s$ ", temp, cwd);
  return prompt;
}

// check if line needs piping
void exec_piped_line(char *line) {
  char *pipechr;
  if (!(pipechr = strchr(line, '|')))
    exec_line(line); //line contains no pipes
  else {
    // get the commands left and right of pipe
    char *left = malloc(32);
    char *right = malloc (32);
    left = strtok(line, "|");
    right = strtok(NULL, "|");

    // get the output of left into stdin
    FILE *in;
    extern FILE *popen();
    char buff[512];
    buff[0] = 0;
    in = popen(left, "r");
    int fd = fileno(in);
    dup2(fd, 0);
    exec_line(right);
    close(fd);
    free(left);
    free(right);
  }
}

// executes a single line (possibly with multiple terms, but no semicolons or )
void exec_line(char *line) {
  // terms is the set up for execlp
  char **terms = calloc(8, sizeof(char*));
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
    } else if (redir_out_active == 1) {
      redir_out(term);
      redir_out_active = 0;
    } else if (redir_out_active == 2) {
      redir_out2(term);
      redir_out_active = 0;
    } else if (strcmp(term, "<") == 0)
      redir_in_active = 1;
    // else if (strcmp(term, "|") == 0)
    else if (strcmp(term, ">") == 0)
      redir_out_active = 1;
    else if (strcmp(term, ">>") == 0)
      redir_out_active = 2;
    else
      terms[n++] = term;
  }
  // null delimiting
  terms[n] = NULL;
  exec_cmd(terms, n);
  redir_reset();
  free(terms);
}

// executes a single command without piping or redirection, parses by terms[0]
void exec_cmd(char **terms, int len) {
  // check for special cases if there is a command
  if (terms[0])
    if (strcmp(terms[0], "exit") == 0) {
      // kill all related process
      kill(0, SIGINT);
    } else if (strcmp(terms[0], "cd") == 0) {
      if (chdir(terms[1]) != 0) {
      	char msg[32];
      	sprintf(msg, "shell: cd: %s", terms[1]);
      	perror(msg);
      }
    } else if (execvp(terms[0], terms) == -1)
      printf("%s: command not found\n", terms[0]);
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

// redirect stdout and appends to the file
void redir_out2(char *filename) {
  printf("sadfasdfasdfsafsdfsFD\n");
  int file = fileno(fopen(filename, "a"));
  dup2(file, 1);
}

// resets stdin and stdout
void redir_reset() {
  dup2(stdin_temp, 0);
  dup2(stdout_temp, 1);
}


int main() {
  // copy stdin and stdout
  const int stdin_temp = dup(0);
  const int stdout_temp = dup(1);

  // this is always running
  while (1) {
    // print the prompt
    char * prompt = getprompt();
    printf("%s", prompt);
    free(prompt);

    // get user input (big size for big inputs)
    char line[16 * 32];
    fgets(line, sizeof(line), stdin);

    // get the first command (commands are split by ; in line)
    char *subline = strtok(line, ";");
    // have a child run the command while the next token isn't null
    do
      if (fork() == 0)
        exec_piped_line(subline);
      else wait(NULL);
    while (subline = strtok(NULL, ";"));
  }
}

// debugging function
void print_arr(char **arr, int len) {
  printf("[");
  int n;
  for (n = 0; n < len; n++)
    printf("%s, ", arr[n]);
  printf("%d]\n", len);
}
