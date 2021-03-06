/*
 * client.c
 *
 * This is a sample internet client application that will talk
 * to the server s.c via port 5000
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

#define PORT 5000

char buffer[BUFSIZ];

int
main (int argc, char *argv[])
{
	int client_socket, len;
	struct sockaddr_in server_addr;
	struct hostent *host;

	/*
	 * check for sanity
	 */

	if (argc != 2) {
		printf ("usage: c server_name\n");
		return 1;
	}	/* endif */

/* start chatting; exit condition, user input/buffer = "exit" */
	while(1)
	{	
	/*
	 * determine host info for server name supplied
	 */
	if ((host = gethostbyname (argv[1])) == NULL) {
		printf ("grrr, can't get host info!\n");
		return 2;
	}	/* endif */

	/*
	 * get a socket for communications
	 */

	if ((client_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) {
		printf ("grrr, can't get a client socket!\n");
		return 3;
	}	/* endif */

	/*
	 * initialize struct to get a socket to host
	 */

	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	memcpy (&server_addr.sin_addr, host->h_addr, host->h_length);
	server_addr.sin_port = htons (PORT);

	/*
	 * attempt a connection to server
	 */

	if (connect (client_socket, (struct sockaddr *)&server_addr,
	sizeof (server_addr)) < 0) {
		printf ("grrr, can't connet to server!\n");
		close (client_socket);
		return 4;
	}	/* endif */

			/*
			* Write to server
			*/
			printf ("\nYou(%s): ", argv[1]);
			fflush (stdout);
			fgets (buffer, sizeof (buffer), stdin);
			if (buffer[strlen (buffer) - 1] == '\n') {
				buffer[strlen (buffer) - 1] = '\0';
			}
			//exit condition
			if (strcmp(buffer, "exit") == 0){
				break;
			}
			write (client_socket, buffer, strlen (buffer));	
			close (client_socket);


			/*/
			* Read from server
			*/
			memset(buffer,0,BUFSIZ);
			len = read (client_socket, buffer, BUFSIZ);
			if (len == 0 || len == -1){
				close (client_socket);
			}
			else {
				printf ("\nServer(%s): %s\n", inet_ntoa(server_addr.sin_addr), buffer);
				close (client_socket);
			}
	} /* end while */

	/*
	 * cleanup
	 */

	close (client_socket);
	printf ("Client is finished\n");

	return 0;
}	/* end main */



