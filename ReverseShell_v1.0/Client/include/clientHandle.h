/*==================================================
Eksam PG3401 C Programming for Linux , Task 5
Source: clientHandle.h
Description: header file to connect the sourcefiles
Author: Candidate 1087 

====================================================*/

#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H
// Libraries // 
#include <netinet/in.h>
/////////////////////////////////////////////////////////////////
// structure contains information
// about the server ,address structure
// this in my opinion makes the code more modular, 
// easier to expand
// this struct is used continiously in the clientHandle.c file
// and also called upon in main.c
////////////////////////////////////////////////////////////////

typedef struct {
    struct sockaddr_in ServAddr;
    int iPort;
    char chServerIP[INET_ADDRSTRLEN];
    
} ServerInfo;


// functions in the clientHandle.c//
int CreateSocket();
void SetServerAddr(ServerInfo *pServ_info);
void ConnectToServer(int iSockFd, ServerInfo * pServInfo);
void *SendData(void *pIsockFd);
void *ReceiveData(void *pIsockFd);
#endif

// ================= end of file ==========================//
