//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: client.c
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

/////////////////////////////////////////////////////////////////////////////////////
//  This function is a thread function designed to handle the reception of messages 
//  for a client connected to the server, the infinite loop constantly reads 
//  messages from the server until a termination condition is met.
//////////////////////////////////////////////////////////////////////////////////////
void *ReceiveMessages(void *sock_ptr){
    int iSock  = *(int *)sock_ptr;
    char chBuffr[1024];
    int iReadValue;

    while(1){
    
    /////////////////////////////////////////////////////////////
    // Before reading a new message, the buffer is cleared to 
    //  ensure no data from previous messages interferes.
    /////////////////////////////////////////////////////////////
        memset(chBuffr, 0, sizeof(chBuffr));
        iReadValue = read(iSock, chBuffr, 1024);

        if(iReadValue <= 0){
            printf("Connection closed by server.\n");
            break;
        } 
        else {
            // Extract sender's ID and the actual message
            char *sender_id = strtok(chBuffr , ":");
            char *message = strtok(NULL, "\n");

            // If both sender_id and message are available, display them
            // If not, display a generic message
            if (sender_id != NULL && message != NULL) {
                if(strcmp(sender_id, "SYSTEM") == 0){
                    printf("%s: %s\n", sender_id, message);
                
                }else{
                    printf("Received message from %s: %s\n", sender_id, message);
                }
            
            
            } else {
                printf("Received unknown format message: %s\n", chBuffr);
            }
        }
    }

    close(iSock);
    exit(0);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  process and validate command-line arguments for a client-side program. It extracts 
//  configuration details like the port, IP address, and phone number from the provided 
//  arguments and stores them in a client_config_t structure
//  To indicate success or specific types of failure. In this case, 0 indicates successful 
//  processing of the arguments, and -1 indicates the program should terminate after displaying the help message
// if the user selects a port that is not the one the server is using, there will be no connection
//   that's why the function is a Int
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int HandleArguments(int iArgC, char **apszArgV, client_config_t *config ) {
        
        if (iArgC == 2 && strcmp(apszArgV[1], "-help") == 0) {
            
            printf("Usage: %s -p <port> -ip <ip_address> -phone <phone_number>\n", apszArgV[0]);
            printf("-p: Specify the port to connect to.\n");
            printf("-ip: Specify the IP address to connect to.\n");
            printf("-phone: Specify your phone number for identification.\n");
        
            return -1; // Use -1 as a signal to main() that program should exit
        }
            
        for (int i = 1; i < iArgC; i++){
            if(strcmp(apszArgV[i], "-p") == 0){
                i++;
                if (i < iArgC){
                    config->iPort = atoi(apszArgV[i]);
                }
            }else if (strcmp (apszArgV[i], "-ip") == 0){
                i++;
                
                if (i < iArgC) {
                    strncpy(config->szIP, apszArgV[i], sizeof(config->szIP) - 1);
                    config->szIP[sizeof(config->szIP) - 1] = '\0';  
                }
                
            
            }else if (strcmp(apszArgV[i], "-phone") == 0) {
                i++;
                if (i < iArgC) {
                    strncpy(config->szPhone, apszArgV[i], sizeof(config->szPhone) - 1);
                    config->szPhone[sizeof(config->szPhone) - 1] = '\0';  
            }
          }
        }
        
        return 0;

    }


// ===== End of program  ==== //
