/*
Portscanner MAIN Function
27-03-23
Author: u$3rH0N3$T
*/
// C-Libs//
#include <stdio.h>
#include <stdlib.h>
// Headers
#include "portscanner.h"
#define MAX_PORT 65535
#define TIMEOUT 5

int ScanPorts(char *hostname, int startPort, int endPort);

int main(int argc, char *argv[]){
	
	char *hostname;
	int startPort = 1;
	int endPort = MAX_PORT;

	
	

	if (argc < 2){
		fprintf(stderr, "Usage: %s <hostname> [starPort] [endPort]\n", argv[0]);
	
	}

	hostname = argv[1];

	if (argc > 2){
		startPort = atoi(argv[2]);
	
	}

	if (argc > 3){
		endPort = atoi(argv[3]);	
	
	}

	return ScanPorts(hostname, startPort, endPort);
	

}

// ----------End of Program---------//

