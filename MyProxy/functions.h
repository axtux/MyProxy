#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define MAXSIZE 256
#define BUFF_SIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>

int get_int(char string[], int error);
char* str_addr(struct sockaddr_in address);
char* getdate(char *filname);
void ecrireFichier(char texte[], char nom[]);

#endif // FUNCTIONS_H_INCLUDED
