// UT El Paso EE 4374 Lab 3
// Multi-threaded Prime Number Search
// Author: ???
//
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include "jgcaballero_prime.h"

/* Global variables that are shared */
sPRIME_THREAD primeThreadData[MAX_THREADS];
int	numThreads;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int primeNum = 0;
int counter = 0;

void *test_counter(void *param){
	pthread_mutex_lock( &mutex1 );
	printf("METHOD ACCESSED FAM BY!%d\n", pthread_self());
	counter++;
	pthread_mutex_unlock( &mutex1 );
}

void *print_counter(){
	return counter;
}

void *prime_search(void *param){
	
	FILE *primeFile;
	FILE *notPrimeFile;
	
	primeFile = fopen("primes.dat","w");
	notPrimeFile = fopen("notPrime.dat","w");

	pthread_mutex_lock( &mutex1 );
	for(primeNum=2; primeNum <= (long int) param; primeNum++)
	{
		/*If primeNum is prime, print on prime.dat */
        if (test_prime(primeNum) == 1)
        {
          fprintf(primeFile, "%lu\n", primeNum);
        }
        /*Else, print on notPrime.dat */
        else
        {
		  fprintf(notPrimeFile, "%lu\n", primeNum);
	    }
	}
	
	fclose(primeFile);
	fclose(notPrimeFile);
	
	pthread_exit(0);
	pthread_mutex_unlock( &mutex1 );
}

void *mini_shell(void *param){
	int inputSize, i;
	int threadNum;
	char buff[128];		// Character buffer

	while(1)	{
		// Request a line of input
		write(1, " -> ", 4);
		// Read the line
		inputSize = read(0, buff, 128);
		// Strip off carriage return
		buff[inputSize-1] = '\0';
		
		if((buff[0] >= '1') && (buff[0] <= '9'))
		{
			// An integer was entered
			threadNum = buff[0] - '0';
			if(threadNum <= numThreads)
			{
				printf("Thread %d progress: %d\n", threadNum, primeThreadData[threadNum-1].current);
			}
			else
			{
				printf("Thread %d does not exist\n", threadNum);
			}
			
		}
		else
		{		
			if(buff[0] == 'a')
			{
				// Print status for all threads
				for(i = 0; i < numThreads; i++)
				{
					printf("Thread %d progress: %d\n", i+1, primeThreadData[i].current);
				}
			}
			else
			{
				printf("Unrecognized input\n");
			}
		}

		printf("\n");
		fflush(NULL);

	}

	pthread_exit(0);

}

int test_prime(int n){
	int i;
    for (i=2; i  < n; i++) {
        if (n % i == 0 && i != n)
        return 0;
    }
    return 1;
}
