/*=============================================
Eksam PG3401 C Programming for Linux 
Task 5
Source: main.c 
Description: Main function for server
Author: Candidate 1087 
Version 1.0 
==============================================*/


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

#define DEFAULT_PORT 9999
#define BUFFER_SIZE 1024

///////////////////////////////////////////////////////////////////////////////////
//  The server listens for incomming connections and comunicates
//  with a connected client. The executable takes two arguments
//  < -l > to listen for incomming connectons, <-p> to define a port
//  the defaut port in this programs is 9999, but its optional.
//  the <-h> help command to display the available options for this client
//  The different instances are in this case divided in fucntions 
//  That are called upon in main, creating a server socket, acceping,
//  listening and bind , and process arguments.
//  
//  The other implementation that is this client has, that is has a 
//  \shell command ,thats lets the server user execute some system commands
//  like listing out the files in the client side, making a file. 
//  To exit the shell instance, the user have to type \exit, and it goes back
//  to server mode where the server and client can send message to eachother. 
///////////////////////////////////////////////////////////////////////////////////

//// MAIN FUNCTION //////
int main(int iArgC, char *pszArgV[]){
    
    int iServerFD; //<- server file descriptor
    int iClientFD; // <- client file descriptor
    struct sockaddr_in server_addr, client_addr; // adsresses 
    
    int iListenMode = 0; 
    int iPort = DEFAULT_PORT;
    
    Arguments(iArgC, pszArgV, &iListenMode, &iPort); // func

    if (!iListenMode) {
        printf("Error: Missing -l flag for listening mode.\n");
        exit(EXIT_FAILURE);
    }

    // binding server 
    iServerFD = CreateServerSocket(&server_addr, iPort);  
    BindAndListen(iServerFD, &server_addr);  
    
    printf("[+] Server listening on port %d\n", iPort);
    
    iClientFD = AcceptConn(iServerFD, &client_addr); 
    printf("Client Connected\n");
    
    pthread_t recv_thread, send_thread;
    
    
    // init threads with both functions -> RecieveData, SendDataLoop
    pthread_create(&recv_thread, NULL, (void *)RecieveData, (void* )&iClientFD);
    pthread_create(&send_thread, NULL, (void *)SendDataLoop,(void *)&iClientFD);

    // wait for both threads to be complete 
    pthread_join(recv_thread, NULL);
    pthread_join(send_thread, NULL);

    close(iClientFD);
    close(iServerFD);

    return 0;
}
//// =================== END OF FILE =========================////
