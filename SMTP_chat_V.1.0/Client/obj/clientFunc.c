/*Client Funtion
================================== // 
Date 06.04.23
Desription: Client function for chat
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

// function that lets clients chat with eachother.
void *ReceiveMessages(void *sock_ptr){

	int iSock  = *(int *)sock_ptr;
	char chBuffr[1024]; // allocate with max number if 1024 bytes
    int iReadValue;

	while(1){
		memset(chBuffr, 0, sizeof(chBuffr));
		iReadValue = read(iSock, chBuffr, 1024);

        if(iReadValue <= 0){
            printf("Connection closed by server.\n");
            break;
        } 
		else{
            printf("Received message: %s\n", chBuffr);
        }
	}

	close(iSock);
	exit(0);
	return NULL;

	
}


// ===== End of program  ==== //
