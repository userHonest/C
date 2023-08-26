/*========================================================================
Eksam PG3401 C Programming for Linux , Task 4
Source: threadData.c
Description: source file that holds functions to be called upon in main 
Author: Candidate 1087 

=========================================================================*/

/// Libraries ///
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include "threadData.h"
/////////////////////////////////////////////////////////////////////////////////////
//  The function ThreadA takes a void argument, the arguments is a pointer to 
//  <ThreadData> structure by casting the argument to a <threadData> pointer and 
//  and extracting the <FileName> field from it. The file is opened and read in 
//  binary mode using <fopen> reading the data in the file in a while loop, the 
// the buffer size is defined with 4096 bitsin he header.
/////////////////////////////////////////////////////////////////////////////////////


void* ThreadA(void* arg) {
    // casting argument to ponter in ThreadData struct
    // and extracting the pFileName
    
    ThreadData *data = (ThreadData *)arg;
    const char *pFileName = data->pFileName;
    
    
    // reading file in binary
    FILE* fp = fopen(pFileName, "rb");
    if (!fp) {
        perror("Failed to open file");//<- check
        exit(EXIT_FAILURE);
    }

    while (1) {
       
        sem_wait(&data->sem_buffer_empty);
        
        int read_bytes = fread(data->buffer + data->bytes_in_buffer, 1, BUFFER_SIZE - data->bytes_in_buffer, fp);
        
         //<- wait for buffer to become emtpy before adding data
        data->bytes_in_buffer += read_bytes;

        if (read_bytes < BUFFER_SIZE - data->bytes_in_buffer) {
            
            sem_post(&data->sem_buffer_full);//<- break from the loop if the buffer is full
            break;
        }
          //if not,there is new data in the buffer , 
        sem_post(&data->sem_buffer_full);
        
    }
    
    fclose(fp);
    pthread_exit(NULL);//<- exiting thread 
}

/////////////////////////////////////////////////////////////////////////////////////////////
//  This function init a count using memset, it enters a loopt that reads data 
//  from buffer, counts the number of occurences of each ASCII character and counts
//  the number of occurrences of some specific word in the file.
//  the loop first waits with semaphore ,that will indicate that there is new data 
// in the buffer, the iterates over each byte in the buffer, checks if its an ASCII 
// character tithin range and increments the count. The in calls for the amount of words
//  in the buffer. ThreadB will wait for ThreadA to fill the buffer to process the data.
/////////////////////////////////////////////////////////////////////////////////////////////
void* ThreadB(void* arg) {
    
    ThreadData *data = (ThreadData *)arg;
    
     // initializing the count with 
    memset(data->count, 0, sizeof(data->count));
    memset(&data->word_counts, 0 ,sizeof(data->word_counts));

    
        /// The main processoing in the of data in the buffer ///
    while (1) {
       
        sem_wait(&data->sem_buffer_full);
       
         // Counting the number of ocurrences for each
        // ASCII characters in the BUFFER.
        for (int i = 0; i < data->bytes_in_buffer; i++){
            if(data->buffer[i] >= ASCII_START && data->buffer[i] <= ASCII_END){
                data->count[data->buffer[i] - ASCII_START]++;
            }
        }
        
        
    // Counting the number of occurrneces of specific words in the buffer /// 
        CountWordsInBuff(data->buffer, data->bytes_in_buffer, &data->word_counts);
        
        if(data->bytes_in_buffer < BUFFER_SIZE){
            
            break;
        }
                
          // Set the number of bytes in the buffer to 0
        // indicating that the buffer is empty and 
        // signal the buff empty semaphore. ///     

        data->bytes_in_buffer = 0;
        sem_post(&data->sem_buffer_empty);

      
    }
    

    
////////////////////////////////////////////
//  Printing number of unstances on screen
////////////////////////////////////////////    
    
     /// Printing the counts that i named Analysis Report 
    // the data from the amount of words and letters in the file 
    // printing them out 
    printf("\nAnalysis Report\n");
    printf("+-------------------+\n");
    printf("| ASCII |   Total   |\n");
    printf("+-------------------+\n");
    for (int i = ASCII_START; i < ASCII_END; i++) {
        printf("|  '%c'  | %8u |\n", i, data->count[i - ASCII_START]);
        printf("+-------------------+\n");
    }

    printf("\n");
    printf("+----------------------+\n");
    printf("| Common Short Words   |\n");
    printf("+----------------------+\n");
    printf("| Word    |    Count   |\n");
    printf("+----------------------+\n");
    printf("| and     | %8u |\n", data->word_counts.count_word_and);
    printf("| at      | %8u |\n", data->word_counts.count_word_at);
    printf("| it      | %8u |\n", data->word_counts.count_word_it); 
    printf("| my      | %8u |\n", data->word_counts.count_word_my);
    printf("| Hamlet  | %8u |\n", data->word_counts.count_word_hamlet);
    printf("+----------------------+\n");

    
    return(NULL);//<- exiting thread. 
}

