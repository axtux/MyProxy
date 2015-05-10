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

int connection_handler(void *);

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


int connection_handler(void *socket) {
  int close_client = 0, client = *(int*)socket;
  int server = -1;
  
  char request[BUFF_SIZE], response[BUFF_SIZE], requests[BUFF_SIZE], buffer[BUFF_SIZE];
  int request_size = 0, response_size = 0, requests_size = 0, buffer_size = 0;
  
  char *host = 0, *uri = 0, *cache_file = 0;
  
  buffer_size = recv(client, buffer, BUFF_SIZE, 0);
  while(buffer_size > 0) {
    // copy buffered data to requests
    if(requests_size+buffer_size > BUFF_SIZE) {
      return http_header(client, "413 Request Entity Too Large", 1);
    }
    memcpy(&requests[requests_size], buffer, buffer_size);
    requests_size += buffer_size;
    
    while(extract_first_request(requests, &requests_size, request, &request_size) == 0) {
      host = extract_host(request, request_size);
      uri = extract_uri(request, request_size);
      close_client = mempos(request, "Connection: close\r\n", request_size) +1;
      
      if(host && uri) {
        server = conn_socket(host, HTTP_PORT);
        if(server < 0) {
          http_header(client, "502 Bad Gateway (1)", close_client);
        } else {
          printf("Connected to %s successfully.\n", host);
          
          cache_file = cache_filename(host, uri);
          remove_header(request, &request_size, "If-Modified-Since");
          remove_header(request, &request_size, "Connection");
          add_cache_header(request, &request_size, BUFF_SIZE, http_time(cache_file));
          add_close_header(request, &request_size, BUFF_SIZE);
          /* print new request
          request[request_size] = 0;
          printf("--- REQUEST ---\n%s", request);
          //*/
          if(send(server, request, request_size, 0) != request_size) {
            http_header(client, "502 Bad Gateway (2)", close_client);
          } else {
            response_size = 0;
            while( (buffer_size = recv(server, buffer, BUFF_SIZE, 0)) > 0) {
              if(response_size+buffer_size > BUFF_SIZE) {
                response_size = buffer_size = 0;
                break;
              }
              memcpy(&response[response_size], buffer, buffer_size);
              response_size += buffer_size;
            }
            if(buffer_size < 0) {
              printf("Error receiving data from server.\n");
              http_header(client, "502 Bad Gateway (3)", close_client);
            } else { // buffer_size == 0
              printf("Connection closed by server.\n");
              if(response_size > 0) {
                switch(extract_http_code(response, response_size)) {
                  case 304: // Not Modified / Try to get file from cache
                    buffer_size = file_get_contents(cache_file, buffer, BUFF_SIZE);
                    if(buffer_size > 0) {
                      printf("Got 304, sending data from cache file %s.\n", cache_file);
                      memcpy(response, buffer, buffer_size);
                      response_size = buffer_size;
                    } else {
                      printf("Got 304 but no cache file %s.\n", cache_file);
                    }
                  case 200: // OK / write to cache
                    file_put_contents(cache_file, response, response_size);
                  default:
                    /*
                    response[response_size] = 0;
                    printf("--- RESPONSE ---\n%s", response);
                    //*/
                    if(send(client, response, response_size, 0) != response_size) {
                      printf("Error sending data back to the client.\n");
                      close(client);
                    } else {
                      printf("Data sent back to the client.\n");
                      if(close_client) {
                        printf("Closing client connection.\n");
                        close(client);
                      }
                    }
                }
              } else { // response_size == 0
                http_header(client, "502 Bad Gateway (4)", close_client);
              }
            }
          }
          close(server);
        }
        free(host);
        free(uri);
      } else {
        return http_header(client, "400 Bad Request", 1);
      }
    }
    buffer_size = recv(client, buffer, BUFF_SIZE, 0);
  }
  
  printf("Connection with client closed\n");
  close(client);
}




