/* Jose Caballero
   EE 4374 lab #2 Command Executer
   03/01/2016*/

/*WARNING, very long code! Proceed with caution! */

/*The purpose of this lab was to implement our own shell. For this assignment, I spent quite A LOT figuring out how I would implement the shell commands, like I actually thought we had to 
  implement 'ls' ourselves. This is why I made such a mess doing my own arrays with functions and such. You can ignore those parts if you wish, though I would say I did learn how I would 
  implement my own shell commands. Anyways, the real objective of the lab was on learning how I would implement my own str_tok that we did for lab1 and use that to manipulate the arguments
  the user would give our shell, and execute the commands. I learned there are various exec() functions, and each of them have different purposes. Later on this lab I explain why I use execlp
  vs execvp. Because at first I started implementing my own shell methods, my executeCmd method actually handles MY own shell implementations, and if the user calls a command not implemented 
  by myself, I redirect it to the regular_unix_commands method, which will call on the, you guessed it, regular unix commands. Using then the given arguments after being tokenized by our own
  str_tok, we pass the arguments to the executeCmd method, which will handle the arguments and depending on the number of arguments passed, it will call a function of the exec() family, and 
  will fork() the command, giving us a the parent and child. Like we saw in class, if something goes wrong, the id would be less than 0, in which case we would end the process and the shell
  would prompt another input to the user. If it was zero, then we run our operations because the fork was successful. 

  FILES NEEDED TO RUN: jgcaballero_argtok.c jgcaballero_argtok.h jgcaballero_exec.c jgcaballero_exec.h jgcaballero_lab2.c and Makefile
  This code was run and tested using cygwin and the -gcc compiler on windows. If you encounter any problems, please email me at jgcaballero@miners.utep.edu*/

#include "jgcaballero_exec.h"
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))

/*Did not know C did not have any boolean type built-in, implemented it to try and implement part 2.2 of the assignment. */
typedef enum { false, true } bool;

/*Arrays that I was using when I was implementing my own shell built-in commands */
  char *builtin_str[] = {"ls1", "help1", "top"};
  int (*builtin_func[]) (char **) = { &ls1, &help1, &top, &top_kek };
  int arr_length = NELEMS(builtin_str);

  char *builtin_str_w_arg[] = {"cat1", "mkdir1", "wc1"};
  int (*builtin_func_w_arg[]) (char **) = { &cat1, &mkdir1, &wc1 };

/*used for printing all implemented functions */
  char *all_functions[] = {"ls", "ls -l", "ls -a", "cat", "help", "top"};
  int all_functions_length = NELEMS(all_functions);

/*This method was giving me a lot of trouble with segmentation faults/stack dumps whenever a & was found, I swapped the args == NULL <-> free in order to fix it. */
  int execBackground(char **args)
  {
    int i;
    // check for the ampersand at the end of the set of tokens
    i = 0;
    while(args[i] != 0)
    {
     i++;
    }                       // traverse to the end of the tokens
    if(args[i-1][0] == '&') // check the last token
    {
        //Printf for debugging
        //printf("[%d] = %s\n", i-1, args[i-1]);
        args[i-1] = NULL;  // remove the ampersand
        free(args[i-1]);
        return 1;
      }
      else
      {
        return 0;
      }
    }

/*I wasted a lot of time on this, I thought that for the assignment, we actually had to implement each function (ls, ls -a.... cat), so I decided to build those functions in the shell
  using the exec family and making my shell know when to call for each one. That's why I spent a while trying to think of the logistics of when to call each method. For example, how would
  I call my ls -l function when I'm only checking for the first argument only. I did learn something out of it too so I guess all that research I did on the exec family wasn't all for 
  nothing. I made part 2.1 of the lab harder than it needed to be. This method will test the built-in methods I implemented (try calling ls1... ls1 -l). All functions I implemented end with 1.*/
  int executeCmd(char **args){
    char kek_string[20];
    strcpy(kek_string, "kek");
    char l_argument[20];
    char a_argument[20];
    strcpy(l_argument, "-l");
    strcpy(a_argument, "-a");

    /*Printfs used for debugging */
    // printf("ARR LENGTH: %d\n",arr_length);

    // printf("[%d] = %s\n", 0, args[0]);
    // printf("[%d] = %s\n", 1, args[1]);

    int i;
    for (i = 0; i < arr_length; i++){

    /*Compares input to see which in-built method it is going to use.
      NOTE: If any of the built-in commands are used, the shell will terminate after execution,
      due to me not implementing the fork() and wait() methods accordingly. These methods were
      to see how I would implement my OWN methods before I realized what I think is what is being really
      asked for. regular_unix_commands is the method that will work with any command given. */
      if(args[1] == NULL){
        if (strcmp(args[0],builtin_str[i]) == 0){
          return (*builtin_func[i])(args);
        }
      } 
      else  {
        //Runs ls -l method
        if (strcmp(args[0],builtin_str[i]) == 0 && strcmp(args[1],l_argument) == 0){
          printf("ls -l method : %s\n",builtin_str[i] );
            //return ls1_l(args);
          return 0;
        }
        //Runs ls -a method
        else if (strcmp(args[0],builtin_str[i]) == 0 && strcmp(args[1],a_argument) == 0){
          printf("ls -a method : %s\n",builtin_str[i] );
            //return ls1_a(args);
          return 0;
        }
        /*Runs any method that has a second argument that is not a command, like
          wc fileName
          cat fileName
          mk dirName */
          else if (strcmp(args[0],builtin_str_w_arg[i]) == 0){
            //printf("MATCH FAM");
            //return (*builtin_func_w_arg[i])(args);
            return 0;
          }
        } 
      }   
      return regular_unix_commands(args);
    }

    /*Attempt to implement aprt 2.2, make background processes not make the shell wait for it to terminate and instead ask for input while the process is running in the 
    background. */
    void signalChild(int signum){
      int status;
      pid_t pid;

      /*WNOHANG : The waitpid() function shall not suspend execution of the calling thread if status is not immediately available for one of the child processes specified by pid.
       "linux.die.net" */
      pid = waitpid(-1, &status, WNOHANG);
    }

