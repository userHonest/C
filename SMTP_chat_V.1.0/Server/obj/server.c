/*=====================// 
Description: server.c file for server
Project: SMTP CHAT
Author: u$3r_h0n3$t
Version: 1.+
Date: 06.04.2023
=======================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
// Headers //
#include "server.h"

pthread_mutex_t mClientsMutual = PTHREAD_MUTEX_INITIALIZER;
client_t *clients[1024];
int iNumClients = 0;
const char *pHello = "[+] Connected..\n";
client_t *clients[1024];


//-------- Handling server commands ----- //
void *AdminHandler(void *pArgs, int iServerFD){
	char chInput[256];
	
	while(1){
		fgets(chInput, sizeof(chInput), stdin);
		chInput[strcspn(chInput, "\n")] = '\0';
		
		if (strcmp(chInput, "\\q")== 0){
			printf("Shutting gracefully down the server...\n");

			close(iServerFD);
			exit(0);
		}

		else if (strcmp(chInput, "\\man") == 0){
			printf("Useful server commands: \n");
			printf("\\q : Shut down the server\n" );
			printf("\\man : To Dipslay server commands  \n");

		}
		else {
			printf("Unknown commands. Type \\man to see the list of awailable commands\n");
		}
	}
	
	return NULL;

}
// -----------------------------------------------------------------------------------//
// == Broadcasting message to all connected clients == //
void Broadcast(client_t *pSender, const char *chMSG){
	pthread_mutex_lock(&mClientsMutual);

	for (int i = 0; i <iNumClients; i++){
		if (clients[i] != pSender){
			send(clients[i]->iSock, chMSG, strlen(chMSG), 0);
		}
	}

	pthread_mutex_unlock(&mClientsMutual);
}
// -----------------------------------------------------------------------------------//

// == Handling Multiple clients __ // 
void *ClientHandler(void *pClientData) {
    //int new_socket = *(int *)socket_desc;
    client_t *pClient = (client_t *)pClientData;
	char chBuff[1024] = {0};

    send(pClient->iSock, pHello, strlen(pHello), 0);
    printf("Connected via SMPT on port 25\n");
    
    int iRun = 1;
    while (iRun) {
        memset(chBuff, 0, sizeof(chBuff));
        if (read(pClient->iSock, chBuff, 1024) <= 0) {
            iRun = 0;
        }
        else {
            printf("Received message from client: %s\n", chBuff);
            Broadcast(pClient, chBuff);
        }
    }
    
    printf("Connection Closed\n");
    close(pClient->iSock);
    free(pClient);
    
    return 0;
}

//============================= END OF PROGRAM =======================================//
