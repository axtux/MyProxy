#include <stdio.h>
#include "network.h"


#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

int get_int(char string[], int error);

int main(int argc, char* argv[]) {
  char buffer[BUFF_SIZE];
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
  int client_socket;
  struct sockaddr_in client_address;
  int client_address_size = sizeof(client_address);
  //int client_address_size = sizeof client_address;
  while(1) {
    client_socket = accept(tcp_socket, (struct sockaddr *) &client_address, &client_address_size);
    if (client_socket == -1) {
      printf("Error accepting first client.\n");
      continue;
    }
    printf("Connection accepted from %d.%d.%d.%d:%d\n", client_address.sin_addr.s_addr/(256*256*256)%256, client_address.sin_addr.s_addr/(256*256)%256, client_address.sin_addr.s_addr/(256)%256, client_address.sin_addr.s_addr%256, client_address.sin_port%65536);
    if(((struct sockaddr*)&client_address)->sa_family == AF_INET) {
      printf("Internet IPv4 address.\n");
    }
    
    int addr = client_address.sin_addr.s_addr;
    printf("Client connected from %d.%d.%d.%d:%d.\n", addr/(256*256*256), addr/(256*256)%256, addr/256%256, addr%256, client_address.sin_port);
    close(client_socket);
  }

  close(tcp_socket);

  return 0;
}

int get_int(char string[], int error) {
  int result = 0, i = 0;
  while(string[i] != '\0') {
    if(string[i] < '0' || string [i] > '9') {
      return error;
    } else {
      result = 10*result+string[i]-'0';
      ++i;
    }
  }
  return result;
}


