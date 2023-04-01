/*
Portscanner headerfile
27-03-23
Author: u$3rH0N3$T
*/
#ifdef PORTSCANNER_H
#define PORTSCANNER_H

#include <netdb.h>
#include <netinet.h>
#include <sys/socket.h>


#define MAX_PORT 65535
#define TIMEOUT 5


int ScanPorts(char *hostname, int startPort, int endPort);

#endif

//------------End-------------//

