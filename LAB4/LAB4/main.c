#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define EVER ;;

/*Faça um programa que crie 2 processos filho e alterne a execução dos filhos. Após 10 trocas de contexto, o processo pai mata os processos filho.
 OBS: Os processos filho são formados por loops eternos.*/

int main(int argc, const char * argv[]) {

    pid_t filho1, filho2;
    int i;
    
    if((filho1 = fork())!=0){
        printf("PID ATUAL");
        
    }
    else{
         for(EVER)
           printf("OI");
         
    }
  
    
    //filho2 = fork();
    
   /*
    if(filho1!=0){
        //cod do pai , pid tem valor do filho
        kill(filho1,SIGSTOP);
        kill(filho2,SIGSTOP);
        
        for(i=0; i<10; i++){
            if(i%2 == 0){
            kill(filho1, SIGCONT);
            sleep(2);
            kill(filho1, SIGSTOP);
            }
            else{
            kill(filho2, SIGCONT);
            sleep(2);
            kill(filho2, SIGSTOP);
            }
        }
        kill(filho1, SIGKILL);
        kill(filho2, SIGKILL);
    }
    else{
        if(filho2!=0){
            //cod do filho1 , pid tem valor do neto
            for(EVER){
                printf("B");
            }
        }
        else{
            //cod do filho2
            for(EVER){
                printf("A");
                sleep(1);
            }
        }
    }
*/
    
    return 0;
}
