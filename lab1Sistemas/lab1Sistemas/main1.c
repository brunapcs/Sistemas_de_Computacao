#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int main(){

//primeiro vc compila o alo mundo
// o primeiro argumento do execv tem q ser o nome que voce deu ao executavel do alomundo.c
// o segundo argumento eh NULL pois nao passamos nenhum parametro para o novo processo
        execv("alo",NULL);
        printf("Falhou"); /* printara na tela somente se a instrucao execv falhar */

}

