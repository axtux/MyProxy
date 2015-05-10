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
int strpos(char *str, char *rch) {
  char *start = strstr(str, rch);
  if(start) {
    return (int) ((start-str) / sizeof(*str));
  } else {
    return -1;
  }
}

int mempos(char *mem, const char *str, int max_length) {
  int i = 0, len = strlen(str);
  
  for(max_length -= len-1; i < max_length; ++i) {
    if(memcmp(&mem[i], str, len) == 0) {
      return i;
    }
  }
  
  return -1;
}

char* http_time(char *filename){
  struct stat *buf = malloc(sizeof(*buf));
  char *str = 0;
  
  if(stat(filename, buf) != -1) {
    struct tm time = *gmtime(&buf->st_mtime);
    str = malloc(sizeof(*str) * (HTTP_TIME_LENGTH+1));
    strftime(str, HTTP_TIME_LENGTH+1, "%a, %d %b %Y %X GMT", &time);
  }
  
  free(buf);
  return str;
}

void remove_header(char *request, int *length, char *header) {
  int start = mempos(request, header, *length);
  
  if(start != -1) {
    int size = mempos(&request[start], "\r\n", *length) +2;
    
    if(size != 1) {
      //*
      int i;
      for(i = start; i+size < *length; ++i) {
        request[i] = request[i+size];
      }
      //*/ memcpy(&request[start], &request[start+size], *length-start-size);
      *length -= size;
    }
  }
}

void add_cache_header(char *request, int *length, int max_length, char *time) {
  int new_length = *length + 19 + HTTP_TIME_LENGTH + 2;
  if(!time || max_length < new_length) {
    return;
  }
  
  memcpy(&request[*length-2], "If-Modified-Since: ", 19); // copy over last \r\n
  memcpy(&request[*length-2+19], time, HTTP_TIME_LENGTH);
  memcpy(&request[*length-2+19+HTTP_TIME_LENGTH], "\r\n\r\n", 4);
  *length = new_length;
}

void add_close_header(char *request, int *length, int max_length) {
  int new_length = *length + 19;
  if(max_length < new_length) {
    return;
  }
  
  memcpy(&request[*length-2], "Connection: close\r\n\r\n", 21); // copy over last \r\n
  *length = new_length;
}

int file_put_contents(char *filename, char *content, int length) {
  FILE* file = fopen(filename, "w");
  
  if(!file) {
    printf("Error opening file %s.\n", filename);
    return -1;
  }
  
  if(fwrite(content, sizeof(content[0]), length, file) != length) {
    printf("Error writing file %s.\n", filename);
    return -2;
  }
  
  fclose(file);
  return 0;
}

