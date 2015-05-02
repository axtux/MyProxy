

int init_socket(int port) {
  int tcp_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // IPv4 TCP Socket
  if(tcp_socket == -1) {
    printf("Error opening socket.\n");
    return -1;
  }

  struct sockaddr_in server_address;
  server_address.sin_addr.s_addr = htonl(INADDR_ANY);
  server_address.sin_family = AF_INET; // IPv4
  server_address.sin_port = htons(port);

  if(bind(tcp_socket, (struct sockaddr *) &server_address, sizeof server_address) == -1) {
    printf("Error binding socket.\n");
    return -2;
  }

  if(listen(tcp_socket, MAX_CONN) == -1) {
    printf("Error listening socket.\n");
    return -3;
  }

  return tcp_socket;
}
