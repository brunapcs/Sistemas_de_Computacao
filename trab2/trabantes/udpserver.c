#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include<errno.h>
#include <arpa/inet.h>
#include "server.h"
#include "permissoes.h"
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

char pathname[MAXPATHLEN];

#define BUFSIZE 1024

/*
 * error - wrapper for perror
 */
void error(char *msg) {
    perror(msg);
    exit(1);
}

int main(int argc, char **argv) {
    int sockfd; /* socket */
    int portno; /* port to listen on */
    int clientlen; /* byte size of client's address */
    struct sockaddr_in serveraddr; /* server's addr */
    struct sockaddr_in clientaddr; /* client addr */
    struct hostent *hostp; /* client host info */
    char buf[BUFSIZE]; /* message buf */
    char *hostaddrp; /* dotted decimal host addr string */
    int optval; /* flag value for setsockopt */
    int n; /* message byte size */
    
    /*
     * check command line arguments
     */
    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[1]);
    
    /*
     * socket: create the parent socket
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    
    /* setsockopt: Handy debugging trick that lets
     * us rerun the server immediately after we kill it;
     * otherwise we have to wait about 20 secs.
     * Eliminates "ERROR on binding: Address already in use" error.
     */
    optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
               (const void *)&optval , sizeof(int));
    
    /*
     * build the server's Internet address
     */
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons((unsigned short)portno);
    
    /*
     * bind: associate the parent socket with a port
     */
    if (bind(sockfd, (struct sockaddr *) &serveraddr,
             sizeof(serveraddr)) < 0)
        error("ERROR on binding");
    
    /*
     * main loop: wait for a datagram, then echo it
     */
    clientlen = sizeof(clientaddr);
    while (1) {
        
        /*
         * recvfrom: receive a UDP datagram from a client
         */
        bzero(buf, BUFSIZE);
        n = recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &clientlen);
        if (n < 0) error("ERROR in recvfrom");

        /*
         * gethostbyaddr: determine who sent the datagram
         */
        hostp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr,
                              sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        if (hostp == NULL)
            error("ERROR on gethostbyaddr");
        hostaddrp = inet_ntoa(clientaddr.sin_addr);
        if (hostaddrp == NULL)
            error("ERROR on inet_ntoa\n");
        printf("server received datagram from %s (%s)\n",
               hostp->h_name, hostaddrp);
        printf("server received %ld/%d bytes: %s\n", strlen(buf), n, buf);
    
        // ********* ESSA PARTE QUE NOS INTERESSA *********
        //como compilar os programas para poder fazer os testes:
        /* - abra duas janelas de terminal
           - em uma vc roda o server udp e na outra o cliente udp
         Entao:
            - gcc updserver.c server.c permissoes.c -c
            - gcc udpserver.o server.o permissoes.o -o prog
            - ./prog 3999 &
         
         Na outra:
            - gcc udpcliente.c -o cliente
            - ./cliente localhost 3999
         
         quando vc rodar o cliente vai pedir para vc digitar uma mensagem e vc tera q colocar exatamente com a sintaxe correta a mensagem
         quando vc enviar vc pode observar o comportamento do servidor no terminal ao lado para verificar se esta rodando corretamente e verificar qual menssagem o cliente esta recebendo de volta. Lembrando q essa troca de menssagens ocorre atraves da variavel buf.
         */
        
        int ver=0; //int para receber o retorno da funcao de verificacao de permissoes
        INFO *msg = (INFO *)malloc(sizeof(INFO));
        char *req;
        
        /* quebra mensagem carregada em buf e salva em parametros da estrutura msg */
        /* salva em req a requisicao sendo feita */
        req = leitura(buf, msg);
        
        // *********  VERIFICA REQUISICAO ***********
        
        //esta primeira funcao (wr) esta semi-pronta, falta testar mais
        /*repare que eu criei uma variavel char* wr que armazenara a string de resposta que o servidor deve mandar para o cliente
        e, no final estou copiando o conteudo de wr para o buf, que eh a variavel que faz a comunicacao*/
        
            //write in file or create new file

