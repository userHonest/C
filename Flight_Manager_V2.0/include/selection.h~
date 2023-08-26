//============================================== // 
//  Retake_Exam PGR3401 C-Programming for Linux
//  Teacher: Bengt Østby
//  -----
//  Task 3
//  Version 1.1
//  Sourcefile: selection.h
//  Author: Kandidate_nr: 2019
//  Patch_Date: 04/08/2023
//===============================================//
#ifndef SELECTION_H
#define SELECTION_H

// libraries // 
#include <stddef.h>
#include <stdbool.h>

// 
typedef struct Passenger Passenger;

/////////////////////////////////////////////////////////////
//   Flight structure that is used in whe whole program
//   all the data relaed to add a flight, delete, display 
//  is from this structure 
/////////////////////////////////////////////////////////////

typedef struct Flight{
    char szFlight_ID[20];
    char szDestination[50];
    int iSeats;
    int iDepartureHour;
    int iDepartureMinute;
    int iBookedSeats;
    Passenger* pPassengers;
    struct Flight *pNext;
    struct Flight *pPrev;
    int iIsDelayed; 
    
}Flight;


/////////////////////////////////////////////////////////////////
//  Passenger data for reservations are added here, these 
//  are the used to handle multiple flights and store that data 
/////////////////////////////////////////////////////////////////

typedef struct Passenger{
    int iSeatNumber;
    char szName[50];
    int iAge;
    struct Passenger* pNext;

}Passenger;

void PrintScreen(void);
void AddFlight(Flight** ppFlights, const char* szFlight_ID, const char* szDestination, int iSeats, int iDepartureHour, int iDepartureMinute);
void ShowFlightData(const Flight* flight);
void AddPassenger(Flight* flights, const char* szFlight_ID, int iSeatNumber, const char* szName, int iAge);
Flight* GetFlight(Flight* flights, const char* szFlight_ID);
Flight* GetFlightTime(Flight* flights, int iDepartureHour, int iDepartureMinute);
void DeleteFlight(Flight** ppFlights, const char* szFlight_ID);
void DeletePassenger(Flight **flights, char *szName, char *szFlight_ID);
bool ChangeSeat(Flight **flights, char *szFlight_ID, char *szName, int iNewSeat);
void SearchPassenger(Flight *flights, const char *szName);

int UpdateFLightDepartureTime(Flight* flights, const char* szFlight_ID, int iNewDepartureHour, int iNewDepartureMinute);
void ClearBuff();
int CountFLights(Flight* flights, const char* szDestination);

#endif

