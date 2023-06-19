/*Client Main Funtion
================================== // 
Date 06.04.23
Desription: Main function
Author: u$3r_h0n3$t
Version 1.1
===================================*/
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
char *chConMSG = "Connected via SMTP, PORT 25\n";
char chBuffr[1024] = {0};
char chName[100];
char chMSG[1124];


// MAIN // 
int main(int argc, char *argv[]) {
    

    if ((iSock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    stServAddr.sin_family = AF_INET;
   	stServAddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "192.168.136.128", &stServAddr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(iSock, (struct sockaddr *)&stServAddr, sizeof(stServAddr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    send(iSock, chConMSG , strlen(chConMSG ), 0);
    printf("Magic Telecomunications \n");
    iReadValue = read(iSock, chBuffr, 1024);
    printf("%s\n", chBuffr);

    printf("Add Username: ");
    fgets(chName, 100, stdin);
    chName[strcspn(chName, "\n")] = '\0';

    pthread_t receive_thread;
    if (pthread_create(&receive_thread, NULL, ReceiveMessages, (void *)&iSock) < 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    int iRun = 1;
    while (iRun) {

        printf("Add message: ");
        fgets(chBuffr, 1024, stdin);
        chBuffr[strcspn(chBuffr, "\n")] = '\0';

        if (strncmp(chBuffr, "\\q", 3) == 0) {
            printf("Exiting server\n");
            break;
        }
        
        else if (strcmp(chBuffr, "\\man") == 0 ){
            printf("Client commands: \n");
            printf("\\man : To display client commands\n");
            printf("\\q : To quit and leave the server\n");
        
        
        }else{
            snprintf(chMSG, sizeof(chMSG), "%s: %s", chName, chBuffr);
            send(iSock, chMSG, strlen(chMSG), 0);
            
            if (strcmp(chBuffr, "\\q") != 0 && strcmp(chBuffr, "\\man") != 0 && strcmp(chBuffr, "\\save") != 0) {
            
            
            }
            else if(chBuffr[0] == '\\'){
                printf("Unknown commands. Type \\man to see the list of available commands\n");
            }
        }
        
    }

    close(iSock);
    return 0;
}

// ===== End_Of_Program ======= //
