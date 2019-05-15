#include <stdio.h>
#include <unistd.h>

int main(void) {
    int fd[2] ;
    int pid_ls, pid_sort ;
    
    /* Cria o pipe. */
    if(pipe(fd)<0) {
        perror("pipe") ;
        return -1;
    }
    
    /* Chama o ls. */
    pid_ls = fork() ;
    if(pid_ls < 0) {
        perror("fork") ;
        return -1;
    }
    if(pid_ls != 0) {
        /* Fecha fd de leitura, pois
         * o ls só escreve. */
        close(fd[0]);
        
        /* Faz stdout apont  ar para o
         * fd de escrita do pipe. Desse modo,
         * o processo ls escreverá no pipe e
         * não na tela. */
        dup2(fd[1], 1) ;
        execlp("/bin/ls", "/bin/ls", NULL) ;
        
    }
    
    /* Processo pai aqui. */
    
    /* Chama o sort. */
    pid_sort = fork() ;
    if(pid_sort < 0) {
        perror("fork") ;
        return -1 ;
    }
    if(pid_sort != 0) {
        /* Fecha o fd de escrita, pois
         * o sort só realiza leitura. */
        close(fd[1]) ;
        
        /* Faz stdin apontar para o fd de
         * leitura do pipe. Desse modo,
         * o processo sort lerá do pipe e
         * não da entrada padrão (teclado). */
        dup2(fd[0], 0) ;
        execlp("/bin/sort", "/bin/sort", NULL) ;
    }
    
    close(fd[0]); close(fd[1]) ;
    
    return 0 ;
}
/* EOF */
