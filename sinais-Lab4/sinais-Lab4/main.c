#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define EVER ;;

int main() {
    pid_t filho1;
    pid_t filho2;
    pid_t filho3;
    
    filho1 = fork();
    if(filho1 != 0){
        kill(filho1, SIGSTOP);
        filho2= fork();
        kill(filho2, SIGSTOP);
        if( filho2!=0){
            filho3 = fork();
            kill (filho3, SIGSTOP);
            if(filho3 !=0){
                //PAI
                for(EVER){
                kill(filho1, SIGCONT);
                sleep(1);
                kill(filho1, SIGSTOP);
                kill(filho2, SIGCONT);
                sleep(2);
                kill(filho2, SIGSTOP);
                kill(filho3, SIGCONT);
                sleep(2);
                kill(filho3, SIGSTOP);
                }
            }
            else{
                //FILHO 3
                for(EVER)
                printf("3");
            }
        }
        else{
            //FILHO 2
            for(EVER)
             printf("2");
        }
    }
    else{
        //FILHO 1
        for(EVER)
         printf("1");
    }
       
    return 0;
}
