/*===========================================================
Eksam PG3401 C Programming for Linux. 
Task 2
Source: main.c 
Description: Program that take an input and write it 
to a different output, in this case converting numbers
classifing them.
Version 1.0 
Author: Candidate 1087 

============================================================*/

// Libraries //
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h> // lib to check if file is in the same dir.

// Headers //
#include "mathTypes.h"
#include "mathTypeFunctions.h"


// == main function == // 
int main(int iArgc, char *apszArgV[]){
    
    // ===== Variables ======= // 
    const char *pszInputFilename = apszArgV[1];
    const char *pszOutputFilename = "Result.txt";
    struct stat stBuff;
    int iNumbers;        // both int variables are declared in plural to avoid confusion with others.
    int iIndexes = -1; 
    
    // check so its proper usage in the terminal
    // handling arguments 
    if (iArgc < 2){
        printf("Usage: %s <add_filename>\n", apszArgV[0]);
        return 1;
        
    }
    
    // ==== FILE HANDLING ============== //
    FILE *pInputFile = fopen(pszInputFilename, "r");
    FILE *pOutputFile = fopen(pszOutputFilename, "w");
    // check if eny error opening the file 
    if (pInputFile == NULL || pOutputFile == NULL){
        printf("Error opening the file!\n");
        return 1;
    }
    
    // check if the file is in same dir or is accsessible,
    if (stat(pszInputFilename, &stBuff) != 0){
        printf("File '%s' does not exists or is inaccsessible.\n", pszInputFilename);
        return 1;
    }
    
    //////////////////////////////////////////////////////////////////////////////////////
    //  This whole block of code is in a while loop, and some variables are             //
    //  to be declared inside the loop but on the top , for them to work properly they  //
    //  need to be declared in certain order for the code to work.                      //
    //  1- The name of the properties are stored and declare in an array of pointers    //
    //  "apzsPropertyNames"                                                             //
    //  2 - The structure stMetadata contains the properties in boolean value of       //
    //  the numbers that are going to be readed in the file                             //
    //  3 - The boolean values are then declareded in an array named bPropertyValues      //
    //  corresponding to the properties of the structure stMetadata                     //
    //  4 The code calculates the number of properties in apszPropertyNames array       //
    //  by dividing the size of the array in bytes by the size of the firs element in   //
    //  bytes, the result is stored in variable named iNumProperties                    //
    //                                                                                  //
    //  The reason i decided to use this aproach, is beacuse i came to the conclusion   //
    //  that i i where to handle each properties in conditional statements if/else      //
    // or ternary operators (not common practice) the code would end up very recursive  //
    // and not so elegant according to my vision                                        //
    //////////////////////////////////////////////////////////////////////////////////////
    
    // 8 default digis for binary // 
    int iMinDigits = MinBinaryDigits(pInputFile);
    
    
    while (fscanf(pInputFile, "%d", &iNumbers) != EOF){
        
        // Variables inside the loop //
        iIndexes++;
        // storing property names in a list
        const char *apszPropertyNames[] = {
            "Abundant",
            "Cube",
            "Deficient",
            "Fibonacci",
            "Square",
            "Odd",
            "Perfect",
            "Prime"
        };
        
        // initializing struct with properties of the number
        TASK2_NUMBER_METADATA stMetadata = {
            .iIndex = iIndexes,
            .iNumber = iNumbers,
            .bIsAbundantNumber = isAbundantNumber(iNumbers),
            .bIsCubeNumber = isCubeNumber(iNumbers),
            .bIsDeficientNumber = isDeficientNumber(iNumbers),
            .bIsFibonacci = isFibonacci(iNumbers),
            .bIsSquareNumber = isSquareNumber(iNumbers),
            .bIsOddNumber = isOddNumber(iNumbers),
            .bIsPerfectNumber = isPerfectNumber(iNumbers),
            .bIsPrimeNumber = isPrimeNumber(iNumbers)
            
            
            };
        
        // array of boolean values initializing with the values of them
        // structure
        bool bPropertyValues[] = {
        
            stMetadata.bIsAbundantNumber,
            stMetadata.bIsCubeNumber,
            stMetadata.bIsDeficientNumber,
            stMetadata.bIsFibonacci,
            stMetadata.bIsSquareNumber,
            stMetadata.bIsOddNumber,
            stMetadata.bIsPerfectNumber,
            stMetadata.bIsPrimeNumber
        
        };
        
        // Caltulating the number of elements in the  struct  stored in iNumProperties
        // returing the size of the firts element in the array whic is a pointer to null terminated string      
        int iNumProperties = sizeof (apszPropertyNames) / sizeof(apszPropertyNames[0]);

        
        // printing out lines for each index number
        PrintSeparator(pOutputFile);
        // output for numbes and index 
        fprintf(pOutputFile, "\nIndex:%d\nNumber:  %d\n", iIndexes, iNumbers);
        
        PrintToBinary(pOutputFile, iNumbers, iMinDigits);
        PrintToHex(pOutputFile, iNumbers);
       
        fprintf(pOutputFile, "\n");
        
        
        // writing each property to the outputfile
        // the loop iterates to the bPropertyValues
        // the code check if the property values are
        // true or false
        for (int i = 0; i < iNumProperties; i++){
            fprintf(pOutputFile, "%s:", apszPropertyNames[i]);
            
            if (bPropertyValues[i]){
                fprintf(pOutputFile, "Yes\n");
            } 
            else{
                fprintf(pOutputFile, "No\n");
            }
        
        }
         // printing index separators 
        PrintSeparator(pOutputFile);
    
    
    }
    
    // closing the stream with fclose functions
    // this release any resource assosiated with the file stream 
    // avoiing resource leak
    
    
    fclose(pInputFile);
    fclose(pOutputFile);

    printf("File '%s' was created in current directory.\n", pszOutputFilename);
    return 0;


    }

// ========= End of File ========= //

