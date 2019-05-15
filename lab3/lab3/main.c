#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#define EVER ;;

void handler(int sinal);

int main(int argc, const char * argv[]) {
    pid_t child_a, child_b;
    child_a = fork();

    if (child_a == 0) {
        //CHILD A CODE
        for(EVER)
            printf("A");
    }
    else {
        child_b = fork();
        if(child_b == 0){
            //CHILD B CODE
            for(EVER)
                printf("B");
        }
        else{
            //PARENT CODE
            kill(child_a, SIGSTOP);
            kill(child_b, SIGSTOP);
            
       
            for(int i=0; i<10; i++){
                if(i%2==0){
                    printf("1");
                    kill(child_b, SIGSTOP);
                    sleep(2);
                    kill(child_a, SIGCONT);
                    
                }
                else{
                    printf("2");
                    kill(child_a, SIGSTOP);
                    sleep(2);
                    kill(child_b, SIGCONT);
                }
            }
            kill(child_b, SIGKILL);
            kill(child_a, SIGKILL);
            printf("FOI");
            //exit(0);
        }
    }
    return 0;
}

