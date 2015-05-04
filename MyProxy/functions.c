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


char* getdate(char *filname){
	struct stat *buf;
	char buff[100];
	buf = malloc(sizeof(struct stat));
	time_t date;
	stat( filename, buf );
	date = buf->st_mtime;
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



