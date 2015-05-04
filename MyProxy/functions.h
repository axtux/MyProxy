#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <regex.h>
#include <string.h>

int get_int(char string[], int error);
char* str_addr(struct sockaddr_in address);
void mem_copy(char* src, char* dst, int dst_start, int length);
void mem_copyy(char* src, char* dst, int src_start, int length);
char *regmatch(char *str_request, char *str_regex);

#endif // FUNCTIONS_H_INCLUDED
