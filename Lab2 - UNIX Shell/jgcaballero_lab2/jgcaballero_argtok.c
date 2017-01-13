/* Jose Caballero
   EE 4374 
   02/18/2016
   02/23/2016
   03/01/2016*/

   /*This program is a string tokenizer in which a user enters an input and the program will tokenize each word separated with an empty space as a delimiter.
     To accomplish this, I ask for user input, and get_input() stores it to an array arr, and then I pass that array to my function argtok. The argtok function will 
     calculate the string length using my_strlen() method which I implemented, and will also count the number of spaces ' ' using the num_of_spaces() function I implemented.
     After that is done, I create the array ** pointer which will contain the pointers that point to another pointer which contain the token found. Whenever I find an empty
     space, I replace the ' ' with a '\0' to signal the end of a token. I insert that token to str array at the index location and continue doing so until I clear the string.
     Several printfs are put in the code to trace the code's execution and how everything happens, used for debugging purposes.

     FILES NEEDED TO RUN: jgcaballero_argtok.c/h jgcaballero_lab1.c and jgcaballero_Makefile.
     This code was run and tested using cygwin and the -gcc compiler. If you encounter any problems, please email me at jgcaballero@miners.utep.edu

     Input : $ Today's quiz was not so bad after all.
     Output: You entered :Today's quiz was not so bad after all.
     String length method returns : 38
     Num of spaces method returns : 7
     [0] = Today's
     [1] = quiz
     [2] = was
     [3] = not
     [4] = so
     [5] = bad
     [6] = after
     [7] = all.
     [8] = (null)
     Bye now!

     I print the last index + 1 in order to show the null that signals the end of the tokenized string.
      */

#include <stdio.h>
#include <stdlib.h>
#include "jgcaballero_argtok.h"

/* Method used to call all methods, I prefer it to do it this way so main() has only one call and looks clean. Do not know if it is good practice in C. Would appreciate feedback. */
int test_program(){
    char *ar;
   
    get_input(&ar);
    argtok(ar);

    return 0;

}

/* get_input: Function used to get user input
   Input : Jose Caballero
   Output: Jose Caballero

   This method just reads input and assigns it to *arr. */
int get_input(char **arr){

    char *result = malloc(10);
        
    printf("$ ");
    scanf ("%[^\n]%*c", result);
    printf("You entered :%s\n", result);

    *arr = result;

    return 0;
   
}

/* strlen: return length of string s 
   Input : Jose Caballero
   Output: 14 */
int my_strlen(char *s){
    int n;

    for (n = 0; *s != '\0'; s++){
      n++;
    }

return n;
}

/*num_of_spaces: Function used to count the number of spaces in a given input
  Input : Jose Caballero
  Output: 1 */
int num_of_spaces(char *s){
    int n;

    for (n = 0; *s != '\0'; s++){
      if(*s == ' '){
        n++;
      }
    }
return n;
}


/*argtok: Function which takes user input as a "string" of character array, and will tokenize each word that is separated by a ' ' white space 
  and will return an array of pointers to another array. A pointer pointer as it was discussed during lecture.
  Input : Took me some time to get it right
  Output: [0] = took
  [1] = me
  [2] = some
  [3] = time
  [4] = to
  [5] = get
  [6] = it
  [7] = right
  [8] = (null)
  */
char ** argtok(char * str){
    int string_length = my_strlen(str);
    int number_of_spaces = num_of_spaces(str);
    int pointer_index = 0;
    
    /*Print statement used for debugging, returns string length */
    //printf("String length method returns : %d\n", string_length);

    /*Print statement used for debugging, returns number of spaces found on the given input */
    //printf("Num of spaces method returns : %d\n", number_of_spaces);

    //Alocate memory and return a pointer to it, which I set to the pointer variable.
    char **pointer = calloc(number_of_spaces+2, sizeof(char*));
    char temp = ' ';
    
    /*Loop through string searching for white spaces and replacing the ' ' empty space with a '\0' to signal the end of a token */
    int i;
    for(i = 0; i < string_length; i++){
        //Simple swap using temp variable
        if(str[i] == ' '){
            temp = str[i];
            str[i] = '\0';
        } 

        /*If temp is equal to a white space, then increment one to the ptr array and make it point to the address of str which will contain one token that is being pointed to, temp will also 
        hold the index of the next starting character followed by the space. 
        Input:  I am not too fond of C. 
        Output: temp will hold at some point "I,a,n,t,f,o,C." */
        else if(temp == ' '){
            pointer[pointer_index++] = &str[i];
            //Debugging printfs
            //printf("What is this &str????: %s\n",&str[i]);
            temp = str[i];
            //  printf("temp is now : %s\n", &temp);
        }
    }

    //For loop used to print contents of the array, I used the pointer_index + 1 in order to show the last '\0' at the end of the array to indicate the input is finished. Prints [pointer_index+1] = null;
    int j = 0;
    for(j = 0; j < pointer_index + 1; j++){
      //printf("[%d] = %s\n", j, pointer[j]);
    }
    //Deallocating memory
    //printf("Bye now!");
    return pointer;
    //free(pointer);
}





