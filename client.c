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

#define SIZEBUF 256 //taille buffer

void main(int argc, char *argv[]){
  int port;
  if(argc < 2) {
    printf("Missing port number. Usage : %s server port\n", argv[0]);
    return -1;
  } else {
    port = get_int(argv[1], MIN_PORT-1);
    if(port < MIN_PORT || port > MAX_PORT) {
      printf("Invalid port number. Port must range from %d to %d.\n", MIN_PORT, MAX_PORT);
      return -2;
    }
  }
  
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
        
  int result;
  char message[SIZEBUF];
  char buffer[SIZEBUF];
  
  //recv(adrFamille, buffer, SIZEBUF, 0);
  while(1){
    //send
    int resultmes = 0;
    resultmes = scanf("%s",message);
    int ssize = send(adrFamille, message, SIZEBUF, 0);
    //printf("%s\n",message);
    if(ssize ==0 ){
      printf("problem send cote client\n");
    
      close(adrFamille);
      exit(0);
    }
    int nbrOctet = recv(adrFamille, buffer, SIZEBUF, 0);
    //printf("%s\n",buffer);
    puts(buffer);
    memset(buffer, '\0', SIZEBUF);
    
  }
  //close
  result = close(adrFamille);
  printf("client fini\n");
  exit(0);
}
