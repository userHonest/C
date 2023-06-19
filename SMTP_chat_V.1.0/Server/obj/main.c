/*==================================// 
Description: main.c file for server
Project: SMTP CHAT
Author: u$3r_h0n3$t
Version: 1.+
Date: 06.04.2023
===================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#include "server.h"

int main(int argc, char *argv[]) {
	int iServerFD;
	struct sockaddr_in sAddr;
	int iOpt = 1;
	int iAddrlen = sizeof(sAddr);
	const char *pHello = "[+] Connected..\n";



    // Creating socket file descriptor/ strAem oriented, more than one.>!
    if ((iServerFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 25
    if (setsockopt(iServerFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &iOpt, sizeof(iOpt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = inet_addr("192.168.136.128");
    sAddr.sin_port = htons(PORT); // will switch the bytes // in different order.

    // Forcefully attaching socket to the port 25
    if (bind(iServerFD, (struct sockaddr *)&sAddr,
             sizeof(sAddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    if (listen(iServerFD, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("[+] Server is listening for incoming connections...\n");
	printf("Use the \\man command for mor info about the server\n");

	pthread_t sAdminThread;
	if (pthread_create(&sAdminThread, NULL,(void *(*)(void *))AdminHandler, (void* )(intptr_t)iServerFD) < 0){
		perror("pthread_create admin");
		exit(EXIT_FAILURE);
	}

// Mantianing connection 
    while (1) {
        int iNewSocket;
        if ((iNewSocket = accept(iServerFD, (struct sockaddr *)&sAddr,
                                 (socklen_t *)&iAddrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
       
		pthread_t mClientThread;
        client_t *pNewClient = malloc(sizeof(client_t));
        pNewClient->iSock = iNewSocket;
		pNewClient->addr = sAddr;

	// implementing new clients to the list
		pthread_mutex_lock(&mClientsMutual);
		clients[iNumClients++] = pNewClient;
		pthread_mutex_unlock(&mClientsMutual);

		
        
        if (pthread_create(&mClientThread, NULL, ClientHandler, (void *)pNewClient) < 0){
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
        
        pthread_detach(mClientThread);
    }

    close(iServerFD);
    
    return 0;
}


//================ END OF PROGRAM ====================//
