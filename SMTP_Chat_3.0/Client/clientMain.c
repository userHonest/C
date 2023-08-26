//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: clientMain.c
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

// Headers //
#include "client.h"

int iSock = 0;
int iReadValue;
struct sockaddr_in stServAddr;
char *chConMSG = "Connected...\n";
char chBuffr[1024] = {0};
char chName[100];
char chMSG[1124];


//================ MAIN  Function ====================// 
int main(int iArgC, char *apszArgV[]) {
    client_config_t config = { EXPECTED_PORT, "127.0.0.1", "" }; // initialize with default values

    HandleArguments(iArgC, apszArgV, &config); //<- handlig comandline arguments 
    
    ///////////////////////////////////////////////////////////////////////////////////////////
    // check for the client, if server is running in one port, and the client tries to
    // connect to another, the it will not work, the client must conncet to the right port
    ////////////////////////////////////////////////////////////////////////////////////////////
    if (config.iPort == 0 || strlen(config.szIP) == 0 || strlen(config.szPhone) == 0){
        printf("Error: Missing requied arguments.\n");
        printf("Run with '-help' for usage information.\n");
        return -1;
    
    }
    
    if (config.iPort != EXPECTED_PORT){
        printf("Error: Invalid port.\n");
        return -1;
    
    }
    
  

    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    stServAddr.sin_family = AF_INET;
    stServAddr.sin_port = htons(config.iPort);

    if (inet_pton(AF_INET, config.szIP, &stServAddr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(iSock, (struct sockaddr *)&stServAddr, sizeof(stServAddr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    
    send(iSock, chConMSG , strlen(chConMSG ), 0);
    printf("Magic Telecomunications \n"); //<- hello message
    iReadValue = read(iSock, chBuffr, 1024);
    printf("%s\n", chBuffr);

    if (strlen(config.szPhone) > 0) {
        printf("Phone number (User ID): %s\n", config.szPhone);
    }
    
    ////////////////////////////////////////////////////////////////////////////////////
    // This part creates a new thread dedicated to receiving messages from the server 
    // using the ReceiveMessages function. 
    /////////////////////////////////////////////////////////////////////////////////////
    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, ReceiveMessages, (void *)&iSock) < 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }


    ///////////////////////////////////////////////////////////////////////////////////
    // This is the main loop, where the user will then be identified by a id number
    // here the user can type in commands while running in the loop.
    ///////////////////////////////////////////////////////////////////////////////////
    int iRun = 1;
    while (iRun) {
        
        
        printf("%s: ", config.szPhone);
        fgets(chBuffr, sizeof(chBuffr), stdin);
        chBuffr[strcspn(chBuffr, "\n")] = '\0';

        if (strncmp(chBuffr, "\\q", 3) == 0) {
            printf("Exiting server\n");
            iRun = 0;
        } else if (strcmp(chBuffr, "\\man") == 0) {
            printf("Client commands: \n");
            printf("\\man : To display client commands\n");
            printf("\\save : (BETA), will save the conversation in a text file (Premium Only))\n");
            printf("\\q : To quit and leave the server\n");
            
        } else {
            
            snprintf(chMSG, sizeof(chMSG), "%s: %s", config.szPhone, chBuffr);
            send(iSock, chMSG, strlen(chMSG), 0);

            
            if (chBuffr[0] == '\\' && strcmp(chBuffr, "\\save") != 0) {
                printf("Unknown command. Type \\man to see the list of available commands\n");
            }
        }
    }


    close(iSock);
    return 0;
}
// ===== End_Of_Program ======= //
