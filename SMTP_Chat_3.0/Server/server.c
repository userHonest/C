//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: server.c
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

// ==<Libraries>================== //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

// ==<Headers> == //
#include "server.h"


///////////////////////////////////////////////////////////////
// fucntion that is called in the main while loop, that lets 
// the server admin , use hardocded commands , like shut down
// or display a manual 
/////////////////////////////////////////////////////////////////
void *AdminHandler(void *pArgs) {
    server_config_t *config = (server_config_t *)pArgs;
    char chInput[256];

    while (1) {
        fgets(chInput, sizeof(chInput), stdin);
        chInput[strcspn(chInput, "\n")] = '\0';

        if (strcmp(chInput, "\\q") == 0) {
            printf("Shutting gracefully down the server...\n");
            close(config->iSock);
            exit(0);
        } else if (strcmp(chInput, "\\man") == 0) {
            printf("Useful server commands: \n");
            printf("\\q : Shut down the server\n");
            printf("\\send : (Beta) Allow clients to send files to eachother (Premium Version)\n");
            printf("\\log : (Beta) Save conversations logs, allow users to access (Premium Version)\n");
            printf("\\man : To display server commands  \n");
        } else {
            printf("Unknown commands. Type \\man to see the list of available commands\n");
        }
    }

    return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////
//  This function is also used in the main while loop, where is the function 
//  that will allow other clients send messages to eachother and sends messages
//  to all connected clients, 
//  execpt the sender 
///////////////////////////////////////////////////////////////////////////////////////

void Broadcast(server_config_t *config, client_t *pSender, const char *chMSG) {
    char szFormattedMessg[1039]; //<- 1024 + 15 for the phoneID
    snprintf(szFormattedMessg, sizeof(szFormattedMessg), "%s: %s", pSender->szPhoneID,chMSG);
    
    pthread_mutex_lock(&(config->mClientsMutual)); // <- locking the tread,only one thread can proceed

    /////////////////////////////////////////////////////////////////////////////////////////
    // The server iterates through all the connected clients.
    //  For each client, if the client is not the sender (pSender), the server sends the 
    //  formatted message to that client using the send function. The send function uses 
    //  the client's socket (iSock) to transmit the data
    ///////////////////////////////////////////////////////////////////////////////////////
    for (int i = 0; i < config->iNumClients; i++) {
        if (config->clients[i] != pSender) {
            send(config->clients[i]->iSock, szFormattedMessg, strlen(szFormattedMessg), 0);
        }
    }

    pthread_mutex_unlock(&(config->mClientsMutual)); //<- unlocking the thread after the message is transmitted 
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//  ClientHandler, is designed to manage individual client connections in the server. When a new client 
//  connects, a new thread runs this function to handle communication with that client
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void *ClientHandler(void *pClientData) {
    // takes datda from the server configuration structure.
    client_t *pClient = (client_t *)pClientData;
    server_config_t *config = ((client_t *)pClientData)->config;
    
    //// reading client ID  /// 
    char chBuff[1024] = {0};
    int iRecievedBytes = read(pClient->iSock, chBuff, sizeof(chBuff) - 1);
    
    if(iRecievedBytes > 0){
        chBuff[iRecievedBytes] = '\0';
        strncpy(pClient->szPhoneID, chBuff, sizeof(pClient->szPhoneID) - 1);
        pClient->szPhoneID[sizeof(pClient->szPhoneID) - 1] = '\0';
    } else {
        printf("Failed to recieve phone ID from client.\n");
        return NULL;
    }
    

    send(pClient->iSock, config->pHello, strlen(config->pHello), 0);
    printf("Connected via SMTP on port %d\n", config->port);

    //////////////////////////////////////////////////////////////////////////////////////////////
    //  The server continues to listen for messages from the client in a loop. For each message:
    //    The buffer chBuff is cleared using memset. Data is read from the client's socket.
    //  If no datais read, the loop stops,  iRun is set to 0
    //  If a message is received, it's printed to the server console and broadcasted to 
    //  all other clients using the Broadcast function
    ///////////////////////////////////////////////////////////////////////////////////////////////

    int iRun = 1;
    while (iRun) {
        memset(chBuff, 0, sizeof(chBuff));
        if (read(pClient->iSock, chBuff, 1024) <= 0) {
            iRun = 0;
        } else {
            printf("Received message from client: %s\n", chBuff);
            Broadcast(config, pClient, chBuff);
        }
    }

    printf("Connection Closed\n");
    close(pClient->iSock);
    pClient->iIsDone = 1; //<- client is done, its place in the thread will go to another.

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
// This function holds all the comandline arguments, using the variables held in
// in the config structure.
//////////////////////////////////////////////////////////////////////////////////// 

void RunArguments(int iArgC, char *apszArgV[], server_config_t *config) {
    for (int i = 1; i < iArgC; i++) {
        
        if (strcmp(apszArgV[i], "-help") == 0) {
            printf("Usage: \n");
            printf("-l : To start listening for incoming connections.\n");
            printf("-p : To set the port number. Default is 25.\n");
            printf("-ip : To set the IP address. Default is 127.0.0.1.\n");
            exit(0);
        
        } else if (strcmp(apszArgV[i], "-l") == 0) {
            config->iListenFlag = 1;
            
        } else if (strcmp(apszArgV[i], "-p") == 0 && i + 1 < iArgC) {
            config->port = atoi(apszArgV[++i]);
        
        } else if (strcmp(apszArgV[i], "-ip") == 0 && i + 1 < iArgC) {
            
            strncpy(config->ip, apszArgV[++i], 15);
        }
    }
}
// ============ End_Of_File ========================= // 
