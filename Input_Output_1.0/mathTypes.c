/*=============================================
Eksam PG3401 C Programming for Linux , Task 2
Source: mathTypes.c source file
Description: ONE source file for all functions to be called upon in main
Author: Candidate 1087 
Version 1.0 
==============================================*/

// Libraries //
#include <stdbool.h>
#include <math.h>

// headers //
#include "mathTypes.h"
#include "mathTypeFunctions.h"

//define for the amout if integers to add
// for function PrintToBinary
#define MAX 32

// functions //
/////////////////////////////////////////////////////////////////////////////////////////
//  this function takes 3 parameters , pOutoutfile, iNum, iMinDigtis 
//   iBin is an array of integers with max size 32, the integeres is to keep track
// the function enters a while loop that repeatdly divides the decimal number iNUm by 2
// The loop continues until iNum is less than or equal to 0.
// the pOutputFile prints out the string and indicates that the output will be binary
// iMinDigits specifies the numbers of ceros added to the number printed as binary
// To finalize  the function enters another for loop that prints each binary digit 
//  stored in the iBin //array to the output file starting from the most significant 
//  digitto the last element in the array and  prints each digit as an integer 
//  with a 1 or 0 using the fprintf() function.
////////////////////////////////////////////////////////////////////////////////////////////

void PrintToBinary(FILE *pOutputFile, int iNum, int iMinDigits){
    int iBin[MAX]; // defined 
    int i = 0;
    
    while (iNum > 0){
        iBin[i] = iNum % 2;
        iNum = iNum / 2;
        i++;
    }
    
    fprintf(pOutputFile, "Binary: ");
    
    for (int j = 0; j < iMinDigits - i; j++){ // if not work ,deelete this 
        fprintf(pOutputFile, "0");
    }
    
    // printing each binary digit stored in iBin
    for (int j = i - 1; j >= 0; j--){
        fprintf(pOutputFile, "%d", iBin[j]);
    }
    fprintf(pOutputFile, "\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
//  The reason i implemented this function it that, the binary number printed out was missing
//  a few ceroes, wich can confuse , so i hadded this function that sets the minium binary digits
//  to 8 which mye be sen as a default value.
//  The func takes one parameter, that is a pointer to a input file, that contains the list 
//  of numbers .After finding the max decimal number, the function calculates the number 
//  of binary digits required to represent that number plus one using the <log2> function from 
//  the math library, and rounds the result up to the nearest integer using the <ceil> function. 
//  This is done to ensure that all binary numbers in the file will have at least this many digits 
//  when they are converted to binary.
/////////////////////////////////////////////////////////////////////////////////////////////////////////

int MinBinaryDigits(FILE *pInputFile){
    int iMaxNumber = 0;
    int iNumbers;
    int iMaxDigits = (int)ceil(log2(iMaxNumber + 1)); // using log2 function from math library
    int iMinDigits = 8; // minimun digits //
    
    
    while (fscanf(pInputFile, "%d", &iNumbers) !=EOF){
        if (iNumbers > iMaxNumber){
            iMaxNumber = iNumbers;
        }
    }
    
    // using rewind function
    //  so the file can be read again by other funtions
    // since the binary is at the very start
    rewind(pInputFile); 
    
    return iMinDigits;

}

////////////////////////////////////////////////////////////////////////////////////////////
// this function is to print out the decimal numbers from input , out as hexadecimals
// this functionality is optional. Next, the function enters a while loop that repeatedly 
// divides the decimal number <iNumb> by 16 and stores the remainder in the <iHexNumbers> 
// array starting at index 0.  However, instead of printing digits 10 to 15 as decimal 
// integers, they correspond to hexadecimal digits A to F, it prints them as the 
// corresponding letters <A> to <F> using a series of if-else statements. 
// 

void PrintToHex(FILE *pOutputFile, int iNumb){
    int x = 0;
    int iHexNumbers[100];
    
    // here it calculates // 
    while (iNumb != 0){
        iHexNumbers[x] = iNumb % 16;
        iNumb = iNumb / 16;
        x++;
    }
    
    fprintf(pOutputFile, "Hexadecimal: ");
    for (int f = x - 1; f >=0; f--){
        if (iHexNumbers[f] == 10){
            fprintf(pOutputFile, "A");
        } 
        else if (iHexNumbers[f] == 11){
            fprintf(pOutputFile, "B");
        }
        else if (iHexNumbers[f] == 12){
            fprintf(pOutputFile, "C");
        }
        else if (iHexNumbers[f] == 13){
            fprintf(pOutputFile, "D");
        }
        else if (iHexNumbers[f] == 14){
            fprintf(pOutputFile, "E");
        }
        else if (iHexNumbers[f] == 15){
            fprintf(pOutputFile, "F");
        }
        else {
            fprintf(pOutputFile, "%d", iHexNumbers[f]);
        }
        
        
    }
    
    fprintf(pOutputFile, "\n");

}
// abundant numbers function provided  

bool isAbundantNumber(int n) {
   int iSum = 0;
   for (int i = 1; i < n; i++) {
      if (n % i == 0) {
         iSum += i;
      }
   }
   return (iSum > n);
}

//---------------------------------//
// function to compute a cubic root

bool isCubeNumber(int n) {
   int iCube_n = (int)cbrt(n); // expresion cbrt calculates the cube root of the number.
   return (iCube_n * iCube_n * iCube_n == n);
}

// -------------------------------------//

// function deficient number
bool isDeficientNumber(int n) {
   int iSum = 0;
   for (int i = 1; i < n; i++) {
      if (n % i == 0) {
         iSum += i;
      }
   }
   return (iSum < n);
}

// ------------------------------//

// Fibonacci //
bool isFibonacci(int n) {
   int a = 0, b = 1, c = a + b;
   while (c < n) {
      a = b;
      b = c;
      c = a + b;
   }
   return (c == n);
}

//-----------------------------//

// function is square //

bool isSquareNumber(int n) {
   int iSqrt_n = (int)sqrt(n); // expressin sqrt calculaters the sware root of a numbers
   return (iSqrt_n * iSqrt_n == n);
}

// -----------------------------//

//function to check if number is odd

bool isOddNumber(int n) {
   return (n % 2 != 0);
}

//-----------------------//

// function to to check if number is are equal
bool isPerfectNumber(int n) {
   int iSum = 0;
   for (int i = 1; i < n; i++) {
      if (n % i == 0) {
         iSum += i;
      }
   }
   return (iSum == n);
}

// -------------------------//

// function to check if number is prime

bool isPrimeNumber(int n) {
   if (n <= 1) return false;
   for (int i = 2; i < n; i++) {
      if (n % i == 0) {
         return false;
      }
   }
   return true;
}

// Function to print out column lines in the outputfile 
void PrintSeparator(FILE *pszOutputFilename){
    for (int i = 0; i < 45; i++){
        fprintf(pszOutputFilename, "-");
    
    }
    fprintf(pszOutputFilename, "\n\n");
    

}
// --------------------------------------------------------//


/* ============== End of File  ========================= */

