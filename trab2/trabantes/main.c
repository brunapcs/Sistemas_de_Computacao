#include "server.h"
#include "permissoes.h"
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

char pathname[MAXPATHLEN];

int main() {
    INFO *msg = (INFO *)malloc(sizeof(INFO));
    //char str[80] = "DC-REQ,/trab2,10,dirname,10,2023,W,W"; //req, path, strlen, dirname, strlen(?), clienteID , ownerPerm, otherPerm
    //char str[80] = "RD-REQ,/oi/arq,3,payload,10,5,3032,W,W";//req, path, strlen, payload, nrbyrtes, offset, clienteID, ownerPerm, otherPerm
    //char str[80] = "WR-REQ,/primeiro/escrita.txt,9,payload,10,30,2222,W,R"; //escreve nrbytes do conteúdo de payload no arquivo path a partir da posição offset
    //char str[80] = "WR-REQ,/primeiro/oie.txt,9,payload,10,30,2222,W,W";
    //char str[80] = "WR-REQ,/primeiro/maisum.c,9,payload,10,30,2232,R,R";
    char str[80] = "DC-REQ,/primeiro,10,teste6,6,2023,W,R";
    char *req;
  
    /* quebra mensagem salva e parametros */
    req = leitura(str, msg);
    
    if(!strcmp(req, "WR-REQ")){
       /* int ver=verificaPermissoes(msg->path, msg->ClientID);
       if(ver == 2){ //arquivo novo
            printf("Arquivo novo");
            adicionaServerList(msg->path, msg->ClientID, msg->ownerPerm, msg->otherPerm);
            WrReq1(msg, msg->otherPerm, msg->ownerPerm);
        }
        else if(ver == 0){
            printf(" voce nao tem permissao de fazer isto com esse arquivo");
        exit(1);
         }
        else{
            puts("arquivo ja existe");
            WrReq(msg);
        }
*/
    }
    
    if(!strcmp(req, "RD-REQ" )){
        RdReq(msg); 
    }
    else if(!strcmp(req, "WT-REQ")){
        WrReq(msg); 
    }
    
    else if(!strcmp(req, "FI-REQ")){
        FiReq(msg); 
    }
    
    else if(!strcmp(req, "DC-REQ")){
        DcReq(msg);
    }
   
    
    return 0;
}
