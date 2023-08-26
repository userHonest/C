/*=============================================
Eksam PG3401 C Programming for Linux , Task 4
Source: threadData.h 
Description: Header file  
Author: Candidate 1087 

==============================================*/

#ifndef THREAD_DATA_H
#define THREAD_DATA_H

#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include <stdbool.h>


#define BUFFER_SIZE 4096
#define NUM_THREADS 2
#define ASCII_START 32
#define ASCII_END 126
#define ASCII_RANGE (ASCII_END - ASCII_START + 1)

//////////////////////////////////////////////////////////////////////
// This structure holds counts of the occurrences of specific words
///////////////////////////////////////////////////////////////////////
typedef struct{
    unsigned int count_word_and;
    unsigned int count_word_at;
    unsigned int count_word_it;
    unsigned int count_word_my;
    unsigned int count_word_hamlet;
    
}WordCount;

////////////////////////////////////////////////////////////////////////
// This structure contains the data each thread needs to do its job. 
//  It includes an array to hold the count of each ASCII character, 
//  the number of bytes currently in the buffer, the buffer itself, 
//  a mutex to ensure that only one thread accesses the buffer at a 
//  time, condition variables to signal when the buffer is full or 
//  empty, the name of the file, a flag indicating whether the file 
//  has been completely read or not, the WordCount structure, and t
//  wo semaphores indicating whether the buffer is full or empty.
//////////////////////////////////////////////////////////////////////////
typedef struct {
    unsigned int count[ASCII_END - ASCII_START + 1];
    int bytes_in_buffer;
    
    unsigned char buffer[BUFFER_SIZE];
    pthread_mutex_t mutex;
    pthread_cond_t cond_full;
    pthread_cond_t cond_empty;
    const char *pFileName;
    int iFileCompletion;
    
    WordCount word_counts;
    
    sem_t sem_buffer_empty;
    sem_t sem_buffer_full;
     
} ThreadData;


void* ThreadA(void* arg);
void* ThreadB(void* arg);
void CountWordsInBuff(const char *buffer, int iBuffSize, WordCount *word_counts);
void SaveData(const ThreadData *data, const char *pSavedFile);


#endif 

//// =========================== END OF FILE ============================ ////
