#include "functions.h"

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

char* str_addr(struct sockaddr_in address) {
  char* r = malloc(4*4+5);
  sprintf(r, "%d.%d.%d.%d:%d", address.sin_addr.s_addr%256, address.sin_addr.s_addr/(256)%256, address.sin_addr.s_addr/(256*256)%256, address.sin_addr.s_addr/(256*256*256)%256, address.sin_port%65536);
  return r;
}

char* getdate(char *filename){
  struct stat *buf = malloc(sizeof(*buf));
  char *buff = malloc(sizeof(*buff) * 100);
  stat( filename, buf );
  time_t date = buf->st_mtime;
  struct tm temps = *localtime(&date);
  printf("%lld\n", (long long)buf->st_mtime );
  strftime(buff, 100, "%a. %d %B %Y %X", &temps);
  free(buf);
  return buff;
}

void ecrireFichier(char texte[], char nom[]){
  FILE* fichier = NULL;
  fichier = fopen(nom, "w");
  if (fichier != NULL)
  {
    fputs(texte, fichier);
    fclose(fichier);
  }
  return;
}

int lireFichier(char nom[], char message[]){ //modifie message
  FILE* fichier = NULL;
  fichier = fopen(nom, "r");
  if (fichier == NULL){
    printf("fichier inexistant\n");
    return -1;
  }
  
  fgets(message, MAXSIZE, fichier);
  return 1;
  
}



char* cache_name(char* host, char* uri) {
  int size = 0;
  int i = 0;
  
  while(host[i] != '\0') {
    
  }
}

// Not working with gcc 4.8
char *regmatch(char *str_request, char *str_regex) {
  regex_t preg;
  
  if(regcomp(&preg, str_regex, 0)) {
    printf("Error compiling regex.\n");
    return 0;
  }
  
  size_t nmatch = preg.re_nsub;
  regmatch_t *pmatch = malloc(sizeof(*pmatch) * nmatch);
  
  char *str_match = 0;
  
  if(regexec(&preg, str_request, nmatch, pmatch, REG_EXTENDED | REG_ICASE) == 0) { // return first matching ()
    int start = pmatch[0].rm_so; // offset first char
    printf("start=%d\n", start);
    int end = pmatch[0].rm_eo; // offset of last char+1
    printf("end=%d\n", end);
    size_t size = end - start;
    
    str_match = malloc(sizeof(*str_match) * (size+1));
    strncpy(str_match, &str_request[start], size);
    str_match[size] = '\0';
  }
  regfree(&preg); // frees memory allocated by regcomp()
  free(pmatch);
  return str_match;
}

int extract_first_request(char *requests, int *requests_size, char *request, int *request_size) {
  int i;
  *request_size = -1;
  
  for(i = 4; i < *requests_size; ++i) {
    if(
      requests[i-4] == '\r'
      && requests[i-3] == '\n'
      && requests[i-2] == '\r'
      && requests[i-1] == '\n'
    ) {
      *request_size = i;
      break;
    }
  }
  
  if(*request_size != -1) {
    //request = malloc(sizeof(*request) * (*request_size));
    for(i = 0; i < *request_size; i++) {
      request[i] = requests[i];
      if(i+*request_size < *requests_size) {
        requests[i] = requests[i+*request_size];
      }
    }
    *requests_size -= *request_size;
    return 0;
  }
  return -1;
}

char* extract_host(char* request, int length) {
  int i = 0;
  int host_start = 0, host_end = 0;
  char *host = 0;
  
  while(i+6 < length) {
    if(
      (request[i] == 'h' || request[i] == 'H')
      && (request[i+1] == 'o' || request[i+1] == 'O')
      && (request[i+2] == 's' || request[i+2] == 'S')
      && (request[i+3] == 't' || request[i+3] == 'T')
      && request[i+4] == ':'
      && request[i+5] == ' '
    ) {
      host_start = i+6;
      break;
    }
    ++i;
  }
  
  if(host_start) {
    i = host_start;
    while(i < length) {
      if(
        (request[i] >= 'a' && request[i] <= 'z')
        || (request[i] >= 'A' && request[i] <= 'Z')
        || (request[i] >= '0' && request[i] <= '9')
        || request[i] == '.'
      ) { // match [.a-zA-Z0-9]
        // do nothing
      } else if(i != host_start && request[i] == '\r' && i+1 < length && request[i+1] == '\n'){
        host_end = i;
        break;
      }
      ++i;
    }
    if(host_end) {
      int size = host_end - host_start;
      host = malloc(sizeof(*host) * (size +1));
      i = 0;
      while(i < size) {
        host[i] = request[host_start+i];
        ++i;
      }
      host[size] = '\0';
    }
  }
  
  return host;
}
char* extract_uri(char* request, int length) {
  int i = 0;
  int host_start = 0, host_end = 0;
  char *host = 0;
  
  while(i+6 < length) {
    if(
      (request[i] == 'h' || request[i] == 'H')
      && (request[i+1] == 'o' || request[i+1] == 'O')
      && (request[i+2] == 's' || request[i+2] == 'S')
      && (request[i+3] == 't' || request[i+3] == 'T')
      && request[i+4] == ':'
      && request[i+5] == ' '
    ) {
      host_start = i+6;
      break;
    }
    ++i;
  }
  
  if(host_start) {
    i = host_start;
    while(i < length) {
      if(
        (request[i] >= 'a' && request[i] <= 'z')
        || (request[i] >= 'A' && request[i] <= 'Z')
        || (request[i] >= '0' && request[i] <= '9')
        || request[i] == '.'
      ) { // match [.a-zA-Z0-9]
        // do nothing
      } else if(i != host_start && request[i] == '\r' && i+1 < length && request[i+1] == '\n'){
        host_end = i;
        break;
      }
      ++i;
    }
    if(host_end) {
      int size = host_end - host_start;
      host = malloc(sizeof(*host) * (size +1));
      i = 0;
      while(i < size) {
        host[i] = request[host_start+i];
        ++i;
      }
      host[size] = '\0';
    }
  }
  
  return host;
}

