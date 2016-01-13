/*
 * threadedApp.c
 *
 * This program demonstrates threads of execution
 */


#define _REENTRANT

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


/*
 * prototypes
 */

void* my_thread_function (void *);
int   really_waste_some_time (int);


int main (int argc, char *argv[])
{
  int		status, *ptr_status;
  int		x;
  time_t	startTime, stopTime;
  pthread_t 	tid[10];

  /* timestamp the start of the application */
  startTime = time(NULL);
  
  /*
   * initialize local variables
   */
  ptr_status = &status;

  /*
   * only run if 2 or more arguments available and limit
   * number of commandline arguments to 10 additional arguments
   */
  if (argc < 2) 
    {
      printf ("usage: threadedApp word1 word2 word3 ...\n");
      exit (1);
    }

  if (argc > 11) argc = 11;

  /*
   * run a loop for each additional argument on commandline
   * and start a thread of execution, giving each thread the
   * appropriate commandline argument. the thread will 
   * "play" with the word and rearrange the letters in the word
   * to simulate real work. Note how we keep track of each
   * thread's ID in the tid array.
   */
  for (x = 0; x < (argc - 1); x++)
  {
    if (pthread_create (&tid[x], NULL, my_thread_function, (void *)argv[x + 1]) != 0) 
    {
      printf ("Arghh !! I cannot start thread #%d\n", x);
      exit (2);
    }
  }

  /*
   * wait for each thread to finish up, and the return
   * status of each thread will be the length of each
   * corresponding argument on the commandline. Note that
   * the ptr_status variable is used to communicate the location
   * of the status variable for the pthread_join function to fill
   * in on our behalf.
   */
  for (x = 0; x < (argc - 1); x++) 
  {
    if (pthread_join (tid[x], (void **)ptr_status) != 0) 
    {
      printf ("Arghh !! I cannot wait for thread #%u\n", tid[x]);
      exit (3);
    }
    printf ("Thread ID: %u length of argument %d is %d\n", tid[x],(x+1),status);
  }

  /*
   * we're done!
   */
  printf ("All threads done! Thanx for coming ...\n");

  /* timestamp the stop of the application */
  stopTime = time(NULL);
  printf("*** TIME SPENT : %d seconds\n", (stopTime-startTime));

}


/*
 * void *my_thread_function (void *data);
 *
 * This is a thread function. it accepts a single pointer
 * to any arbitrary data from the caller, and returns a pointer
 * to arbitrary data. Note the use of the function pthread_self() - 
 * it will obtain the thread ID of the current thread, to help
 * keep the threads apart visually during their execution.
 */

void * my_thread_function (void *data)
{
  char *word = (char *)data;
  int x;
	
  /*
   * simulate some work by performing some goofy
   * array operations on the incoming string
   */
  printf ("Thread ID: %u forwards: ", pthread_self());
  for (x = 0; x < strlen (word); x++)
  {
    printf ("%c", word[x]);
  }
  printf ("\n");
  fflush (stdout);
	
  really_waste_some_time (strlen (word));

  printf ("Thread ID: %u backwards: ", pthread_self());
  for (x = 0; x < strlen (word); x++)
  {
    printf ("%c", word[strlen (word) - 1 - x]);
  }
  printf ("\n");
  fflush (stdout);
	
  really_waste_some_time (strlen (word));

  printf ("Thread ID: %u uppercase: ", pthread_self());
  for (x = 0; x < strlen (word); x++)
  {
    printf ("%c", toupper (word[x]));
  }
  printf ("\n");
  fflush (stdout);
	
  really_waste_some_time (strlen (word));

  printf ("Thread ID: %u lowercase: ", pthread_self());
  for (x = 0; x < strlen (word); x++)
  {
    printf ("%c", tolower (word[x]));
  }
  printf ("\n");
  fflush (stdout);
	
  really_waste_some_time (strlen (word));
 
  /*
   * enough simulated work. let's return a status value
   * now ...
   */
	
  return (void *)strlen (word);
}


/*
 * int really_waste_some_time (int t);
 *
 * This function just sleeps for the sake of wasting
 * some time. It's meant to simulate some "real" work
 * happening.
 */

int really_waste_some_time (int t)
{
  sleep (t);
  return 0;
}
















