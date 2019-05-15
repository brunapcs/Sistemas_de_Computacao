#include "permissoes.h"
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



int verificaPermissoes(char *path, char *clientID){//retorna 1 caso permissao validada
  char *buffer;
  char *p;
  FILE *serverlist;
  struct stat aux;
  int done = 0;
  size_t len = strlen(path);
  char *pch;
  char ownerID[5];
  char ownerPerm[2];
  char otherPerm[2];

  serverlist = fopen("perm.txt", "r");
  if (serverlist == NULL){
    printf("Erro: falha ao abrir  para verificar permissoes\n");
    exit(-1);
  }

  stat("perm.txt", &aux);
  buffer = malloc(aux.st_size);
  p = buffer; 

  // read the file into a buffer
  if (fread(buffer, 1, aux.st_size, serverlist) != aux.st_size){
    printf("Erro: nao foi possivel carregar serverlist para buffer\n");
    exit(-1);
  }
  
  fclose(serverlist);
  do{
      char *q = strchr(p, *path); //FIX: procura a primeira letra do path apos o /
      if (q != NULL){ //caso tenha encontrado a primeira letra
          if (strncmp(q, path, len) == 0){ //found name?
            //parse entrada de datalist
            pch = strtok (q+len+1, " ");
            memcpy(ownerID, pch, strlen(pch)+1);

            pch = strtok (NULL, " ");
            memcpy(ownerPerm, pch, strlen(pch)+1);

            pch = strtok (NULL, "\n");
            memcpy(otherPerm, pch, strlen(pch)+1);
              if(strncmp(ownerID, clientID, strlen(ownerID)) == 0){
                  //verifica permissao de owner
                  printf("voce eh o owner e");
                  if(strcmp(ownerPerm, "W") == 0){
                      printf(" vc pode escrever \n");
                      return 1;
                  }
              return 0;
            }
            else{ //verifica permissao para others
                printf("voce eh um other e");
                if(strcmp(otherPerm, "W") == 0){
                    printf(" voce pode escrever\n" );
                    return 1;
                }
              return 0;
            }
          }
          else
            p = q + 1; // continue search
        }
        else{
          printf("dado nao foi encontrado no servidor\n");
          return 2;
        }
    } while (done==0);
  return -1;
}

int adicionaServerList(char *path, char *ownerID, char *ownerPerm, char *otherPerm){ //FIX faltando adicionar com as permissoes
 char *entrada;
 FILE *serverlist;
 entrada = strndup(path, strlen(path)+1);

  serverlist = fopen("perm.txt", "a");
  if (serverlist == NULL){
    printf("Nao foi possivel obter a lista de dados do servidor\n");
    exit(1);
  }

  strncat (entrada, " ", 1);
  strncat (entrada, ownerID, strlen(ownerID));

  strncat (entrada, " ", 1);
  strncat (entrada, ownerPerm, strlen(ownerPerm));

  strncat (entrada, " ", 1);
  strncat (entrada, otherPerm, strlen(otherPerm)+1);
  entrada[strlen(entrada)+1] = '\n'; //substitiu \0 por \n

  printf("entrada - %s - %ld\n", entrada, strlen(entrada));
    fputs("\n", serverlist);
  fputs(entrada, serverlist);
  fclose(serverlist);

  return 1;
}

size_t removeEntrada(char *buffer, size_t size, const char *path){
    // file format assumed to be as specified in the question i.e. name{space}somevalue{space}someothervalue\n
    
    //find path
    char *p = buffer;
    int done = 0;
    size_t len = strlen(path);
    
    char *line;
    size_t newSize = 0;
    size_t lineSize = 0;
    size_t restSize;
    
    do{
        char *q = strchr(p, *path); //FIX: procura a primeira letra do path apos o /
        if (q != NULL) //caso tenha encontrado a primeira letra
        {
            if (strncmp(q, path, len) == 0) //found name?
            {
                // count number of characters the line has.
                for (line = q; *line != '\n'; ++line){
                    ++lineSize;
                }
                
                lineSize += 1; //include \n in lineSize
                
                // calculate length left after line by subtracting offsets
                restSize = (size_t)((buffer + size) - (q + lineSize));
                
                // move block with next line forward
                memmove (q, q + lineSize, restSize);
                
                // calculate new size
                newSize = size - lineSize;
                done = 1;
            }
            
            else{
                p = q + 1; // continue search
            }
        }
        
        else{
            printf("Erro: dado nao foi encontrado no servidor\n");
            done = 1;
            newSize = -1;
        }
        
    }while (done==0);
    return newSize;
}



int atualizaServerList(char *path){
  struct stat dtlst;
  stat("perm.txt", &dtlst);
  char *buffer;
  size_t newSize;
  FILE* serverList;
  serverList = fopen("perm.txt", "r");
  if (serverList == NULL){
    printf("Erro: falha ao abrir lista de dados do servidor para leitura\n");
    exit(-1);
  }
  
    // allocate memory to hold file
  buffer = malloc(dtlst.st_size); 

    // read the file into a buffer
  if (fread(buffer, 1, dtlst.st_size, serverList) != dtlst.st_size)  {
    printf("Erro: nao foi possivel carregar serverlist para buffer\n");
    exit(-1);
  }
  fclose(serverList);
  newSize = removeEntrada(buffer, dtlst.st_size, path);
  if(newSize != -1){
    serverList = fopen("perm.txt", "w");
    if (serverList == NULL)
    {
      printf("Erro: falha ao abrir lista de dados do servidor para escrita\n");
      exit(-1);
    }
    fwrite(buffer, 1, newSize, serverList);
    fclose(serverList);   
  }

  printf("PATH - %s\n", path);

  free(buffer);
  
  return 1;
}


