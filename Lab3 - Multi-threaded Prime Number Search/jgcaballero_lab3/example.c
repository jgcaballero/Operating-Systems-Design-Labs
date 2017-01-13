/*
*
* DECthreads example program conducting a prime number search
*
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
/*
* Constants used by the example.
*/
#define workers 5 /* Threads to perform prime check */
#define request 110 /* Number of primes to find */
/*
* Macros
*/

/*
* Global data
*/
pthread_mutex_t prime_list = PTHREAD_MUTEX_INITIALIZER; /* Mutex for use in
accessing the
prime */
pthread_mutex_t current_mutex = PTHREAD_MUTEX_INITIALIZER; /* Mutex associated
with current
number */
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER; /* Mutex used for
thread start */
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER; /* Condition variable
for thread start */
int current_num= -1;/* Next number to be checked, start odd */
int thread_hold=1; /* Number associated with condition state */
int count=0; /* Count of prime numbers - index to primes */
int primes[request];/* Store prime numbers - synchronize access */
pthread_t threads[workers]; /* Array of worker threads */

static void unlock_cond (void* arg)
{
int status; /* Hold status from pthread calls */
	status = pthread_mutex_unlock (&cond_mutex);
	check (status, "Mutex_unlock");
}
/*
* Worker thread routine.
*
* Worker threads start with this routine, which begins with a condition wait
* designed to synchronize the workers and the parent. Each worker thread then
* takes a turn taking a number for which it will determine whether or not it
* is prime.
*/
void *prime_search (void* arg)
{
int numerator; /* Used to determine primeness */
int denominator; /* Used to determine primeness */
int cut_off; /* Number being checked div 2 */
int notifiee; /* Used during a cancelation */
int prime; /* Flag used to indicate primeness */
int my_number; /* Worker thread identifier */
int status; /* Hold status from pthread calls */
int not_done=1; /* Work loop predicate */
int oldstate; /* Old cancel state */
	my_number = (int)arg;
/*
* Synchronize threads and the parent using a condition variable, the
* predicate of which (thread_hold) will be set by the parent.
*/
status = pthread_mutex_lock (&cond_mutex);
check (status, "Mutex_lock");
pthread_cleanup_push (unlock_cond, NULL);
while (thread_hold) {
	status = pthread_cond_wait (&cond_var, &cond_mutex);
	check (status, "Cond_wait");
}
pthread_cleanup_pop (1);

/*
* Perform checks on ever larger integers until the requested
* number of primes is found.
*/
while (not_done) {
/* Test for cancellation request */
	pthread_testcancel ();
/* Get next integer to be checked */
	status = pthread_mutex_lock (&current_mutex);
	check (status, "Mutex_lock");
current_num = current_num + 2; /* Skip even numbers */
	numerator = current_num;
	status = pthread_mutex_unlock (&current_mutex);
	check (status, "Mutex_unlock");
/* Only need to divide in half of number to verify not prime */
	cut_off = numerator/2 + 1;
	prime = 1;
/* Check for prime; exit if something evenly divides */
	for (denominator = 2;
		((denominator < cut_off) && (prime));
		denominator++) {
		prime = numerator % denominator;
}
if (prime != 0) {
/* Explicitly turn off all cancels */
	pthread_setcancelstate (PTHREAD_CANCEL_DISABLE, &oldstate);
/*
* Lock a mutex and add this prime number to the list. Also,
* if this fulfills the request, cancel all other threads.
*/
status = pthread_mutex_lock (&prime_list);
check (status, "Mutex_lock");

if (count < request) {
	primes[count] = numerator;
	count++;
}
else if (count == request) {
	not_done = 0;
	count++;
	for (notifiee = 0; notifiee < workers; notifiee++) {
		if (notifiee != my_number) {
			status = pthread_cancel (threads[notifiee]);
			check (status, "Cancel");
		}
	}
}
status = pthread_mutex_unlock (&prime_list);
check (status, "Mutex_unlock");
/* Reenable cancellation */
pthread_setcancelstate (oldstate, &oldstate);
}
pthread_testcancel ();
}
return arg;
}
main()
{
int worker_num; /* Counter used when indexing workers */
void *exit_value; /* Individual worker’s return status */
int list; /* Used to print list of found primes */
int status; /* Hold status from pthread calls */
int index1; /* Used in sorting prime numbers */
int index2; /* Used in sorting prime numbers */
int temp; /* Used in a swap; part of sort */
int line_idx; /* Column alignment for output */
/*
* Create the worker threads.
*/

for (worker_num = 0; worker_num < workers; worker_num++) {
	status = pthread_create (
		&threads[worker_num],
		NULL,
		prime_search,
		(void*)worker_num);
	check (status, "Pthread_create");
}
/*
* Set the predicate thread_hold to zero, and broadcast on the
* condition variable that the worker threads may proceed.
*/
status = pthread_mutex_lock (&cond_mutex);
check (status, "Mutex_lock");
thread_hold = 0;
status = pthread_cond_broadcast (&cond_var);
check (status, "Cond_broadcast");
status = pthread_mutex_unlock (&cond_mutex);
check (status, "Mutex_unlock");
/*
* Join each of the worker threads inorder to obtain their
* summation totals, and to ensure each has completed
* successfully.
*
* Mark thread storage free to be reclaimed upon termination by
* detaching it.
*/
for (worker_num = 0; worker_num < workers; worker_num++) {
	status = pthread_join (threads[worker_num], &exit_value);
	check (status, "Pthread_join");
	if (exit_value == (void*)worker_num)
		printf ("Thread %d terminated normally\n", worker_num);
	else if (exit_value == PTHREAD_CANCELED)
		printf ("Thread %d was cancelled\n", worker_num);
	else
		printf ("Thread %d terminated unexpectedly with %#lx\n",
			worker_num, exit_value);
/*
* Upon normal termination the exit_value is equivalent to worker_num.
*/
}

/*
* Take the list of prime numbers found by the worker threads and
* sort them from lowest value to highest. The worker threads work
* concurrently; there is no guarantee that the prime numbers
* will be found in order. Therefore, a sort is performed.
*/
for (index1 = 1; index1 < request; index1++) {
	for (index2 = 0; index2 < index1; index2++) {
		if (primes[index1] < primes[index2]) {
			temp = primes[index2];
			primes[index2] = primes[index1];
			primes[index1] = temp;
		}
	}
}
/*
* Print out the list of prime numbers that the worker threads
* found.
*/
printf ("The list of %d primes follows:\n", request);
for (list = 0, line_idx = 0; list < request; list++, line_idx++) {
	if (line_idx >= 10) {
		printf (",\n");
		line_idx = 0;
	}
	else if (line_idx > 0)
		printf (",\t");
	printf ("%d", primes[list]);
}
printf ("\n");
}
