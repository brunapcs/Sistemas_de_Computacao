#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

int file_select(struct direct   *entry)
{
    if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0))
        return (0);
    else
        return (1);
}
int soma(int count, struct direct **files){
    puts("oi" );
    printf("%s", files[0]->d_name);
    if(count<=0)
        return 0;
    int sum =0;
    char path[MAXPATHLEN];
    struct stat aux;
    char complete_path[256] = "";
    strcat (complete_path, getwd(path));
    strcat (complete_path, "/");
    strcat (complete_path, files[count]->d_name);
    printf("%s", complete_path);
    stat (complete_path , &aux);
    sum += aux.st_size;
    count--;
    
    return sum + soma(count, files);
}
int main(){
    char path[MAXPATHLEN];
    struct direct** files;
    int file_select();
    int count;
    int i;
    
    if (getwd(path) == NULL) {printf("deu ruim"); exit(-1);}
    count = scandir(path, &files, file_select, alphasort);
   
    printf("%d", soma(count, files));

}