int file_get_contents(char *filename, char *content, int max_length) {
  FILE* file = fopen(filename, "r");
  int length = 0;
  
  if(!file) {
    printf("Error opening file %s.\n", filename);
    return -1;
  }
  
  length = fread(content, sizeof(content[0]), max_length, file);
  if(length <= 0) {
    printf("Error reading file %s.\n", filename);
    return -2;
  }
  
  fclose(file);
  return length;
}
// transform % to %25 (reversible) and / to %2F (forbidden) and add CACHE_PATH
char *cache_filename(char* host, char* uri) {
  char *CACHE_PATH = "./cache_files/", *filename = 0;
  int i, j, size = 0;
  
  for(i = 0; host[i] != 0; ++i) {
    if(host[i] == '%' || host[i] == '/') {
      size += 2;
    }
  }
  size += i;
  
  for(i = 0; uri[i] != 0; ++i) {
    if(uri[i] == '%' || uri[i] == '/') {
      size += 2;
    }
  }
  size += i;
  
  j = strlen(CACHE_PATH);
  filename = malloc(sizeof(*filename) * (j+size+1));
  filename[j+size] = 0;
  memcpy(filename, CACHE_PATH, j);
  
  for(i = 0; host[i] != 0; ++i, ++j) {
    if(host[i] == '%') {
      filename[j] = '%';
      filename[j+1] = '2';
      filename[j+2] = '5';
      j += 2;
    } else if(host[i] == '/') {
      filename[j] = '%';
      filename[j+1] = '2';
      filename[j+2] = 'F';
      j += 2;
    } else {
      filename[j] = host[i];
    }
  }
  
  for(i = 0; uri[i] != 0; ++i, ++j) {
    if(uri[i] == '%') {
      filename[j] = '%';
      filename[j+1] = '2';
      filename[j+2] = '5';
      j += 2;
    } else if(uri[i] == '/') {
      filename[j] = '%';
      filename[j+1] = '2';
      filename[j+2] = 'F';
      j += 2;
    } else {
      filename[j] = uri[i];
    }
  }
  
  return filename;
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
  int pos = mempos(requests, "\r\n\r\n", *requests_size)+4;
  
  if(pos != 3) {
    *request_size = pos;
    memcpy(request, requests, *request_size);
    
    *requests_size -= *request_size;
    memmove(requests, &requests[*request_size], *requests_size);
    
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
  int uri_start = 0, uri_end = 0;
  char *uri = 0;
  
  while(i+4 < length) {
    if(
      (request[i] == 'g' || request[i] == 'G')
      && (request[i+1] == 'e' || request[i+1] == 'E')
      && (request[i+2] == 't' || request[i+2] == 'T')
      && request[i+3] == ' '
    ) {
      uri_start = i+4;
      break;
    }
    ++i;
  }
  
  if(uri_start) {
    i = uri_start;
    while(i < length) {
      if( // match uri chars
        (request[i] >= 'a' && request[i] <= 'z')
        || (request[i] >= 'A' && request[i] <= 'Z')
        || (request[i] >= '0' && request[i] <= '9')
        || request[i] == '%'
        || request[i] == '-'
        || request[i] == '.'
        || request[i] == '_'
        || request[i] == '~'
        || request[i] == ':'
        || request[i] == '/'
        || request[i] == '?'
        || request[i] == '#'
        || request[i] == '['
        || request[i] == ']'
        || request[i] == '@'
        || request[i] == '!'
        || request[i] == '$'
        || request[i] == '&'
        || request[i] == '\''
        || request[i] == '('
        || request[i] == ')'
        || request[i] == '*'
        || request[i] == '+'
        || request[i] == ','
        || request[i] == ';'
        || request[i] == '='
      ) {
        // do nothing
      } else if( // not first char and end of request
        i != uri_start
        && i+10 < length
        && request[i] == ' '
        && (request[i+1] == 'h' || request[i+1] == 'H')
        && (request[i+2] == 't' || request[i+2] == 'T')
        && (request[i+3] == 't' || request[i+3] == 'T')
        && (request[i+4] == 'p' || request[i+4] == 'P')
        && request[i+5] == '/'
        && (request[i+6] >= '0' && request[i+6] <= '9')
        && request[i+7] == '.'
        && (request[i+8] >= '0' && request[i+8] <= '9')
        && request[i+9] == '\r'
        && request[i+10] == '\n'
      ) {
        uri_end = i;
        break;
      } else { // wrong char
        break;
      }
      ++i;
    }
  }
  
  if(uri_end) {
    int size = uri_end - uri_start;
    uri = malloc(sizeof(*uri) * (size +1));
    i = 0;
    while(i < size) {
      uri[i] = request[uri_start+i];
      ++i;
    }
    uri[size] = '\0';
  }
  
  return uri;
}

int extract_http_code(char *response, int response_size) {
  int code = 0;
  if(response_size < 13) {
    return code;
  }
  if(
    (response[0] == 'h' || response[0] == 'H')
    && (response[1] == 't' || response[1] == 'T')
    && (response[2] == 't' || response[2] == 'T')
    && (response[3] == 'p' || response[3] == 'P')
    && response[4] == '/'
    && (response[5] >= '0' && response[5] <= '9')
    && response[6] == '.'
    && (response[7] >= '0' && response[7] <= '9')
    && response[8] == ' '
    && (response[9] >= '0' && response[9] <= '9')
    && (response[10] >= '0' && response[10] <= '9')
    && (response[11] >= '0' && response[11] <= '9')
    && response[12] == ' '
  ) {
    code += 100*(response[9] - '0');
    code += 10*(response[10] - '0');
    code += response[11] - '0';
  }
  return code;
}

