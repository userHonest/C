//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 4
//  First Version 1.0 - 06/04/2023
//  Patched Version 1.1 - 06/08/2023
//  Sourcefile: cleint.h
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
#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define EXPECTED_PORT 25

// structure to hold the arguments data 
typedef struct {
    int iPort;
    char szIP[16];
    char szPhone[15];


} client_config_t;

void *ReceiveMessages(void *sock_ptr);
int HandleArguments(int iArgC, char **apszArgV, client_config_t *config );
#endif

// ======= End__of Header ========================== //
