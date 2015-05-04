#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>

int get_int(char string[], int error);
char* str_addr(struct sockaddr_in address);

#endif // FUNCTIONS_H_INCLUDED
