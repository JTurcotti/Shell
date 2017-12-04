x# Shell

Joshua Turcotti and Charles weng
Systems Period 4 With Mr. Dryland Weaver
Project 1: Shelling Out Commands

This is a bash-like shell for DW's systems class

## Currently Implemented Features:
* reading and execing lines of input, including exit and cd
* fully accurate user prompt (PS1)
* multiple commands per line with semicolon delimiting
* redirection to/from stdout/in (including both at once)
  - redirect with <, >, and >>
* simple piping supported

## Attempted Feaetures:
* tried to insert ~/ in place of home/<user>/

## Bugs:
* cding into a different directory and then holding down enter will sometimes(rarely) show the prompt for initial directory and even execute commands as if in the initial directory. (I think it's just my computer's lag)
* doing ctrl + d breaks the shell and it spams the terminal with the prompt
* tab is counted as a viable delimiter, but because it appears in the terminal as spaces, it messes up the command input
* not sure why but I can't seem to redirect another file with commands into the shell and have it recognize it. probably just missing something.


## Files & Headers:
shell.c
  this is contains all the functions

  /*====== char *getprompt() ===============
  inputs: none
  returns: promt for user

  this function gets all the information for the prompt and formats it into a malloced String
  ================*/


  /*======= void exec_piped_line(char * line) ===============
  inputs: char *line
  returns: nothing

  this function takes in a single line that may contain pipes and redirection, but no semicolons.

  forwards line to after processing any pipes (up to one)
  ================*/


  /*======= void exec_line(char *line) ===============
  inputs: char *line
  returns: nothing

  this function takes in a single that may contain redirection, but no semicolons or pipes

  forwards a processed line into exec_cmd
  ================*/


  /*======= void exec_cmd(char **terms, int len) ===============
  inputs: char ** terms
          int len
  returns: nothing

  checks if the command is special and executes it accordingly (exit and cd are different)
  ================*/


  /*======= void redir_in(char *filename) ===============
  inputs: char *filename
  returns: nothing

  preps the filetable for a stdin redirection
  ================*/


  /*======= void redir_out(char *filename) ===============
  inputs: char *filename
  returns: nothing

  preps the filetable for a stdout redirection (O_TRUNC)
  ================*/


  /*======= void redir_out2(char *filename) ===============
  inputs: char *filename
  returns: nothing

  preps the filetable for a stdout redirection (O_APPEND)
  ================*/


  /*======= void redir_reset()===============
  inputs: none
  returns: nothing

  resets the filetable for future commands
  ================*/


  /*======= void print_arr(char **arr, int len) ===============
  inputs: char ** arr
  returns: int len

  debug function to see what's inside an array of pointers
  ================*/
