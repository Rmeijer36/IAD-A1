/* * * * * * * * * * * * * * * * * * * * * * * * * *
* FILE          : Server.c							*
* PROJECT       : Assignment #3 - Chat				*
* PROGRAMMER    : Richard Meijer & Connor McQuade	*
* FIRST VERSION : 03/20/2014						*
* DESCRIPTION   : This is a simple internet server	*
* 				: application that will respond to	*
*				: requests on port 1234.			*
* * * * * * * * * * * * * * * * * * * * * * * * * */
/* ----- LIBRARIES/CONSTANTS ----- */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>

#define PORT 1234

/* ----- PROTOTYPES ----- */
void SigCatcher (int n);
char* GetTime(void);



/* ----- MAIN ----- */
int main (void)
{
	/* ----- Varriables ----- */
	char buffer[BUFSIZ];
	int server_socket, client_socket;
	int client_len;
	struct sockaddr_in client_addr, server_addr;
	int len, i;
	int firstRun = 0;
	FILE *p;

	//install a signal handler
	signal (SIGCHLD, SigCatcher);

	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf ("Error while attempting to get the server socket.\n");
		return 1;
	}//endif

	//initialize our server address info for binding purposes
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (PORT);

	if (bind (server_socket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
	{
		printf ("Error binding server socket.\n");
		close (server_socket);
		return 2;
	}//endif

	if (listen (server_socket, 5) < 0) 
	{
		printf ("Error listening on socket.\n");
		close (server_socket);
		return 3;
	}//end if

	//printf("[%s][SERVER] Server running. Waiting for connections...", GetTime());

	/*
	* run an endless loop that will listen for incoming requests
	* and fork a child to handle them.
	*/
	while (1) 
	{
		client_len = sizeof (client_addr);
		if ((client_socket = accept (server_socket, (struct sockaddr *)&client_addr, &client_len)) < 0) 
		{
			printf ("Error accepting a packet from client.\n");
			close (server_socket);
			return 4;
		}//end if

		if (fork() == 0) 
		{
			//printf("\n[%s][SERVER] Connection established with client (%s).", GetTime(), inet_ntoa(client_addr.sin_addr));

			//Read from Client
			memset(buffer,0,BUFSIZ);
			len = read (client_socket, buffer, BUFSIZ);
			if (len == 0 || len == -1) 
			{
			}
			else
			{
				printf("\n[%s] Client(%s): %s", GetTime(), inet_ntoa(client_addr.sin_addr), buffer);
			}//end if
			
			fflush (stdout);
			printf("\n[%s] You: ", GetTime());
			fgets (buffer, sizeof (buffer), stdin);
			if (buffer[strlen (buffer) - 1] == '\n')
			{
				buffer[strlen (buffer) - 1] = '\0';
			}//end if

			//printf("\n[%s][SERVER] Client (%s) disconnected.", GetTime(), inet_ntoa(client_addr.sin_addr));
			len = strlen(buffer);
			write (client_socket, buffer, len);
			close (client_socket);
			return 0;
		} 
		else 
		{
			close (client_socket);
		}//end if
	}//end while

	return 0;
}//end main


/*
 * Name: GetTime(void)
 * Parameters: void
 * Output: char* - time string
 * Description: outputs the local time in the format HH:MM:SS
 */
char* GetTime(void)
{
	time_t myTime = time(NULL);
	time (&myTime);
	struct tm t = *localtime(&myTime);
	char time[BUFSIZ];
	char buffer[BUFSIZ];

	//Format time data - HH:MM:SS (Hour, Minute, Second)
	sprintf(buffer,"%d",t.tm_hour);
	strcpy(time, buffer);
	strcat(time, ":");
	sprintf(buffer,"%d",t.tm_min);
	strcat(time, buffer);
	strcat(time, ":");
	sprintf(buffer,"%d",t.tm_sec);
	strcat(time, buffer);

	return time;
}


/*
 * Name: SigCatcher(int n)
 * Parameters: int n
 * Output: void
 * Description: this signal handler is used to catch the termination
 * of the child (as the parent could concievably
 * wait for the child to exit).
 */
void SigCatcher (int n)
{
    wait3 (NULL, WNOHANG, NULL);    
    signal (SIGCHLD, SigCatcher);
}//end SigCatcher();

