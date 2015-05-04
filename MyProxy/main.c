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
void *connection_handler(void *);
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
      	     continue;
  	   }
	   pthread_t sniffer_thread;
	   new_sock = malloc(1);
           *new_sock = client_socket;
	  
	  printf("client connecté avec succès.\n");
          printf("Connection accepted from %d.%d.%d.%d:%d\n", client_address.sin_addr.s_addr/(256*256*256)%256, client_address.sin_addr.s_addr/(256*256)%256, client_address.sin_addr.s_addr/(256)%256, client_address.sin_addr.s_addr%256, client_address.sin_port%65536);

	   
        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("could not create thread");
            return 1;
        }
         
    }
    //close(client_socket);
    close(tcp_socket);
  return 0;
}


void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[BUFF_SIZE];
     
    //Send some messages to the client
    message = "You are connected\n";
    //write(sock , message , strlen(message));
     
    //message = "Now type something and i shall repeat what you type \n";
    //write(sock , message , strlen(message));
     
    //Receive a message from client
    while( (read_size = recv(sock , client_message , BUFF_SIZE , 0)) > 0 )
    {
        //end of string marker
		client_message[read_size] = '\0';
		
		//Send the message back to client
        send(sock , client_message , strlen(client_message), 0);
	//send(adrFamille, message, BUFF_SIZE, 0);
		
		//clear the message buffer
      		//client_message[BUFF_SIZE];
		memset(client_message, '\0', BUFF_SIZE);
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
         
    return ;
} 

