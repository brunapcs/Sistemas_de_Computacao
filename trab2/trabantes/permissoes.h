#include <stdlib.h>
int verificaPermissoes(char *path, char *clientID);
int adicionaServerList(char *path, char *ownerID, char *ownerPerm, char *otherPerm);
int atualizaServerList(char *path);
size_t removeEntrada(char *buffer, size_t size, const char *path);