//////////////////////////////////////////////////////////////////////////////////////
//  This function iterates over each character in the buffer and counts the number of 
//  of occurrances of specific words by checking the character sequence that follows
//  each instance of a specific letter.
//
//  First the loop, iterate ove reach character in the buffer
//  if the character is <a> , then check if the next characters from the word <and>
//  or <at> and update the word accordingly, and so on with the other words
//////////////////////////////////////////////////////////////////////////////////////
void CountWordsInBuff(const char *buffer, int iBuffSize, WordCount *word_counts){
    for(int i = 0; i < iBuffSize; i++){
        // counting and / at
        if(buffer[i] == 'a'){
            if(i + 2 < iBuffSize && buffer[i + 1] == 'n' && buffer[i + 2] == 'd')
                word_counts->count_word_and++;
            if(i + 1 < iBuffSize && buffer[i + 1] == 't')
                word_counts->count_word_at++;
        }
        
        // counting it 
        if(buffer[i] == 'i'){
            if(i + 1 < iBuffSize && buffer[i + 1] == 't')
                word_counts->count_word_it++;
        }
        
        
        // counting my 
        if(buffer[i] == 'm'){
            if(i + 1 < iBuffSize && buffer[i + 1] == 'y')
                word_counts->count_word_my++;
        }
        
        
        // counting hamlet
        if(buffer[i] == 'H'){
            if(i + 5 < iBuffSize && buffer[i + 1] == 'a' && buffer[i + 2] == 'm' && buffer[i + 3] == 'l' && buffer[i + 4] == 'e' && buffer[i + 5] == 't')
                word_counts->count_word_hamlet++;
        }
    
    }


}
//////////////////////////////////////////////////////////////////
//  function to save data to file  
//  Saving the data to a text file , and its coded so that
// the result has a formatted table to display the result
// this option will be only activated if user types < -out >
// in the terminal and the resul will be saved in as a text file
// BookAnalysis.txt int the same directory.
//////////////////////////////////////////////////////////////////

void SaveData(const ThreadData *data, const char *pSavedFile){
    FILE *pFileSave = fopen(pSavedFile, "w");
    
    if(!pFileSave){
        perror("Failsed to open file");
        exit(EXIT_FAILURE);
    }

    fprintf(pFileSave, "\nAnalysis Report\n");
    fprintf(pFileSave, "+-------------------+\n");
    fprintf(pFileSave, "| ASCII |   Total   |\n");
    fprintf(pFileSave, "+-------------------+\n");
    for (int i = 0; i < ASCII_RANGE; i++) {
        fprintf(pFileSave, "|  '%c'  | %8u |\n", i + ASCII_START, data->count[i]);
        fprintf(pFileSave, "+-------------------+\n");
    }

    fprintf(pFileSave, "\n");
    fprintf(pFileSave, "+----------------------+\n");
    fprintf(pFileSave, "| Common Short Words   |\n");
    fprintf(pFileSave, "+----------------------+\n");
    fprintf(pFileSave, "| Word    |    Count   |\n");
    fprintf(pFileSave, "+----------------------+\n");
    fprintf(pFileSave, "| and     | %8u |\n", data->word_counts.count_word_and);
    fprintf(pFileSave, "| at      | %8u |\n", data->word_counts.count_word_at);
    fprintf(pFileSave, "| it      | %8u |\n", data->word_counts.count_word_it);
    fprintf(pFileSave, "| my      | %8u |\n", data->word_counts.count_word_my);
    fprintf(pFileSave, "| Hamlet  | %8u |\n", data->word_counts.count_word_hamlet);
    fprintf(pFileSave, "+----------------------+\n");

    
    
    
    fclose(pFileSave);


}
/// ==================== End of file ======================== /// 
 
