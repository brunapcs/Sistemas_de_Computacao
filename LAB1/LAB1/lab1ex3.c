#include<sys/types.h>
 #include<sys/wait.h>
 #include <unistd.h>
 #include <stdio.h>
 
 int main(int argc, const char * argv[]) {
     char *args[] = {"o" ,NULL};
     execv("./oi", args);

 return 0;
 }
 
