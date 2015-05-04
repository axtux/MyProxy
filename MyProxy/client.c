#
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

#define SIZEBUF 256 //taille buffer

void main(int argc, char *argv[]){
	
	int port= get_int(argv[1], 0);
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
