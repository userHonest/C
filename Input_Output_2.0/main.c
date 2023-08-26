//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 2
//  Version 1.0
//  Sourcefile: main.c
//  Kandidate_nr: 2019
//  Date: 04/08/2023
//===============================================//

// ==<Libraries>================== //
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

// ==<Headers> == //
#include <metadata.h>

#define MAX_WORD_LENGTH 50


// ================================================== //
// This function is quite small, so its 
// kept here , istead of metadata.c ,
// is for provide help to users using -help command 
// ===================================================//

void PrintHelp(char* ProgramName){
    printf("Usage: %s -file <add file> [-readable] [-help]\n", ProgramName);
    printf("\t-file <add file>: Specifies the input file to process.\n");
    printf("\t-readable: Writes the output to a readable .txt file in addition to the binary file.\n");
    printf("\t-help: Prints this help message.\n");

}


// ==<Main Function> ================================== // 
int main(int iArgC, char *apszArgV[]) {
    
    
    const char *pszInputFileName;
    char szFirstWord[MAX_WORD_LENGTH + 1];
    int iWordIndex = 1;// <- Counter for words processed 
    int iWordCount = 0;
    bool bReadable;
    
    
    // ============================================================== //
    //  Handling Arguments , where the argument -file will take
    //  the given .txt file in our directory.
    //  Handling the binary file,opening it , writing , then saving
    //  to binary .bin file.
    //  using a -help command to display uselfull info the user.
    //  -readable command lets the user be able to read the data 
    //  to a tetx file.  
    // ============================================================== //  
    
    
    // checking if -help command is provided
    
    for (int i = 1; i < iArgC; ++i){
        if (strcmp(apszArgV[i], "-help") == 0){
            PrintHelp(apszArgV[0]);
            return 0;
        }
    }
    
    // checking other argumnets provided.
    if (iArgC < 3 || strcmp(apszArgV[1], "-file") != 0){
        printf("Invalid arguments. Use '-help' to see usage.\n");
        return 1;
    
    }
    
    // check if '-readable' argument is provided.
    if (iArgC >= 4 && strcmp(apszArgV[3], "-readable") == 0){
        bReadable = true;
    }
    
    pszInputFileName = apszArgV[2];
    
    FILE *pInputFile = fopen(pszInputFileName, "r");
    if (!pInputFile){
        perror("Error opening input file");
        return 1; 
    }
    
    FILE *pOutputFile = fopen("result.bin", "wb");
    if (!pOutputFile) {
        perror("Error opening output bin file");
        fclose(pInputFile);
        return 1;
    
    }
    
    FILE *pReadbleFile = NULL;
    if(bReadable){
        pReadbleFile = fopen("result.txt", "w");
        if(!pReadbleFile){
            perror("Error opening output txt file");
            fclose(pInputFile);
            fclose(pOutputFile);
            return 1;
        }
    }
    
     
    // =================================================================== // 
    //  Processing ech word in the input file, and allocating memory
    //  for the struct in the header st_Metadata and the word itself-.
    //  it will sett the values for the current word, coping the word to
    // the struct , wroting the strcut to the output .bin file ant then
    // freeing the allocat3d memory for the struct.
    // ==================================================================== // 

    while (fscanf(pInputFile, "%s", szFirstWord) == 1){
    
        int iTotalSize = sizeof(struct st_Metadata) + strlen(szFirstWord) + 1;
        struct st_Metadata *pWordsMetadata = (struct st_Metadata *)malloc(iTotalSize);
    
        pWordsMetadata->iIndex = iWordCount + 1;
        pWordsMetadata->bIsLowercase = isLowerCase(szFirstWord);
        pWordsMetadata->bIsUppercase = isUpperCase(szFirstWord);
        pWordsMetadata->bIsAnagram = false;
        pWordsMetadata->bIsDisjoint = false;
        pWordsMetadata->bIsHeterogram = isHeterogram(szFirstWord);
        pWordsMetadata->bIsPalindrom = isPalindrome(szFirstWord);
        pWordsMetadata->iSize = strlen(szFirstWord);
        
        strcpy(pWordsMetadata->szWord, szFirstWord);
        
        fwrite(pWordsMetadata, iTotalSize, 1, pOutputFile);
        
        if (bReadable){
            fprintf(pReadbleFile, "Word %d: %s\n", pWordsMetadata->iIndex, pWordsMetadata->szWord);
            
            if (pWordsMetadata->bIsLowercase){
                fprintf(pReadbleFile, "Lowercase: True\n");
                
            }else {
                fprintf(pReadbleFile, "Lowercase: False\n");
            }
            
            
            if (pWordsMetadata->bIsUppercase){
                fprintf(pReadbleFile, "Uppercase: True\n");
                
            }else {
                fprintf(pReadbleFile, "Uppercase: False\n");
            }
            
            if (pWordsMetadata->bIsHeterogram){
                fprintf(pReadbleFile, "Heterogram: True\n");
                
            }else{
                fprintf(pReadbleFile, "Heterogram: False\n");
            }
            
            if (pWordsMetadata->bIsPalindrom){
                fprintf(pReadbleFile, "Palindrome: True\n");
                
            }else{
                fprintf(pReadbleFile, "Palindrome: False\n");
            }
            
            fprintf(pReadbleFile, "Size: %d\n\n", pWordsMetadata->iSize);
            
        }
        
        free(pWordsMetadata);
        
        iWordCount++;
            
    }
    
    // == Closing file == // 
    fclose(pInputFile);
    fclose(pOutputFile);
    if (pReadbleFile) fclose(pReadbleFile);
    
    printf("[+]Total words processed: %d\n", iWordCount);
    
    if (bReadable){
        printf("[+]The readable result was added as a text file in the same directory\n");
    }
    
    return 0;

}

// ==== End_of_File ===== // 
