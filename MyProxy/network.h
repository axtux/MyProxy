#ifndef NETWORK_H_INCLUDED
#define NETWORK_H_INCLUDED

#include "functions.h"

#define BUFF_SIZE 1024
#define MIN_PORT 0
#define MAX_PORT 65535
#define MAX_CONN 20
#define HTTP_PORT 80

int init_socket(int port);
int conn_socket(char* hostname);

#endif // NETWORK_H_INCLUDED
