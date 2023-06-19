/*=Header_File_Client
================================== // 
Date 06.04.23
Desription: Headerfile for chat
Author: u$3r_h0n3$t
Version 1.1
===================================*/
#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define PORT 25

void *ReceiveMessages(void *sock_ptr);

#endif

// ======= End__of Header ========================== //
