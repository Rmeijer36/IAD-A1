/*
 * server.c
 *
 * This is a sample internet server application that will respond
 * to requests on port 5000
 */

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

#define PORT 5000

char buffer[BUFSIZ];


/*
 * this signal handler is used to catch the termination
 * of the child. Needed so that we can avoid wasting
 * system resources when "zombie" processes are created
 * upon exit of the child (as the parent could concievably
 * wait for the child to exit)
 */

void
SigCatcher (int n)
{
    wait3 (NULL, WNOHANG, NULL);    
	signal (SIGCHLD, SigCatcher);
}

int
main (void)
{
	int server_socket, client_socket;
	int client_len;
	struct sockaddr_in client_addr, server_addr;
	int len, i;
	FILE *p;


	/*
	 * install a signal handler for SIGCHILD (sent when the child terminates)
	 */

	signal (SIGCHLD, SigCatcher);

	/*
	 * obtain a socket for the server
	 */

	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get the server socket\n");
		return 1;
	}	/* endif */

	/*
	 * initialize our server address info for binding purposes
	 */

	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (PORT);

	if (bind (server_socket, (struct sockaddr *)&server_addr, 
	sizeof (server_addr)) < 0) {
		printf ("grrr, can't bind server socket\n");
		close (server_socket);
		return 2;
	}	/* endif */

	/*
	 * start listening on the socket
	 */

	if (listen (server_socket, 5) < 0) {
		printf ("grrr, can't listen on socket\n");
		close (server_socket);
		return 3;
	}	/* endif */

	/*
	 * for this server, run an endless loop that will
	 * accept incoming requests from a remote client.
	 * the server will fork a child copy of itself to handle the
	 * request, and the parent will continue to listen for the
	 * next request
	 */

	while (1) {
		/*
		 * accept a packet from the client
		 */

		client_len = sizeof (client_addr);
		if ((client_socket = accept (server_socket, 
		(struct sockaddr *)&client_addr, &client_len)) < 0) {
			printf ("grrr, can't accept a packet from client\n");
			close (server_socket);
			return 4;
		}	/* endif */


		/*
		 * fork a child for reading
		 */

		if (fork() == 0) {
			/*
			 * this is done by CHILD ONLY!
			 */
			//Read from Client
			memset(buffer,0,BUFSIZ);
			len = read (client_socket, buffer, BUFSIZ);
			if (len == 0 || len == -1) {
			}
			else {
				printf("\nClient(%s): %s\n",inet_ntoa(client_addr.sin_addr), buffer);
			}

			//Write to client
			printf("\nYou(%s): ", inet_ntoa(server_addr.sin_addr));
			fflush (stdout);
			fgets (buffer, sizeof (buffer), stdin);
			if (buffer[strlen (buffer) - 1] == '\n') {
				buffer[strlen (buffer) - 1] = '\0';
			}		
			len = strlen(buffer);	
			write (client_socket, buffer, len);
			close (client_socket);
			return 0;
		} else {
			close (client_socket);
		}	/* endif */
	}	/* end while */

	return 0;
}	/* end main */



