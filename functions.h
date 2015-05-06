#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define MAXSIZE 256

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <regex.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>


int get_int(char string[], int error);
char* str_addr(struct sockaddr_in address);
void mem_copy(char* src, char* dst, int dst_start, int length);
void mem_copyy(char* src, char* dst, int src_start, int length);
char *regmatch(char *str_request, char *str_regex);
char* getdate(char *filname);
void ecrireFichier(char texte[], char nom[]);
char* get_host(char* headers);

#endif // FUNCTIONS_H_INCLUDED
