#include <stdio.h>
#include <stdlib.h>

void test_program(void);
method1();
method2(char **arr, size_t *arr_len);


int main(void){

	test_program();

}

void test_program(void){

    char *ar;
    size_t ar_len;
    int i;
    int numSpaces=0;
    int number_of_tokens;

    get_input(&ar, &ar_len);
    number_of_tokens = num_of_tokens(ar);
    
    

    //printf("\nAr is  = %s\n", ar);
    printf("Num of tokens found : %d\n",number_of_tokens);

    if (ar) {
        //printf("Array:\n");
        printf(" String length : = %d\n", ar_len);
        for (i=0; i<ar_len; i++) {
        	if (ar[i] == ' '){
        		numSpaces++;
          		//printf("Number of spaces=%d\n",numSpaces);
        		//printf("\0%s\n",ar[i]);
        		 //printf(" [%d] = %c\n", i, ar[i]);
        	}
            printf(" [%d] = %c\n", i, ar[i]);
            //printf("%s\n",ar);
        }
        free(ar);
    }
}

int get_input(char **arr, size_t *arr_len){

	//int length = my_strlen(&arr) - 1;
	//int length = 6;

    char *result = malloc(10);
 		
    printf("Enter string fam: ");
    scanf ("%[^\n]%*c", result);
    printf("You entered :%s\n", result);

    
	*arr = result;
	*arr_len = my_strlen(result);

}

int num_of_tokens(char *s){
	int n;

	for (n = 0; *s != '\0'; s++){
	  if(*s == ' '){
	  	n++;
	  }
	}
return n+1;
}


/* strlen: return length of string s */
int my_strlen(char *s){
	int n;

	for (n = 0; *s != '\0'; s++){
	  n++;
	}

return n;
}





