#include <stdio.h>
//#include <stdlib.h>
#include "network.h"


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
  printf("Server proxy listening on port %d.\n", port);
  // Accept clients connections
  int client_socket, *new_sock;
  struct sockaddr_in client_address= { 0 };
  int client_address_size = sizeof client_address;
  while(1) {
    client_socket = accept(tcp_socket, (struct sockaddr *) &client_address, &client_address_size);
    if (client_socket == -1) {
       printf("Error accepting client.\n");
       return -3;
    }
    pthread_t sniffer_thread;
    
    printf("Connection accepted from %d.%d.%d.%d:%d\n", client_address.sin_addr.s_addr/(256*256*256)%256, client_address.sin_addr.s_addr/(256*256)%256, client_address.sin_addr.s_addr/(256)%256, client_address.sin_addr.s_addr%256, client_address.sin_port%65536);
    if(pthread_create(&sniffer_thread, NULL,  connection_handler, (void*) &client_socket) != 0) {
        perror("could not create thread");
        return -4;
    }
  }
  close(tcp_socket);
  return 0;
}


void connection_handler(void *socket) {
  int sock = *(int*)socket;
  int read_size;
  char *message, client_message[BUFF_SIZE+1];
   
  //Send some messages to the client
  message = "You are connected\n";
   
  //Receive a message from client
  do {
    read_size = recv(sock, client_message, BUFF_SIZE, 0);
	  client_message[read_size] = '\0';
	
	  //Send the message back to client
    send(sock , client_message , read_size, 0);
	
	  //clear the message buffer
	  //memset(client_message, '\0', BUFF_SIZE);
  } while(read_size > 0);
  
  printf("Connection closed by cient\n");
  close(sock);
} 

char* cache_name(char* host, char* uri) {
  int size = 0;
  int i = 0;
  
  while(host[i] != '\0') {
    
  }
}

