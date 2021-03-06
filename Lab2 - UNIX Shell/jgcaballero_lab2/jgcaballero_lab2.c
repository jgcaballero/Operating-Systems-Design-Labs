/* Jose Caballero
   EE 4374 lab #2 Command Executer
   03/01/2016*/

 /*Please see jgcaballero_exec.c for an explanation of lab assignment #2. */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include "jgcaballero_argtok.h"
#include "jgcaballero_exec.h"

int main()
{
    int i;
	int inputSize;
	char buff[128];		// Character buffer
	char **args;

    write(1, "Welcome to the Student Shell (type 'x' to exit)\n", 48);
	while(1)
	{
		// Request a line of input
		write(1, "\nshell -> ", 10);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize-1] = '\0';

		if(buff[0] == '\0')
			continue;

		if((buff[0] == 'x') && (buff[1] == '\0'))
		{
			exit(0);
		}

		// make the argument vector
		args = argtok(buff);

		// execute the command
		executeCmd(args);

	}

	return 0;

}