//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: main.c
//  Kandidate_nr: 2019

//===============================================//

///============== WARNING =================================================== /// 
//  I might ihave uploaded smiliar code to my github repository
//  i might make it private again, but if not and 
//  someone has forked it, or copied it,and the code is in the
//  web, i would like to avoid confusions or alerts on the plagiarism 
//  scanning. That being said, i provide the link for the repository here
//  as a reference, but the code has been modified and updated.
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


// ==<Main Function> ================================== // 
int main(int iArgC, char *apszArgV[]) {
    
    ////////////////////////////////////////////////////////
    // initializes
    server_config_t config;
    pthread_mutex_init(&config.mClientsMutual, NULL);
    config.iNumClients = 0;
    strncpy(config.ip, "127.0.0.1", sizeof(config.ip)-1); 
    
    config.pHello = "[+] Connected..\n";
    config.port = DEFAULT_PORT;
    config.iListenFlag = 0;
     // Ensuring null termination
    
    /////////////////////////////////////////////////////////////////////
    //  The server listens for incoming client connections and can handle 
    //  multiple clients simultaneously using threads
    //  checks the command-line arguments, initializes the server, 
    //  and listens for client connections.
    ////////////////////////////////////////////////////////////////////////
    RunArguments(iArgC, apszArgV, &config);
    
    
    // if -l flag is initialized then the server will listen
    if (config.iListenFlag){
    
    
        int iServerFD;
        struct sockaddr_in sAddr;
        int iOpt = 1;
        int iAddrlen = sizeof(sAddr);

        // Creating socket 
        if ((iServerFD = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(iServerFD, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &iOpt, sizeof(iOpt))) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }

        sAddr.sin_family = AF_INET;
        sAddr.sin_addr.s_addr = inet_addr(config.ip);
        sAddr.sin_port = htons(config.port);
    

        // Bind the socket to the IP and port
        if (bind(iServerFD, (struct sockaddr *)&sAddr, sizeof(sAddr)) < 0) {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
    
        if (listen(iServerFD, 3) < 0) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
    
        printf("[+] Server is listening for incoming connections on %s:%d...\n", config.ip, config.port);
        printf("Use the \\man command for more info about the server\n"); //<- admin handler commands 

        pthread_t sAdminThread;
        if (pthread_create(&sAdminThread, NULL, AdminHandler, &config) < 0) {
            perror("pthread_create admin");
            exit(EXIT_FAILURE);
        }

    ////////////////////////////////////////////////////////////////////////////////
    //  This while loop,let's multiple clients to connect to the server, 
    //  mandgae broadcasting between clients, lets the server user type in 
    //  commands to shut down the server or display useful data.
    // last if a client disconects, its instance of allocaded memory is freed.
    ////////////////////////////////////////////////////////////////////////////////
        while (1) {
            
            int iNewSocket;
            if ((iNewSocket = accept(iServerFD, (struct sockaddr *)&sAddr, (socklen_t *)&iAddrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
            }
       
            pthread_t mClientThread;
            client_t *pNewClient = malloc(sizeof(client_t));
            pNewClient->iSock = iNewSocket;
            pNewClient->addr = sAddr;
            pNewClient->config = &config;

            pthread_mutex_lock(&config.mClientsMutual);
            config.clients[config.iNumClients++] = pNewClient;
            pthread_mutex_unlock(&config.mClientsMutual);

            if (pthread_create(&mClientThread, NULL, ClientHandler, (void *)pNewClient) < 0) {
                perror("pthread_create");
                exit(EXIT_FAILURE);
            }
        
            pthread_detach(mClientThread);
            
            pthread_mutex_lock(&config.mClientsMutual);
            
            //////////////////////////////////////////////////////////////////
            // i wanted to avid memory leaks when a client disconects 
            // then other clients will fill in the gap, ClientHandler f
            //  unction exits before the free(pClient); 
            //  call for any reason [like unexpected error], 
            //  it will result in a memory leak, this was hard to figure out
            //  no segementation fault was shown while ruinning 
            //////////////////////////////////////////////////////////////////
            
            for (int i = 0; i < config.iNumClients; i++){
                if (config.clients[i]->iIsDone){
                    free(config.clients[i]);
                    
                    for (int j = i; j < config.iNumClients - 1; j++){
                        config.clients[j] = config.clients[j + 1];
                    }
                    
                    config.iNumClients--;
                    i--;
                }
            
            }
            
            pthread_mutex_unlock(&config.mClientsMutual);
        }

        close(iServerFD);
    
    
    } else {
        printf("Server is not listening. Use the -l command to start listeing for incomming connections.\n");
        return 0;
    }

    
    
}



//================ END OF PROGRAM ====================//