/*Based a little bit on the execExample found on blackboard.
  This method is responsible for forking the process correctly and giving the shell the ability to be "on a loop" continually
  asking for input. If the process id is less than zero, terminates, if the pid is zero, do the unix function calls, of course,
  depending on the user input, like how many arguments the user entered. */
  int regular_unix_commands(char **args){
    pid_t pid;
    int status;
    bool background;

  /*Set background to true or false */
    if(execBackground(args) != 0 ){
      background = true;
      printf("Backgound process found! %d\n", background);
    }

    pid = fork();

    if(pid < 0){
    /* Negative process id means there was an error */
      fprintf(stderr,"Error forking a process\n");
      return -1;
    }

    else if (pid == 0) {
    // Child process
      if(args[1] == NULL){
    /*execvp takes a filename, an argumentv(v) and environ(e). This was making my program go into a segmentation fault and do a stackdump. The solution that I applied,
      was to check for more than one argument. Execvp works perfectly if given "ls", but if you give it "ls -l", it goes into a segmentation fault. So if the user only 
      gives the shell one input, execvp works, but if there are more arguments, I simply redirect the command to be run with execlp, which takes a filename, an argument
      list, and the environ(e). This was taken from Figure 8.14 "Differences among the six exec functions, which is the first google result in case you want to check on that.*/

    /*if the number that execvp returns, it means that there was an error forking the process. */
      if (execvp(args[0], args) == -1) {
       fprintf(stderr,"Error forking a process\n");
     }
     return -1;
   } else {
    /*Used to handle the two argument with more than one argument */
    if(execlp(args[0], args[0],args[1], NULL) == -1){
      fprintf(stderr,"Error forking a process\n");
    }
  }

} else {
    // Parent process
  do {
    /*The waitpid function is used to request status information from a child process whose process ID is pid. 
      WUNTRACED : The status of any child processes specified by pid that are stopped, and whose status has not yet been reported since they stopped,
      shall also be reported to the requesting process. "linux.die.net"*/
      waitpid(pid, &status, WUNTRACED);
      /*I'm preeeety sure that for 2.2, I have to catch wether a background is true or not, given by the & sign at the end of the arguments. If it is not a background
      process, then just handle it normally and make all calls to exec appropiately. BUT if it is a background process, I have to tell the shell to not wait for the 
      child process to finish and just make the shell go straight to asking for input. I am pretty sure it has something to do with waitpid() but it is just not 
      working out. But I felt I was atleast somewhat close. */
      if(!background){
      waitpid(-1, &status, WNOHANG);
      //signal(SIGCHLD, signalChild);
      }
      /*WIFEXITED returns a nonzero value if the child process terminated normally with exit or _exit "linux.die.net".
        WIFSIGNALED returns a nonzero value if the child process terminated because it received a signal that was not handled. "linux.die.net"
        While the while loop does not return a 0, keep doing the processes*/
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

/* ---------------------------------- Built-in functions ----------------------------------------------------------------------------*/
  int ls1 (char **args){
    execlp("ls", "ls", (char *)NULL);
  }

  int ls1_l(char **args){
    execlp("ls","ls","-l", NULL);
  }

  int ls1_a(char **args){
    execlp("ls","ls","-a", NULL);
  }

  int cat1(char **args){
    execlp("cat","cat",args[1], NULL);
  }

  int mkdir1(char **args){
    execlp("mkdir","mkdir",args[1], NULL);
  }

  int wc1(char **args){
    execlp("wc","wc",args[1], NULL);
  }

/*Method to test random stuff, ignore. */
  int top(char **args){
    printf("\ntop lel\n");
    return 1;
  }

/*Method to test how I would handle two arguments in the execute cmd function. Just wanted to know if I was reaching this 
dummy method, ignore. */
  int top_kek(char **args){
    printf("\nTOP KEK\n");
    return 1;
  }

/*Method that prints which functions I implemented on this assignment.*/
  int help1(char **args){
    int i;
    printf("\n\nJose Caballero's shell\nJust type the function and hit enter fam.\n");
    printf("The following functions have been implemented on this shell:\n");

    for (i = 0; i < all_functions_length; i++) {
      printf("%d. %s\n", i, all_functions[i]);
    //printf("[%d] = %s\n", j, pointer[j]);
    }
    return 1;
  }
/* ---------------------------------- End of built-in functions ----------------------------------------------------------------------------*/

