#include "functions.h"

#define BUFF_SIZE 2048

void main(int argc, char** argv) {
  /*
  char requests[200], request[100];
  sprintf(requests, "Coucou toi\r\nBonjour sinon\r\n\r\nSeconde requete\r\n\r\nTroisieme requete\r\nBeaucoup plus longue cette fois !\r\nEncore plus longue...\r\n\r\n");
  int request_size = 0, requests_size = strlen(requests);
   int i = 0;
  while(extract_first_request(requests, &requests_size, request, &request_size) == 0 && i++ < 3) {
    request[request_size] = '\0';
    printf("--- REQ ---:\r\n%s--- END ---\r\n", request);
  }
  //*/
  /*
  char requests[200], *rch = "ton";
  sprintf(requests, "Coucou toi\r\nBonjour sinon\r\n\r\nSeconde requete\r\n\r\nTroisieme requete\r\nBeaucoup plus longue cette fois !\r\nEncore plus longue...\r\n\r\n");
  int requests_size = strlen(requests);
  int i = mempos(requests, rch, requests_size);
  printf("len=%d, mempos=%d\n", requests_size, i);
  i = mempos("tt", "tt", 2);
  printf("len=%d, mempos=%d\n", requests_size, i);
  //*/
  /*
  if(argc < 2) {
    printf("Missing argument 1. Usage : %s filename\n", argv[0]);
  } else {
    printf("time = %s\n", http_time(argv[1]));
  }
  //*/
  /*
  char str[2048];
  int size = 48;
  memcpy(str, "Coucou toi\r\nBonjour sinon\r\n\r\nSeconde requete\r\n\r\n", size);
  str[size] = '\0';
  
  if(argc < 2) {
    printf("Missing argument 1. Usage : %s filename\n", argv[0]);
  } else {
    add_cache_header(str, &size, 2048, http_time(argv[1]));
    add_close_header(str, &size, 2048);
    str[size] = '\0';
    printf("--- REQ ---:\r\n%s--- END ---\r\n", str);
  }
  //*/
  //printf("size=%d\n", (int) strlen("Connection: close\r\n"));
  /*
  char str[] = "HTTP/1.1 403 Not Found\r\nDate: Sat, 09 May 2015 09:45:20 GMT\r\nServer: Apache\r\nVary: Accept-Encoding\r\nContent-Length: 320\r\nConnection: close\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n";
  
  printf("code=%d\n", extract_http_code(str, strlen(str)));
  //*/
  /*
  char *filename = "test.txt";
  char *content = "ligne1\nligne2\nEOF";
  
  printf("result=%d\n", file_put_contents(filename, content, strlen(content)));
  //*/
  /*
  char *filename = "test.txt";
  char str[BUFF_SIZE];
  int length = file_get_contents(filename, str, BUFF_SIZE);
  if(length > 0) {
    str[length] = '\0';
    printf("result=%d\nfile=%s", length, str);
  } else {
    printf("result=%d\n", length);
  }
  //*/
  /*
  int i = 0;
  printf("result=%d\n", (i = get_int("3", -1)) == 3);
  printf("i=%d\n", i);
  //*/
  /*
  char *str = "Moi je cherche quelque chose";
  char *rch = "cherche";
  printf("r=%d\n", strpos(str, rch) );
  //*/
  /*
  char str[BUFF_SIZE];
  sprintf(str, "HTTP/1.1 403 Not Found\r\nDate: Sat, 09 May 2015 09:45:20 GMT\r\nServer: Apache\r\nVary: Accept-Encoding\r\nContent-Length: 320\r\nConnection: close\r\nContent-Type: text/html; charset=iso-8859-1\r\n\r\n");
  int length = strlen(str);
  
  remove_header(str, &length, "Connection");
  str[length] = '\0';
  printf("r=%s\n", str);
  //*/
  /*
  char *host = "www.perdu.comé";
  char *uri = "/ui\\op\%test";
  printf("-%s-\n", cache_filename(host, uri));
  //*/
  /*
  switch(get_int("3", -1)) {
    case 3:
      printf("3\n");
    case 2:
      printf("2\n");
    default:
      printf("def\n");
  }
  //*/
  printf("sizeof(char)=%d\n", sizeof(char));
}

