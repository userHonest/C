/*======================================================================================
Eksam PG3401 C Programming for Linux.
Task 2 
Source: mathTypes.h -> header file 
Description: Header file to connect with the mathTypes.c source file
Author: Candidate 1087
Version 1.0

========================================================================================*/

#ifndef MATH_TYPES_H
#define MATH_TYPES_H

#include <stdbool.h>
#include <stdio.h>

/////////////////////////////////////////////////////////////////
// This is a structure that contains boolean and integere fields
// Theese fields store the index and the number and its properties
// At last there is a partat that contains function declarations
// to print if that number that is stored in the structure as
// binary and hexadecimal.
////////////////////////////////////////////////////////////////

typedef struct TASK2_NUMBER_METADATA{

    int iIndex;
    int iNumber;
    bool bIsAbundantNumber;
    bool bIsCubeNumber;
    bool bIsDeficientNumber;
    bool bIsFibonacci;
    bool bIsSquareNumber;
    bool bIsOddNumber;
    bool bIsPerfectNumber;
    bool bIsPrimeNumber;

} TASK2_NUMBER_METADATA;


void PrintSeparator(FILE *pOutputFile);
void PrintToBinary(FILE *pOutputFile, int iNum, int iMinDigits);
int MinBinaryDigits(FILE *pInputFile);
void PrintToHex(FILE *pOutputFile, int iNumb);
#endif // MATH_TYPES_H

// ======== End of File  ========== //  


