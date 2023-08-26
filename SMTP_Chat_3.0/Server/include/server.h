//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: server.h
//  Kandidate_nr: 2019

//===============================================//

///============== WARNING =================================================== /// 
//  I might ihave uploaded smiliar code to my github repository
//  i might make it private again, but if not and 
//  someone has forked it, or copied it,and the code is in the
//  web, i would like to avoid confusions or alerts on the plagiarism 
//  scanning. That being said, i provide the link for the repository here
//  as a reference, but the code has beed modified and updated.
//  https://github.com/userHonest/C/tree/main/SMTP_chat_V.1.0
// ===========================================================================
#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define DEFAULT_PORT 25
#define MAX_CLIENTS 1024

struct server_config_t;

/// structure to handle the clients 
typedef struct {
    int iSock;
    struct sockaddr_in addr;
    struct server_config_t *config;
    char szPhoneID[15]; 
    int iIsDone; //<- boolean flag for the clients interaction with the server
} client_t;

/// structure to handle the configuration fo the server ////
typedef struct server_config_t {
    pthread_mutex_t mClientsMutual;  //<- multithreading 
    client_t *clients[MAX_CLIENTS]; // <- max clients the server can handle 
    int iNumClients; //<- connected clients 
    const char *pHello; // <- greeting message sendt by the server. 
    int port;
    char ip[16]; //<- IPv4 
    int iSock; 
    int iListenFlag; //<- Boolean flag for the server to listen or not 
    
} server_config_t;

// functions //
void *AdminHandler(void *pArgs);
void Broadcast(server_config_t *config, client_t *pSender, const char *chMSG);
void *ClientHandler(void *pClientData);
void RunArguments(int iArgC, char *apszArgV[], server_config_t *config);

#endif

/*==END_OF_Header =============*/
