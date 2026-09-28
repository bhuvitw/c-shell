#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

int main(){ 
    char s[1024]; 
    while(1){
        printf("csh> ");

        if(fgets(s, sizeof s, stdin) == NULL) exit(0);

        // "\n remover from buffer"
        int i = 0; 
        while(s[i]!='\0'){
            if(s[i] == '\n'){
                s[i] = '\0'; 
                break;
            }  
            i++; 
        }

        if(i == 0) continue; 

        
        char *token; 

        token = strtok(s, " "); 
        char *args[513]; 

        int j = 0;
        args[j] = token; 
        while(token != NULL) {
            j++; 
            token = strtok(NULL, " "); 
            args[j] = token; 
        }
        
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
            continue; 
        }

        if(strcmp(args[0], "pwd") == 0) {
            char cwd[PATH_MAX];
            if(getcwd(cwd, PATH_MAX) != NULL) {
                printf("%s\n", cwd);
            }else{
                printf("error\n");
            }
            continue;
        }

        if(strcmp(args[0], "exit") == 0){
            exit(0); 
        }
        
        pid_t pid = fork(); 

        if(pid < 0){
            // fork failed
            exit(1); 
        } else if (pid == 0){
            // child process
            execvp(args[0], args); 
            
            exit(1); 
        } else {
            int status; 
            waitpid(pid, &status, 0);
        }
    }
}