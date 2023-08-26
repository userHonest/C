/*===================================================================
Eksam PG3401 C Programming for Linux , Task 5
Source: socketHandler.h
Description: header to connect other functions from socketHandler.c
Author: Candidate 1087 

====================================================================*/

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H
// Libraries
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <pthread.h>


// functions in socketHandler.c //

int CreateServerSocket(struct sockaddr_in *server_addr, int iPort);
void BindAndListen(int iServerFD, struct sockaddr_in *server_addr);
int AcceptConn(int iServerFD, struct sockaddr_in *client_addr);
void *RecieveData(void *client_fd_ptr);
void Arguments(int argc, char *argv[], int *iListenMode, int *iPort);
void *SendDataLoop(void *client_fd_ptr);

#endif

////================ end of file ======================////
