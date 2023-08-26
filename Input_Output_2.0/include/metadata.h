//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 2
//  Version 1.0
//  Sourcefile: metadata.h
//  Kandidate_nr: 2019
//  Date: 04/08/2023
//===============================================//

#ifndef METADATA_H
#define METADATA_H

// ==<Libraries> === // 
#include <stdbool.h>

// i defined the max word lengt to 50 
#define MAX_WORD_LENGTH 50

// ======================================================= // 
// I added the structure in the header for several reasons 
// code reusability , ecapsulations , maintanability.
// ======================================================== // 

struct st_Metadata {

    int iIndex;
    bool bIsLowercase;
    bool bIsUppercase;
    bool bIsAnagram;
    bool bIsDisjoint;
    bool bIsHeterogram;
    bool bIsPalindrom;
    int iSize;
    char szWord[MAX_WORD_LENGTH + 1]; //<- +1 is for including space fo null-terminator. 
};

bool isLowerCase(const char* str);
bool isUpperCase(const char* str);
bool isAnagram(const char* word1, const char* word2);
bool areDisjoint(const char* word1, const char* word2);
bool isHeterogram(const char* word);
bool isPalindrome(const char* word);

#endif // METADATA_H

// ========<End_Of_File> ========= // 
