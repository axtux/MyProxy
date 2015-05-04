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

char* cache_name(char* host, char* uri) {
  int size = 0;
  int i = 0;
  
  while(host[i] != '\0') {
    
  }
}

void mem_copy(char* src, char* dst, int dst_start, int length) {
  int i = 0;
  while(src[i] != '\0' && i < length) {
    dst[dst_start+i] = src[i];
    ++i;
  }
}

void mem_copyy(char* src, char* dst, int src_start, int length) {
  int i = 0;
  while(src[src_start+i] != '\0' && i < length) {
    dst[i] = src[src_start+i];
    ++i;
  }
}

char *regmatch(char *str_request, char *str_regex) {
  int err;
  regex_t preg;
  char *str_match = 0;
  
  err = regcomp(&preg, str_regex, REG_EXTENDED | REG_ICASE);
  
  if (err == 0) {
    int match;
    size_t nmatch = 0;
    regmatch_t *pmatch = 0;
    
    nmatch = preg.re_nsub;
    pmatch = malloc(sizeof (*pmatch) *nmatch);
    if(pmatch) {
      match = regexec(&preg, str_request, nmatch, pmatch, 0);
      regfree(&preg);
      if (match == 0) {
        int start = pmatch[0].rm_so;
        int end = pmatch[0].rm_eo;
        size_t size = end - start;
           
        str_match = malloc(sizeof (char) * (size + 1));
        if (str_match) {
          strncpy(str_match, &str_request[start], size);
          str_match[size] = '\0';
        }
      }
      //free(pmatch);
    }
  }
  return str_match;
}


