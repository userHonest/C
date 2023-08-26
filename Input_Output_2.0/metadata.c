//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 2
//  Version 1.0
//  Sourcefile: metadata.c
//  Kandidate_nr: 2019
//  Date: 04/08/2023
//===============================================//

// ==<Libraries>================== // 
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// ==<Headers>==================== //
#include <metadata.h>
// ============================================================
// These are the provided functions they are not modified.
// ============================================================

// ==<Functions>==================== //

bool isLowerCase(const char* str) {
   while (*str != '\0') {
      // Check if the character is a lowercase letter (ASCII values: 97-122)
      if (*str < 'a' || *str > 'z') {
         return false; // Character is not lowercase
      }
      str++;
   }
   return true; // All characters are lowercase
}


bool isUpperCase(const char* str) {
   while (*str != '\0') {
      // Check if the character is an uppercase letter (ASCII values: 65-90)
      if (*str < 'A' || *str > 'Z') {
         return false; // Character is not uppercase
      }
      str++;
   }
   return true; // All characters are uppercase
}

bool isAnagram(const char* word1, const char* word2) {
   int count1[26] = { 0 }; // Array to count the occurrences of each letter in word1
   int count2[26] = { 0 }; // Array to count the occurrences of each letter in word2

   int length1 = strlen(word1);
   int length2 = strlen(word2);

   // If the lengths of the words are not equal, they cannot be anagrams
   if (length1 != length2) {
      return false;
   }

   for (int i = 0; i < length1; i++) {
      if (isalpha(word1[i])) {
         int index = tolower(word1[i]) - 'a';
         count1[index]++;
      }
   }

   for (int i = 0; i < length2; i++) {
      if (isalpha(word2[i])) {
         int index = tolower(word2[i]) - 'a';
         count2[index]++;
      }
   }

   // Compare the count of each letter in both words
   for (int i = 0; i < 26; i++) {
      if (count1[i] != count2[i]) {
         return false;
      }
   }

   return true;
}

bool areDisjoint(const char* word1, const char* word2) {
   int count[26] = { 0 }; // Array to count the occurrences of each letter

   int length1 = strlen(word1);
   int length2 = strlen(word2);

   for (int i = 0; i < length1; i++) {
      if (isalpha(word1[i])) {
         int index = tolower(word1[i]) - 'a';
         count[index]++;
      }
   }

   for (int i = 0; i < length2; i++) {
      if (isalpha(word2[i])) {
         int index = tolower(word2[i]) - 'a';
         // If any letter from word2 has occurred in word1, they are not disjoint
         if (count[index] > 0) {
            return false;
         }
      }
   }

   return true;
}

bool isHeterogram(const char* word) {
   int count[26] = { 0 }; // Array to count the occurrences of each letter

   int length = strlen(word);
   for (int i = 0; i < length; i++) {
      if (isalpha(word[i])) {
         int index = tolower(word[i]) - 'a';

         // If the letter has already been counted before, it's not a heterogram
         if (count[index] > 0) {
            return false;
         }

         count[index]++;
      }
   }

   return true;
}

bool isPalindrome(const char* word) {
   int length = strlen(word);
   int i = 0;
   int j = length - 1;

   while (i < j) {
      while (!isalpha(word[i])) {
         i++;
      }
      while (!isalpha(word[j])) {
         j--;
      }

      if (tolower(word[i]) != tolower(word[j])) {
         return false;
      }

      i++;
      j--;
   }

   return true;
}


// =========================================

// ==<End_Of_File>==================== //   