//printf("oi\n");
        if(!strcmp(req, "WR-REQ")){
            char *wr = "WR-REQ,"; //inicializando wr
            ver=verificaPermissoes(msg->path, msg->ClientID);
           
            if(ver == 2){  // retornou 2 eh sinal de arquivo novo (nao encontrou entrada no perm.txt)
                printf("Arquivo novo");
                adicionaServerList(msg->path, msg->ClientID, msg->ownerPerm, msg->otherPerm);
                strcat(wr, WrReq1(msg, msg->otherPerm, msg->ownerPerm)); //concateno em wr a resposta da funcao
            }
            
            else if(ver == 0){
                printf(" voce nao tem permissao de fazer isto com esse arquivo");
                exit(1);
            }
            
            else{
                puts("arquivo ja existe");
                strcat(wr, WrReq(msg));
            }
            strcpy(buf, wr); //copio para o buf
        }
        
        /*Para as proximas funcoes acredito que pode ser utilizada a mesma logica que utilizei na fc de WR para as respostas */
        //FALTA IMPLEMENTAR ESSA RESPOSTA PARA TODAS AS FUNCOES E TESTAR
        
            //read from file
        else if(!strcmp(req, "RD-REQ")){
            char *rd = "RD-REQ";
            ver = verificaPermissoes(msg->path, msg->ClientID);
            if( ver == 2){
                printf(" File you are trying to read from doesnt exists"); exit(1);
            }
            strcpy(buf, msg->payload);
        }
            //show file stat
        else if(!strcmp(req, "FI-REQ")){
            FiReq(msg);

//			printf("oi\n");
			char *wr = "FI-REQ,"; //inicializando wr
            /*ver=verificaPermissoes(msg->path, msg->ClientID);
           
            if(ver == 2){  // retornou 2 eh sinal de arquivo novo (nao encontrou entrada no perm.txt)
                printf("Arquivo novo");
                adicionaServerList(msg->path, msg->ClientID, msg->ownerPerm, msg->otherPerm);
                strcat(wr, WrReq1(msg, msg->otherPerm, msg->ownerPerm)); //concateno em wr a resposta da funcao
            }
            
            else if(ver == 0){
                printf(" voce nao tem permissao de fazer isto com esse arquivo");
                exit(1);
            }
            
            else{
                puts("arquivo ja existe");
                strcat(wr, WrReq(msg));
            }*/
			strcat(wr, FiReq(msg));
            strcpy(buf, wr); //copio para o buf
        }
            //create new directory
        else if(!strcmp(req, "DC-REQ")){
            verificaPermissoes(msg->path, msg->ClientID);
            if(msg == 0){
                printf("voce nao tem permissao de criar itens nesse diretorio"); exit(1); 
            }
            DcReq(msg);
        }
        
        //FALTA COLOCARA OS RESTO DAS REQUISICOES, FI-REQ, DR-REQ, DL-REQ
        //FALTA FAZER AS VERIFICACOES DE PERMISSAO PARA CADA TIPO DE REQUISICAO, PRESTAR ATENÇÃO QUE CADA UMA É DIFERENTE DA OUTRA,
            //exemplo: na requisicao de leitura eu nao tenho que testar permissao nenhuma porque teoricamente todo mundo pode ler alguma coisa
            //         ja na requisicao de write eu tenho que checar se o cara tem permissao para escrever
            //         assim como na requisicao de criar diretorio eu tenho q checar se o cara tem permissao para criar algo no diretorio corrente!!
        //FALTA FAZER OS TESTES COM TUDO FUNCINANDO
        
        
        /* Entao resumidamente, em ordem o que tem que ser feito:
          - implementar as funcoes de mostrar as coisas e testar, (FI-Req e DL-Req)
          - testar a funcao WR-Req com a comunicacao, verificar se tudo funciona normalmente
                    -> se funcionar eh sinal de que estamos no caminho certo
         
         - implementar verificacoes de permissao no resto das funcoes e testar, comecar pela de criacao de diretorio
         - implementar resposta nas funcoes
         - testar resposta das funcoes
         - implementar funcao de remover diretorio
         - implementar funcao de remover arquivo
         - implementar resposta nas funcoes de remocao
         - testar tudo
         
         - correr para o abraço
         */
        
        // sendto: echo the input back to the client
        
        n = sendto(sockfd, buf, strlen(buf), 0,
                   (struct sockaddr *) &clientaddr, clientlen);
        if (n < 0)error("ERROR in sendto");
        
        
        
        
    }
}
