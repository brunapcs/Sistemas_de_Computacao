#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include<errno.h>

extern int alphasort();
char pathname[MAXPATHLEN];

int file_select(struct direct *entry){
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return (0);
    else
     return (1);
}

//pega a msg lida e salva informacoes na struct INFO 
char * leitura(char *str, INFO *msg){
    const char s[2] = ",";
    char *req = strtok(str, s);
   
    msg->path = strtok(NULL, s);
    msg->strlen = atof(strtok(NULL, s));
    
    if(req[0] == 'R' || req[0] == 'W'){
        printf("requisicao de arquivo\n");
        msg->payload = strtok(NULL, s );
        msg->nrbytes = atof(strtok(NULL, s));
        msg->offset = atof(strtok(NULL, s));
        msg->ClientID = strtok(NULL, s);
    
    }
    else if(req[0] == 'D'){
        printf("requisicao de diretorio\n"); 
        msg->dirname = strtok(NULL, s);
        msg->strlen_dirname = atof(strtok(NULL, s));
        msg->ClientID = strtok(NULL, s);
    }
    
    msg->ownerPerm =strtok(NULL, s);
    msg->otherPerm = strtok(NULL, s);

    return req;
}

//WR-REP, path (string), strlen(int), payload(string vazio), nrbytes escritos (int), offset igual ao do W-REQ(int)
//concatena informacoes em string de resposta para o cliente
char *Rep(char* path,int strlen, char* payload, int bytes, int offset){
    char str[10];
    snprintf(str, 10, "%d", strlen);
    strcat(path, ",");
    strcat(path, str);
    strcat(path, ",");
    if(payload != NULL){
        strcat(path, payload);
        strcat(path, ",");
    }
    snprintf(str, 10,"%d", bytes);
    strcat(path, str );
    strcat(path, ",");
    snprintf(str, 10, "%d", offset);
    strcat(path, str);
    return path;
}

char *RdReq(INFO *inf){
    if (getwd(pathname) == NULL ) { printf("Error getting path\n"); exit(0); }
    strncat(pathname, inf->path, inf->strlen);
    char *ret = strrchr(inf->path, '/' )+1;
    if (chdir(pathname) == -1 ) { printf("Erro na troca de diretorio\n"); exit(0); }
    
    int fd = open(ret, O_RDONLY);
    if(fd<0)printf("error opening file");
    int bytes_read = read(fd, inf->payload, inf->nrbytes);
    
    printf("leu %d bytes", bytes_read);
    close(fd);
    
    strcat(pathname, "/");
    strcat(pathname,  ret);
    char *resp= Rep(pathname, inf->strlen, inf->payload, bytes_read, inf->offset);
    return resp;
}

char * WrReq1(INFO *inf, char *otherP, char *ownerP){
    int p = 0;
    if(!strcmp(otherP, "W") && !strcmp(ownerP, "W"))
        p=0777;
    
    else if(!strcmp(ownerP, "W") && !strcmp(otherP, "R"))
        p=0766;
    
    else if(!strcmp(ownerP, "R") && !strcmp(otherP, "W"))
        p=0677;
    
    else if(!strcmp(ownerP, "R") && !strcmp(otherP, "R"))
        p=0666;
    
    
    if (getwd(pathname) == NULL ) { printf("Error getting path\n"); exit(0); }
    strncat(pathname, inf->path, inf->strlen);
    char *ret = strrchr(inf->path, '/' )+1;
    if (chdir(pathname) == -1 ) { printf("Erro na troca de diretorio\n"); exit(0); }
    
    int fd = open(ret, O_RDWR | O_CREAT , p );
    if(fd<0)printf("error opening file");
    int bytes_written = pwrite(fd, inf->payload, inf->nrbytes, inf->offset);
   
    printf("escreveu %d bytes no arquivo: %s", bytes_written, ret);
    close(fd);
   
    strcat(pathname, "/");
    strcat(pathname, ret);
    char *resp = Rep(pathname,inf->strlen,NULL, bytes_written, inf->offset);
    return resp;
}

char *WrReq(INFO *inf){
    if (getwd(pathname) == NULL ) { printf("\nError getting path\n"); exit(0); }
    strncat(pathname, inf->path, inf->strlen);
    if (chdir(pathname) == -1 ) { printf("\n Erro na troca de diretorio\n"); exit(0); }
    char *ret = strrchr(inf->path, '/' )+1;
   
    int fd = open(ret, O_RDWR);
    if(fd<0)printf("\n Error opening file \n");
    int bytes_written = pwrite(fd, inf->payload, inf->nrbytes, inf->offset);
   
    printf("escreveu %d bytes no arquivo: %s", bytes_written, ret);
    close(fd);

    strcat(pathname, "/");
    strcat(pathname, ret);
    char *resp =Rep(pathname, inf->strlen, NULL, bytes_written, inf->offset);
    return resp;
}

char* FiReq(INFO *inf){
    struct dirent **files;
    struct stat* aux;

	if (getwd(pathname) == NULL ) { printf("\nError getting path\n"); exit(0); }
  	strncat(pathname, inf->path, inf->strlen);
    if (chdir(pathname) == -1 ) { printf("\n Erro na troca de diretorio\n"); exit(0); }
    
    int fd = open(inf->arq_name, O_RDWR);
    fstat (fd, aux);
    printf("tsize: %i, owner:%d ", aux->st_size,aux->st_uid );

	
	char *resp = FiResp(aux->st_size,aux->st_uid);
	return resp;
    //retornar o tamanho (st_size) e a id do owner do arquivo (st_uid) para a socket
}

//TESTAR
char* FiResp(int owner, int size){
	char resp[100];

	sprintf(resp, "%d,%i", owner, size);
	return resp;
}

char* DcReq(INFO *inf){
    struct stat fileData;

    if (getwd(pathname) == NULL ) { printf("Error getting path\n"); exit(0); }
   
    if(inf->strlen != 0)
        strncat(pathname, inf->path, inf->strlen+1);
 
    if (chdir(pathname) == -1 ) { printf("Erro na troca de diretorio\n"); exit(0); }
    if(stat(inf->dirname, &fileData) == -1) {
        int p = 0;
        if(!strcmp(inf->otherPerm, "W") && !strcmp(inf->ownerPerm, "W")){
            p=0777;
        }
        else if(!strcmp(inf->ownerPerm, "W") && !strcmp(inf->otherPerm, "R")){
            p=0766;
        }
        else if(!strcmp(inf->ownerPerm, "R") && !strcmp(inf->otherPerm, "W")){
            p=0677;
        }
        else if(!strcmp(inf->ownerPerm, "R") && !strcmp(inf->otherPerm, "R")){
            p=0666;
        }
        if(mkdir(inf->dirname, p) == 0) {
            printf("\nDirectory %s created successfuly!\n\n", inf->dirname);
			strcat(pathname, "/");
			strncat(pathname, inf->dirname, inf->strlen_dirname);
			strcat(pathname, "/");
			char* resp = DcRep(path, inf->strlen);
			return resp;

        } else {
            printf("\nError creating directory %s => %s\n\n", inf->dirname, strerror(errno));
			char* resp = DcRep(path, inf->strlen);
			return resp;
       
        }
    }
    else {
        printf("\nDirectory already exists\n\n");
		char* resp = DcRep(path, inf->strlen);
		return resp;
       
    }
}

//TESTAR
void DcRep(char *path, int strlen){
	char resp[100];

	if(strlen == 0)
		sprintf(resp, "strlen = %d", strlen);
	else
		sprintf(resp, "%s", path);

	return resp;
}












