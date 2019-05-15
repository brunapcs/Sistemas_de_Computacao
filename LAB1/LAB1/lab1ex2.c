#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdio.h>

/*• Pai - Imprime os números de 1 a 50, com um intervalo de 2 segundos entre cada número. Após imprimir todos os números, imprime a frase “Processo pai vai morrer”.
 • Filho1 - Imprime os números de 100 a 200, com um intervalo de 1 segundo entre cada número. Antes de imprimir os números, imprime a frase “Filho 1 foi criado”, e após imprimir todos os números, imprime a frase “Filho 1 vai morrer”.
 •Neto1- (filho do processo Filho1) imprime os números de 300 a 350, com um intervalo de 2 segundos entre cada número. Antes de imprimir os números, imprime a frase “Neto 1 foi criado” Após imprimir todos os números, imprime a frase “Neto 1 vai morrer”.*/

/*
int main(int argc, const char * argv[]) {
    
    pid_t pid ;
    int  i ;
    pid = fork();
    
    if(pid != 0){
        //cod do pai
        wait(pid);
        for(i=1; i<=50 ; i++){
            printf("%d\t", i );
            sleep(2);
        }
        
        printf("\nProcesso pai vai morrer");
    }
    else{
        if((pid=fork())!= 0){
            //cod filho1
            wait(pid);
            printf("\nFilho 1 foi criado");
            for(i=100; i<=200; i++){
                printf("%d\t", i);
                sleep(1);
            }
            printf("\nFIlho 1 vai morrer");
        }
        else{
            //cod do neto
            printf("\nNeto 1 foi criado");
            for(i=300; i<=350; i++){
                printf("%d\t", i);
               // sleep(2);
            }
            printf("\nNeto 1 vai morrer");
        }
        
    }
    return 0;
}
*/
