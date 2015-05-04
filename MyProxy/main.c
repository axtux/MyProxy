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

char* request = "GET / HTTP/1.1\r\nHost: www.perdu.com\r\nConnection: close\r\n\r\n";
int p = 0; // delete me

//int get_int(char string[], int error);
void connection_handler(void *);

int main(int argc, char* argv[]) {
  int port;
  // Arguments checks
  if(argc < 2) {
    printf("Missing port number. Usage : %s port\n", argv[0]);
    return -1;
  } else {
    p = port = get_int(argv[1], MIN_PORT-1);
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
  int sock = *(int*)socket;
  int client_size, server_size;
  char client_buffer[BUFF_SIZE+1], server_buffer[BUFF_SIZE+1];
  char *host = 0, *uri;
  
  char request[BUFF_SIZE+1];
  int request_size = 0;
  
  sprintf(client_buffer, "Proxy: Connected successfully to me !!!\n");
  send(sock, client_buffer, strlen(client_buffer), 0);
  
  //Receive a message from client
  do {
    client_size = recv(sock, client_buffer, BUFF_SIZE, 0);
	  client_buffer[client_size] = '\0';
	  
	  if(request_size+client_size > BUFF_SIZE) {
	    printf("No host found in first %d chars. Closing connection.", BUFF_SIZE);
	    close(sock);
	    return;
	  }
	  mem_copy(client_buffer, request, request_size, client_size);
	  request_size += client_size;
	  request[request_size] = '\0';
	  /*
	  if(host == 0) {
	    host = regmatch(request, "\r\nhost *: *([\\.A-Za-z]+)\r\n");
	    if(host) {
	      printf("--- Host found");
	      printf("%s", host);
	      printf("---");
	    }
	  }
	  //*/
	  request[request_size-2] = '\0'; // remove \r\n
	  int ssock = conn_socket(request, 80); // "208.97.177.124", HTTP_PORT);
    if(ssock < 0) {
      printf("Error connecting host.\n");
      exit(-1);
    } else {
      printf("Connected host successfully.\n");
    }
	  do {
      if(send(ssock, request, strlen(request), 0) < 0) {
          perror("send()");
          exit(-2);
      }
	    server_size = recv(ssock, server_buffer, BUFF_SIZE, 0);
	    server_buffer[server_size] = '\0';
	    printf(server_buffer);
	  } while(server_size > 0);
	  close(ssock);
	  //*/
	  //Send the message back to client
    send(sock, client_buffer, client_size, 0);
	
	  //clear the message buffer
	  //memset(client_buffer, '\0'char* str_addr(struct sockaddr_in address), BUFF_SIZE);
  } while(client_size > 0);
  
  printf("Connection closed by cient\n");
  close(sock);
}




