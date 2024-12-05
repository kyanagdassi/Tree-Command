#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <inttypes.h>

int fileCount = 0;
int dirCount = 0;
int compareDirents(const void* a, const void* b) {
    const struct dirent *aDir = (const struct dirent *)a;
    const struct dirent *bDir = (const struct dirent *)b;
    return strcmp(aDir->d_name, bDir->d_name);
}

int treeFunc(char *dir_name, int count, int a, int s, int parent, int last){
    struct dirent *entry;
    struct stat fileStat;
    struct dirent *arr = NULL;
    void *tmp = realloc(arr, 200*sizeof(struct dirent));
    if (tmp == NULL) {
            return 1;
            
        } else if (tmp == arr) {
        } else {
            arr = tmp;
        }
    int i = 0; 
    if (chdir(dir_name)==-1){
        free(arr);
        return 1;
    }
    DIR *dir = opendir(".");
    if (dir==NULL){
        free(arr);
        return 1;
    }
    if (dir)
    while ((entry = readdir(dir))!= NULL){
        char *ptr = entry->d_name;
        if ((*ptr=='.' && (strcmp(entry->d_name,".")!=0) && (strcmp(entry->d_name,"..")!=0) && a) || *ptr!='.'){
            arr[i] = *entry;
            i++;
        }
        }
    qsort(arr, i, sizeof(struct dirent), compareDirents);
    int j = 0;
    while (j<i){
        stat(arr[j].d_name, &fileStat);
        if (j!=i-1 || (count!=0 && j==i-1)){
            if (strcmp(arr[j].d_name, ".keep")==0 && j ==i-1 && last){
                printf(" ");
            }
            else{
            printf("|");}}
        last = 0;
        if (j==i-1){
            last = 1;
        }   
        char vertSlash = 0;
        char slash = '|';
        for (int index = 0; index<count; index++){
            if (index>1 && parent){
                printf("    ");
            }
            else{
            printf("   ");}
            if ((strcmp(arr[j].d_name, ".c")==0 || strcmp(arr[j].d_name, ".d")==0) && index==0 && (count==2 || count==3) ){
                printf(" ");
            }
            else{
            if (index==0 && index!=count-1){
                printf("%c", slash);
                }}
                
            }
        parent = 0;
        if (strlen(arr[j].d_name)<3){
            parent = 1;
        }
        if (count==0){
            if (j==i-1){
                vertSlash = '`';
                }
            }
        else{
            if (j==i-1){
                vertSlash = '`';
                }
            else{
                vertSlash = '|';
                }
            }

        if (s){
            if (vertSlash==0){
                printf("-- [%11" PRId64 "]  %s\n", (int64_t)fileStat.st_size, arr[j].d_name);
            }
            else{
            printf("%c-- [%11" PRId64 "]  %s\n", vertSlash, (int64_t)fileStat.st_size, arr[j].d_name);}}
            
        else{
            if (vertSlash==0){
                printf("-- %s\n",arr[j].d_name);
            }
            else{
            printf("%c-- %s\n", vertSlash,arr[j].d_name);}
            }
            
        if (S_ISDIR(fileStat.st_mode)){
            dirCount++;
            treeFunc(arr[j].d_name, count+1, a, s, parent, last); 
                }
        else{
            fileCount++;
            }
        j++; 
    }
        
    closedir(dir);  

    free(arr);
    if (chdir("..")==-1){
        return 1;
    }
    
    return 0; 
    
    }
    
int main(int argc, char *argv[]){
    if (argc==2){ 
        printf("%s\n", argv[1]);
        treeFunc(argv[1],0, 0, 0,0, 0);
        printf("\n%d directories, %d files\n", dirCount, fileCount);
        return 0;
    }
    else if (argc==3){
        if (strcmp(argv[1],"-a")==0){
            printf("%s\n", argv[2]);
            treeFunc(argv[2],0, 1, 0,0, 0);
        }
        else if (strcmp(argv[1],"-s")==0){
            struct stat fileStat;
            stat(argv[2], &fileStat);
            printf("[%11" PRId64 "]  %s\n", (int64_t)fileStat.st_size, argv[2]);
            treeFunc(argv[2],0,0,1,0, 0);
        }
        else{
            printf("Invalid command.\n");
        }
        printf("\n%d directories, %d files\n", dirCount, fileCount);
        }
    
    else if (argc==4){
        struct stat fileStat;
        memset(&fileStat, 0 , sizeof(struct stat));
        char name[20];
        memset(&name, 0 , sizeof(name));
        strcpy(name, argv[3]);
        stat(argv[3], &fileStat);
        printf("[%11" PRId64 "]  %s\n", (int64_t)fileStat.st_size, argv[3]);
        treeFunc(name,0,1,1,0,0);
        printf("\n%d directories, %d files\n", dirCount, fileCount); 
    }
    else{
        printf("Invalid command.\n");
    }
}