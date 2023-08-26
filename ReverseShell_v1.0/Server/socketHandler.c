/*===================================================================
Eksam PG3401 C Programming for Linux , 
Task 5
Source: socketHandler.c
Description: Source file, for socketHandler.c where all the functions 
runned in main are 
Author: Candidate 1087 
====================================================================*/

// Libraries //
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

// Headers //
#include "socketHandler.h"

#define BUFFER_SIZE 1024

//////////////////////////////////////////////////////////////////////////////7
//  This function s creates a server socket, and takes two arguments
//  the sockaddr_in and iPort.
//  Socket function creates a socket stream with its current methods
//  Then uses memset to init server address to 0,This means that the socket
//  can recieve connectios from any IP adress.The htons function converts
//  the int from host byte order to network byte order. 
//  finally it returns a socket file descriptor.
//////////////////////////////////////////////////////////////////////////////

int CreateServerSocket(struct sockaddr_in *server_addr, int iPort) {
    int iServerFD;//<c creates the socket
    
    // check for socket connection
    // if not, it prints our socket failed 
    if ((iServerFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Failed");
        exit(EXIT_FAILURE);
    }

    // init server addr and port 
    memset(server_addr, 0, sizeof(*server_addr));

    server_addr->sin_family = AF_INET;
    server_addr->sin_addr.s_addr = INADDR_ANY;
    server_addr->sin_port = htons(iPort);

    return iServerFD;
}

//////////////////////////////////////////////////////////////////////////
//  This funcion binds the server socket to the specified address 
//  and port and sets the socket to listen. the server socket file 
//  descriptor, a pointer to the server address structure cast as 
//  a struct sockaddr *, and the size of the server address structure
//   if bind is 0 it will fail, the same as listen-
/////////////////////////////////////////////////////////////////////////

void BindAndListen(int iServerFD, struct sockaddr_in *server_addr) {
    
    // binding server with addr and port
    if (bind(iServerFD, (struct sockaddr *)server_addr, sizeof(*server_addr)) < 0) {
        perror("Bind Failed");
        exit(EXIT_FAILURE);
    }

    // Listening for connections 
    if (listen(iServerFD, 1) < 0) {
        perror("Listen Failed");
        exit(EXIT_FAILURE);
    }
}

//////////////////////////////////////////////////////////////////////////
//  This function is responsible for accepting an incoming connection 
//  on the server socket and returning the client socket file descriptor.
//  The function calls the accept, accepting incoming connections, once
//   the connection is haccepted the socket file descriptor is stored
//  in the ClientFD
///////////////////////////////////////////////////////////////////////7

int AcceptConn(int iServerFD, struct sockaddr_in *client_addr) {
    int iClientFD;
    int iAddrLen = sizeof(*client_addr);

    // accept conn on server
    if ((iClientFD = accept(iServerFD, (struct sockaddr *)client_addr, (socklen_t *)&iAddrLen)) < 0) {
        perror("Accept Failed");
        exit(EXIT_FAILURE);
    }

    return iClientFD;
}


//////////////////////////////////////////////////////////////////////////////
//  This function is responsible for receiving data from a client 
//  socket and  printing it to the console. The function then enters a loop 
//  , receiving data from the client socket 
//  and printing it to the console. Inside the loop, the function uses memset 
//  to clear the chBuff buffer to all zeros, then calls recv to receive data 
//  from the client socket
//
//
void *RecieveData(void *client_fd_ptr) {
    int iClientFD = *(int *)client_fd_ptr;
    char chBuff[BUFFER_SIZE];
    ssize_t b;


// Recieving data from the client socket,
    while(1){
    memset(chBuff, 0, BUFFER_SIZE); //<- socket at 0
    ssize_t b = recv(iClientFD, chBuff, BUFFER_SIZE - 1, 0);
    
    if (b <= 0){
        if(b == 0){
            printf("Client disconnected.\n");
        }
        else{
            perror("Recv Fail.\n");
        }
        break; //<- exit the loop 
    }    
        chBuff[b] = '\0'; // null temrnate the string and print it to console 
        printf("$>: %s", chBuff);
    }
    return NULL;
}

//////////////////////////////////////////////////////////////////////////////
//  Instead of having all the arguments in the main, as most of the programs
//  i have made in C, this time i added them in a function, the options are
// displayed in a loop,  it takes 3 arguments, is has a iHelpFlag with them
// hetopt function to poreddes command line options, 
//   
void Arguments(int iArgC, char *pszArgV[], int *iListenMode, int *iPort) {
    int iOpt;
    int iHelpFlag = 0; // -> to indicate helpe message to user. 

    while ((iOpt = getopt(iArgC, pszArgV, "hlp:")) != -1) {
        switch (iOpt) {
            case 'h': //<- help option 
                iHelpFlag =1;
                break;
            
            case 'l'://<-listen mode for incommeing connections 
                *iListenMode = 1;
                break;
            case 'p':
                *iPort = atoi(optarg); //<- store option 
                break;
            default:
               
                iHelpFlag = 1;
        }
    }
    
    if(iHelpFlag){
        printf("Usage: %s [-l] [-p <port>] [-h]\n",pszArgV[0]);
        printf("\nOptions:\n");
        printf("    -l\t\tListen for incoming connections\n");
        printf("    -p <port>\tSpecify the port number to use (default: 9000)\n");
        printf("    -h\t\rDisplay help message\n");
        printf("\nCommands:\n");
        printf("    \\shell\tEnter shell mode to execute commands on the client\n");
        printf("    \\exit\t\tExit shell mode");
        exit(EXIT_SUCCESS);
    }
}

////////////////////////////////////////////////////////////////////
//  Here is the fun part, the \shell command lets the user execute
//  system commands over in the client code, so if the user types
//  shell in the server, that wil gets stored in the buffer ,
//  and the client part will execute system commands wit /bin/bash
// no thaht this works only for linux
// if user types \exit, they will exit the shell instance and still
// be connected to the server.
//////////////////////////////////////////////////////////////////////

void *SendDataLoop(void *client_fd_ptr){
    int iClientFD = *(int *)client_fd_ptr;
    char szBuff[BUFFER_SIZE];
    const char *cpShellCMD = "\\shell";
   

    int iShellMode = 0;

    while (1) {
        
        if(iShellMode){
            printf("Server (shell):");
        }
        else{
            printf("Server: ");
        }
        
        memset(szBuff, 0, BUFFER_SIZE);
        fgets(szBuff, BUFFER_SIZE, stdin);
        
        

        
        if(!iShellMode && strncmp(szBuff, cpShellCMD, strlen(cpShellCMD))== 0){
            iShellMode = 1;
        
        }
        else if(iShellMode && strncmp(szBuff, "\\exit\n", 5) == 0){
            iShellMode = 0;
        }
        
        send(iClientFD, szBuff, strlen(szBuff), 0);
    }

    return NULL;
    

}
/// ============== end of file ==================///

