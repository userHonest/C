/*=====================// 
Description: Header file for server
Project: SMTP CHAT
Author: u$3r_h0n3$t
Version: 1.+
Date: 06.04.2023
=======================*/
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 25

typedef struct{
    int iSock;
    struct sockaddr_in addr;
} client_t;

extern pthread_mutex_t mClientsMutual; // Add this line
extern client_t *clients[1024];
extern int iNumClients;

extern const char *pHello;

// functions //
void *AdminHandler(void *pArgs, int iServerFD);
void Broadcast(client_t *pSender, const char *chMSG);
void *ClientHandler(void *pClientData);

#endif 

/*==END_OF_Header=============*/
