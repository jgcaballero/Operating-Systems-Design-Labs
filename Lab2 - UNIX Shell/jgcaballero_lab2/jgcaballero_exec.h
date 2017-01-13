/* Jose Caballero
   EE 4374 lab #2 Command Executer
   03/01/2016*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int executeCmd(char **args);
int regular_unix_commands(char **args);
int ls1 (char **args);
int ls1_l(char **args);
int ls1_a(char **args);
int help1(char **args);
int top(char **args);
int top_kek(char **args);
int cat1(char **args);
int mkdir1(char **args);
int wc1(char **args);
int sleep_method();