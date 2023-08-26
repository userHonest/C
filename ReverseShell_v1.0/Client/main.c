/*========================================================
Eksam PG3401 C Programming for Linux , 
Task 5
Source: main.c Main function for client
Description: Client main code for TCP /Client/Server app
Author: Candidate 1087 

==========================================================*/

/// Libraries ///
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
// headers //
#include "clientHandle.h"

// standard port is 9000
// but it can be changed 
#define PORT 9000
// local_IP
#define SERVER_IP "127.0.0.1"

/// main function ///
int main() {
    // Variables //
    int iSockFd;
    ServerInfo stServInfo;
    
    
    // creating the socket,
    // caling this function that
    // is in the clientHandle.c file
    iSockFd = CreateSocket(); 
    
    // file rescriptor returs fo the socket
    // intializing ServerInfo struct with ip and address.
    stServInfo.iPort = PORT;
    strncpy(stServInfo.chServerIP, SERVER_IP, INET_ADDRSTRLEN);
    SetServerAddr(&stServInfo);
    
    // establishing connection to the server using
    // socket file descriptor. also stored in ServerInfo strcuture.,
    ConnectToServer(iSockFd, &stServInfo);
    
    
    /*  Two threads are then created one recieves the data from the server
        the other is responsable for sending data to the server ,functions
        like SendData, ReceiveData are passed as parameters.
        main thread wats for the tww, joining them. when they finish the socket 
        is closed.
    */
    pthread_t send_thread, recv_thread;
    pthread_create(&recv_thread, NULL, ReceiveData, (void *)&iSockFd);
    pthread_create(&send_thread, NULL, SendData, (void* )&iSockFd);
    
    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);    

    close(iSockFd);//<- closing connection

    return 0;
}

// ===============End of file ==================== //
