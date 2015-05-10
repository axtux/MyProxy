#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

#define MAXSIZE 256
#define CACHE_PATH ./cache_files
#define HTTP_TIME_LENGTH 29

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
int strpos(char *str, char *rch);
int mempos(char *mem, const char *str, int max_mem);
char *cache_filename(char* host, char* uri);
char *regmatch(char *str_request, char *str_regex);
char *http_time(char *filename);
void remove_header(char *request, int *length, char *header);
void add_cache_header(char *request, int *length, int max_length, char *time);
void add_close_header(char *request, int *length, int max_length);
int file_put_contents(char *filename, char *content, int length);
int file_get_contents(char *filename, char *content, int max_length);
int extract_first_request(char *requests, int *requests_size, char *request, int *request_size);
char* extract_host(char* request, int length);
char* extract_uri(char* request, int length);
char* extract_header(char* request, int length, char *header);
int extract_http_code(char *response, int response_size);

#endif // FUNCTIONS_H_INCLUDED
