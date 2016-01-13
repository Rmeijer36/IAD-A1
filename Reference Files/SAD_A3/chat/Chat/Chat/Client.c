/* * * * * * * * * * * * * * * * * * * * * * * * * *
* FILE          : Client.c							*
* PROJECT       : Assignment #3 - Chat				*
* PROGRAMMER    : Richard Meijer & Connor McQuade	*
* FIRST VERSION : 03/20/2014						*
* DESCRIPTION   : This is an internet client		*
*				: application that will talk to		*
*				: the server via port 1234.			*
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
#include <time.h>

#define PORT 1234

/* ----- PROTOTYPES ----- */
char* GetTime(void);



/* ----- MAIN ----- */
int main (int argc, char *argv[])
{
	char buffer[BUFSIZ];
	int client_socket, len;
	struct sockaddr_in server_addr;
	struct hostent *host;
	time_t myTime = time(NULL);
	time (&myTime);
	struct tm t = *localtime(&myTime);

	if (argc != 2) 
	{
		printf ("Usage: \n\t>./Client [server_name]\n\tExample: ./Client 127.0.0.1\n");
		return 1;
	}//end if

	/* start chatting; exit condition, user input/buffer = "exit" */
	while(1)
	{
		if ((host = gethostbyname (argv[1])) == NULL) {
			printf ("Unable to get host info.\n");
			return 2;
		}//end if

		if ((client_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
			printf ("Unable to get a client socket.\n");
			return 3;
		}//end if

		//Initialize struct to get a socket to host
		memset (&server_addr, 0, sizeof (server_addr));
		server_addr.sin_family = AF_INET;
		memcpy (&server_addr.sin_addr, host->h_addr, host->h_length);
		server_addr.sin_port = htons (PORT);

		if (connect (client_socket, (struct sockaddr *)&server_addr,sizeof (server_addr)) < 0) 
		{
			printf ("Unable to connect to server.\n");
			close (client_socket);
			return 4;
		}//end if

		//printf ("\n[%s][CLIENT] Connection to server established. Type to chat!",  GetTime());


		//Write to server
		time (&myTime);
		printf ("\n[%s] You: ", GetTime());
		fflush (stdout);
		fgets (buffer, sizeof (buffer), stdin);
		if (buffer[strlen (buffer) - 1] == '\n') 
		{
			buffer[strlen (buffer) - 1] = '\0';
		}//end if
		
		if (strcmp(buffer, "exit") == 0)//exit condition
		{
			break;
		}//end if
		write (client_socket, buffer, strlen (buffer));

		//Read from server
		memset(buffer,0,BUFSIZ);
		len = read (client_socket, buffer, BUFSIZ);
		if (len == 0 || len == -1)
		{
		}
		else
		{
			printf ("\n[%s] Stranger(%s): %s",  GetTime(), inet_ntoa(server_addr.sin_addr), buffer);
		}//end if
		close (client_socket);
	}//end while

	//clean up
	printf ("\n[%s][CLIENT] Disconnected from server. Client shutting down...\n",  GetTime());
	close (client_socket);

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
