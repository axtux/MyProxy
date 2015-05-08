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

int main(int argc, char *argv[]){
  char request[BUFF_SIZE+1], buffer[BUFF_SIZE+1];
  int port, request_size = 0, buffer_size = 0;
  
  if(argc < 3) {
    printf("Not enough arguments. Usage : %s server port [persistance [pipelining]]\n", argv[0]);
    return -1;
  } else {
    port = get_int(argv[2], MIN_PORT-1);
    if(port < MIN_PORT || port > MAX_PORT) {
      printf("Invalid port number. Port must range from %d to %d.\n", MIN_PORT, MAX_PORT);
      return -2;
    }
  
    sprintf(request, "GET /toi HTTP/1.1\r\nHost: %s\r\n%s\r\n", argv[1], (argc >= 4 && argv[3][0] == '1') ? "" : "Connection: close\r\n");
    request_size = strlen(request);
    printf("Made request:%s", request);
  }
  
  int server = conn_socket("127.0.0.1", port);
  if(server < 0) {
    printf("Error connecting to proxy server.\n");
  } else {
    printf("Connected to proxy server successfully.\n");
    if(send(server, request, request_size, 0) != request_size) {
      printf("Error sending data to server.\n");
    } else {
      do {
        buffer_size = recv(server, buffer, BUFF_SIZE, 0);
        if(buffer_size < 0) {
          printf("Error receiving data from server.\n");
        } else if(buffer_size == 0) {
          printf("Connection closed by server.\n");
        } else {
          buffer[buffer_size] = '\0';
          printf("Data received :%s", buffer);
        }
      } while(buffer_size > 0);
    }
    close(server);
  }
  /*
  int result;
  char message[SIZEBUF];
  result = scanf("%s",message);
  //*/
  return 0;
}
