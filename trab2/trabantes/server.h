#ifndef server_h
#define server_h
#define MAXPATH 100 

struct info{
    char *path;
    char *arq_name; 
    int  strlen;
    char *payload;
    int nrbytes;
    int offset;
    char *ClientID;
    char *ownerPerm;
    char *otherPerm;
    char *dirname;
    int strlen_dirname;
};typedef struct info INFO;



char * leitura(char *str, INFO *msg);

char*RdReq(INFO *inf);

char* WrReq(INFO *inf);
char * WrReq1(INFO *inf, char *otherP, char *ownerP);

char *Rep(char* path,int strlen, char* payload, int bytes, int offset);

char* FiReq(INFO *inf);
char* FiResp(int owner, int size);
char* DcReq(INFO *inf);
char* DcResp(char *path, int strlen);

#endif /* server_h */
