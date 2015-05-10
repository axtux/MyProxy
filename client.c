#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#include "network.h"
#include "functions.h"

char *make_request(char *host, char *uri);

int main(int argc, char *argv[]){
  char *request, buffer[BUFF_SIZE+1];
  int server, port, request_size = 0, buffer_size = 0, persistance = 0, pipelining = 0, i = 0;
  
  if(argc < 6) {
    printf("Not enough arguments. Usage : %s server port persistance pipelining uri1 [uri2 [...]]\n", argv[0]);
    return -1;
  } else {
    port = get_int(argv[2], MIN_PORT-1);
    if(port < MIN_PORT || port > MAX_PORT) {
      printf("Invalid port number. Port must range from %d to %d.\n", MIN_PORT, MAX_PORT);
      return -2;
    }
    
    if(argv[3][0] == '1') {
      persistance = 1;
    }
    if(persistance && argv[4][0] == '1') {
      pipelining = 1;
    }
  }
  printf("pers=%d, pipe=%d\n", persistance, pipelining);
  
  if(persistance) {
    server = conn_socket("127.0.0.1", port);
    if(server < 0) {
      printf("Error connecting to proxy server.\n");
      return -3;
    } else {
      printf("Connected to proxy server successfully.\n");
    }
  }
  
  for(i = 5; i < argc; ++i) {
    if(!persistance) {
      server = conn_socket("127.0.0.1", port);
      if(server < 0) {
        printf("Error connecting to proxy server.\n");
        return -3;
      } else {
        printf("Connected to proxy server successfully.\n");
      }
    }
    
    request = make_request(argv[1], argv[i]);
    request_size = strlen(request);
    
    if(!persistance || i == argc-1) {
      add_close_header(request, &request_size, BUFF_SIZE);
      request[request_size] = '\0';
    }
    
    printf("\n--- REQUEST ---\n\n%s", request);
    
    if(send(server, request, request_size, 0) != request_size) {
      printf("Error sending data to server.\n");
      return -4;
    }
    if(!pipelining) {
      //do {
        buffer_size = recv(server, buffer, BUFF_SIZE, 0);
        if(buffer_size < 0) {
          printf("Error receiving data from server.\n");
        } else if(buffer_size == 0) {
          printf("Connection closed by server.\n");
        } else {
          buffer[buffer_size] = '\0';
          printf("\n--- RESPONSE ---\n\n%s", buffer);
        }
      //} while(buffer_size > 0);
    }
    if(!persistance) {
      close(server);
      printf("Closed proxy connection.\n");
    }
  }
  
  if(pipelining) {
    do {
      buffer_size = recv(server, buffer, BUFF_SIZE, 0);
      if(buffer_size < 0) {
        printf("Error receiving data from server.\n");
      } else if(buffer_size == 0) {
        printf("Connection closed by server.\n");
      } else {
        buffer[buffer_size] = '\0';
        printf("\n--- RESPONSE ---\n\n%s", buffer);
      }
    } while(buffer_size > 0);
  }
  
  if(persistance) {
    close(server);
  }
  
  return 0;
}

char *make_request(char *host, char *uri) {
  char *request = malloc(sizeof(*request) * BUFF_SIZE);
  sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", uri, host);
  return request;
}

