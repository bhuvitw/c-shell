#include <stddef.h>
#include <limits.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int builtin(char* args[513]) {
    if(strcmp(args[0],"cd") == 0) {
        // printf(args[1]); 
        if(args[1] == NULL){
            chdir("/");
        }else{
            int res = chdir(args[1]); 
            if(res == -1){
                printf("cd: %s: No such file or directory\n", args[1]); 
            }
        }
        return 1;  
    }

    else if(strcmp(args[0], "pwd") == 0) {
        char cwd[PATH_MAX];
        if(getcwd(cwd, PATH_MAX) != NULL) {
            printf("%s\n", cwd);
        }else{
            printf("error\n");
        }
        return 1; 
    }

    else if(strcmp(args[0], "exit") == 0){
        exit(0); 
    }

    return 0;
    

}

