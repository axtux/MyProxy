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
    printf("Missing port number. Usage : %s port\n", argv[0]);
    return -1;
  } else {
    port = get_int(argv[1], MIN_PORT-1);
    if(port < MIN_PORT || port > MAX_PORT) {
      printf("Invalid port number. Port must range from %d to %d.\n", MIN_PORT, MAX_PORT);
      return -2;
    }
  }
  
	int result;
	char message[SIZEBUF];
	char buffer[SIZEBUF];

	//socket
	int adrFamille = socket(AF_INET, SOCK_STREAM, 0);
	if(adrFamille < 0){
		printf("problem socket cote client\n");
		exit(0);
	}
	//connect
	struct sockaddr_in sadr;
	sadr.sin_family = AF_INET;
	sadr.sin_port = htons(port);
	sadr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	result = connect(adrFamille, (struct sockaddr *)&sadr, sizeof(sadr));
	if(result != 0){
		printf("problem connect cote client\n");
		close(adrFamille);
		exit(0);
	}
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
