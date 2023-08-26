/*=============================================================================
Eksam PG3401 C Programming for Linux 
Task 4
Source: main.c 
Description: main.c for program that scans a book, count words, or letters 
in this case the book is Hamlet 
Author: Candidate 1087 
Version 1.0
==============================================================================*/


/// Libraries ///
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h> 

///////////////////////////////////////////////////////////////////////////////////
//  The main function is a entry point to read from a file using multi-threading
//  from two threads , threadA and threadB.
//  The main handles commandline arguments, < -file> to grab the file in the dir
//  and process it, < -out> to transfer the processed data to a new text file 
//  named BookAnalysis.txt, stored in the same directory as the original file.
//
//////////////////////////////////////////////////////////////////////////////////////7

// Main Function //
// headers //
#include "threadData.h"

// Main Function //
int main(int iArgC, char *pszArgV[]){
    
    pthread_t threadA;
    pthread_t threadB;
    ThreadData data; // instance of de struct
    const char *pFileName = NULL;
    const char *pSavedFile = NULL;
    //bool bPrintFormattedData = false;
    
   
    
    if(iArgC < 3){
        fprintf(stderr, "Usage: %s -file <filename>\n", pszArgV[0]);
        exit(1);
    }
    // Parsing the commandline arguments -file and -out 
    
    for (int i = 1; i < iArgC; i++){
        if(strcmp(pszArgV[i], "-file") == 0 && i + 1 < iArgC){
            
            pFileName = pszArgV[++i];
            
        }
        else if (strcmp(pszArgV[i], "-out") == 0 && i + 1 < iArgC){
            pSavedFile = pszArgV[++i];
        }
    }
    // check to handlde the file 
    if (!pFileName){
        fprintf(stderr, "No file specified, Provide a file using hte '-file' flag.\n");
        exit(EXIT_FAILURE);
    }
    
    if(!pSavedFile){
        pSavedFile = "BookAnalysis.txt";
        
    }
    
     // init the strcutrue with 0 values for so to count 
    
    memset(data.count, 0, sizeof(data.count));
    
    pthread_mutex_init(&data.mutex, NULL);
    sem_init(&data.sem_buffer_empty,0,1);
    sem_init(&data.sem_buffer_full,0,0);
    
    data.word_counts.count_word_and = 0;
    data.word_counts.count_word_at = 0;
    data.word_counts.count_word_it = 0;
    data.word_counts.count_word_my = 0;
    data.word_counts.count_word_hamlet = 0;
    
    
    
    data.bytes_in_buffer = 0;
    data.pFileName = pFileName;
    data.iFileCompletion = 0;
    
    
    //////////////////////////////////////////////////////////////////
    //   ThreadA is responsable for reading the inputfile
    //   ThreadB is responsable for processing the data
    //
    // creating thread A, starting it at NULL and handling the structure data
    //////////////////////////////////////////////////////////////////////////////
    if(pthread_create(&threadA, NULL, ThreadA, &data) != 0){
        perror("Could not create thread A");
        exit(1);
    }
    
    if(pthread_create(&threadB, NULL, ThreadB, &data) !=0){
        perror("Could not create thread B");
        exit(1);
    }
    
     ////////////////////////////////////////////////////////////////////////
    //  Both threads join anf finish their tasks, when the threads
    //  end , the main clean up by destroying the mutex and semaphores
    //  it could be the equivalent of cleaning up memory, but here
    // the memory is not dinamycally allocated so there is nothing to free
    ///////////////////////////////////////////////////////////////////////////
    
    
    
    
    if(pthread_join(threadA, NULL) != 0){
        perror("Could not join thread A");
        exit(1);
    }
    
    if(pthread_join(threadB, NULL) != 0){
        perror("Could not join thread B");
        exit(1);
    }
    
    pthread_mutex_destroy(&data.mutex);
    sem_destroy(&data.sem_buffer_empty);
    sem_destroy(&data.sem_buffer_full);
    
    
    SaveData(&data, pSavedFile);//<- data is saved to file with the given argument
        
    
    
    
    
    return 0;

}

/// =================== END OF FILE ======================///
