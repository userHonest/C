/*-------------------------------
Portscanner ScanPorts Function
27-03-23
Author: u$3rH0N3$T
-----------------------------*/
// C-Libs//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
// Headers //
#include "portscanner.h"

int ScanPorts(char *hostname, int startPort, int endPort){

	struct hostent *host;
	struct sockaddr_in sa;
	int i , eSock;
	int ret;
	char hostNameBuff[256];

	strncpy(hostNameBuff, hostname, 255);
	hostNameBuff[255] = '\0';

	host = gethostbyname(hostNameBuff);
	if (host == NULL) {
		fprintf(stderr, "Error: Unable to resolve hostname %s\n", hostNameBuff);
		return 1;

	}

	memset (&sa, 0, sizeof(sa));
	sa.sin_family = host->h_addrtype;
	memcpy(&sa.sin_addr.s_addr, host->h_addr, host->h_length);
	
	for (i = startPort; i <= endPort; i++){
		sa.sin_port = htons(i);
		eSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		ret = connect(eSock, (struct sockaddr*) &sa, sizeof(sa));

		if (ret == 0){
			struct servent *service = getservbyport(htons(i), "tcp");
			if (service != NULL){
				printf("Port %d (%s) is open\n", i, service->s_name);			
			
			} 
			else {
				printf("Port %d is open\n", i);
			}
			
			close(eSock);

		}
		else {

			close(eSock);
		}
		
	}
	
	return 0;
}


// ------------ End of Program ----------------//
