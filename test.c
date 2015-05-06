#include "functions.h"

void main(int argc, char** argv) {
  char *request = "GET / HTTP/1.1\r\nHost: www.google.be\r\nConnection: close\r\n\r\n";
  printf(get_host(request));
}

