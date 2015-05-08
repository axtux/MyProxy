#include <stdio.h>
//#include <stdlib.h>
#include "network.h"
#include "functions.h"


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

//int get_int(char string[], int error);
void connection_handler(void *);

int main(int argc, char* argv[]) {
  int port;
  // Arguments checks
  if(argc < 2) {
    printf("Missing port number. Usage : %s port\n", argv[0]);
    return -1;
  } else {
    port = get_int(argv[1], MIN_PORT-1);
    if(port < MIN_PORT || port > MAX_PORT) {
      printf("Invalid port number. Port must range from %d to %d.\n", MIN_PORT, MAX_PORT);
      return -2;
    }
  }
  // Initiating socket
  int tcp_socket = init_socket(port);
  if(tcp_socket < 0) {
    printf("Error initiating socket.\n");
    return -3;
  }
  
  printf("Server proxy listening on port %d.\n", port);
  // Accept clients connections
  int client_socket, *new_sock;
  struct sockaddr_in client_address= { 0 };
  int client_address_size = sizeof client_address;
  while(1) {
    client_socket = accept(tcp_socket, (struct sockaddr *) &client_address, &client_address_size);
    if (client_socket == -1) {
       printf("Error accepting client.\nMake sure port %d is not in use.\n", port);
       return -4;
    }
    pthread_t sniffer_thread;
    
    printf("Connection accepted from %s.\n", str_addr(client_address));
    if(pthread_create(&sniffer_thread, NULL,  connection_handler, (void*) &client_socket) != 0) {
        perror("could not create thread");
        return -5;
    }
  }
  close(tcp_socket);
  return 0;
}


void connection_handler(void *socket) {
  int client = *(int*)socket;
  int server = -1;
  
  char request[BUFF_SIZE], requests[BUFF_SIZE], buffer[BUFF_SIZE];
  int request_size = 0, requests_size = 0, buffer_size = 0;
  
  char *host = 0, *uri = 0;
  
  do {
    buffer_size = recv(client, buffer, BUFF_SIZE, 0);
    // copy buffered data to requests
    if(requests_size+buffer_size > BUFF_SIZE) {
      sprintf(buffer, "413 Request Entity Too Large (max %d chars)\r\n\r\n", BUFF_SIZE);
      send(client, buffer, strlen(buffer), 0);
      printf(buffer);
      close(client);
      return;
    }
    memcpy(&requests[requests_size], buffer, buffer_size);
    requests_size += buffer_size;
    
    while(extract_first_request(requests, &requests_size, request, &request_size) == 0) {
      host = extract_host(request, request_size);
      uri = extract_uri(request, request_size);
      //host = regmatch(requests, "\r\nhost *: *([\\.A-Za-z]+)\r\n");
      if(host) {
        server = conn_socket(host, HTTP_PORT);
        if(server < 0) {
          printf("502 Bad Gateway\r\nError connecting to %s.\n", host);
        } else {
          printf("Connected to %s successfully.\n", host);
          if(send(server, request, request_size, 0) != request_size) {
            printf("Error sending data to server.");
          } else {
            do {
              buffer_size = recv(server, buffer, BUFF_SIZE, 0);
              if(buffer_size < 0) {
                printf("Error receiving data from server.");
              } else {
                if(send(client, buffer, buffer_size, 0) != buffer_size) {
                  printf("Error sending data back to the client.");
                } else {
                
                }
              }
            } while(buffer_size > 0);
          }
          close(server);
        }
        free(host);
      } else {
        printf("Host not found within request.");
      }
    }
    
  } while(buffer_size > 0);
  
  printf("Connection closed by cient (buffer_size=%d)\n", buffer_size);
  close(client);
}




