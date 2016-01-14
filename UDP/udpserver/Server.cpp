/*
* Project     : Industrial Application Design - Assignment #1 (UDP Server)
* Date        : 01/18/2016
* Authors     : Richard Meijer & Pearl Francis
* Description : Simple UDP Server
*
*/
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
int main(){
	int udpSocket, nBytes;
	char buffer[1024];
	struct sockaddr_in serverAddr, clientAddr;
	struct sockaddr_storage serverStorage;
	int *addr_size=0, client_addr_size=0;
	int i;

	/*Create UDP socket*/
	udpSocket = socket(PF_INET, SOCK_DGRAM, 0);

	/*Configure settings in address struct*/
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(7891);
	serverAddr.sin_addr.s_addr =1270000.1;
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	/*Bind socket with address struct*/
	bind(udpSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	/*Initialize size variable to be used later on*/
	//addr_size = 20;

	while (1){
		/* Try to receive any incoming UDP datagram. Address and port of
		requesting client will be stored on serverStorage variable */
		nBytes = recvfrom(udpSocket, buffer, 1024, 0, (struct sockaddr *)&serverStorage, addr_size);

		/*Convert message received to uppercase*/
		for (i = 0; i<nBytes - 1; i++)
			buffer[i] = toupper(buffer[i]);

		/*Send uppercase message back to client, using serverStorage as the address*/
		sendto(udpSocket, buffer, nBytes, 0, (struct sockaddr *)&serverStorage, *addr_size);
	}

	return 0;
}