/*#include<sys/types.h>
#include<sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, const char * argv[]) {
    
    pid_t pid , wpid;
    int status;
    
    pid = fork();
    
    if(pid != 0){
        //cod do pai
        waitpid(pid,&status,0);
        printf("Processo pai pid:%d\n", getpid());
        printf("PAI FINALIZADO\n");
    }
    else{
        //cod do filho
        printf("Processo filho pid:%d\n" , getpid());
        printf("Filho diz pid do pai: %d \n", getppid());
        printf("filho finalizado\n");
    }
    return 0;
}
*/ 
