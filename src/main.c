#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>

#include "BUILTIN_H.h"
#include "PARSER_H.h"

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

        char* args[513];
        parser(s, args);
        
        if(builtin(args) == 1) continue; 
        
        pid_t pid = fork(); 

        if(pid < 0){
            // fork failed
            exit(1); 
        } else if (pid == 0){
            // child process
            char* newargs[513]; 

            i = 0;
            while(args[i] != NULL && strcmp(args[i], ">") != 0 && strcmp(args[i], ">>") != 0 && strcmp(args[i], "2>") != 0){
                newargs[i] = args[i]; 
                i++; 
            }
            newargs[i] = NULL; 

            char* appendFilePath = NULL;
            char* truncateFilePath = NULL;
            char* errorFilePath = NULL; 
            int isAppend = 0;
            int isTrunc = 0;
            int isError = 0;

            
            while(args[i] != NULL) {
                if(strcmp(args[i], ">") == 0) {
                    truncateFilePath = args[i+1]; 
                    isTrunc = 1;
                }
                if(strcmp(args[i], ">>") == 0) {
                    appendFilePath = args[i+1]; 
                    isAppend = 1;
                }
                if(strcmp(args[i], "2>") == 0) {
                    errorFilePath = args[i+1]; 
                    isError = 1; 
                }
                i++;
            }

            if(isAppend){
                int fd = open(appendFilePath, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if(fd < 0){
                    perror("open");
                    exit(1);
                }
                dup2(fd, 1); 
                close(fd); 
            }

            if(isTrunc){
                int fd = open(truncateFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd < 0){
                    perror("open");
                    exit(1);
                }
                dup2(fd, 1); 
                close(fd); 
            }

            if(isError){
                int fd = open(errorFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if(fd < 0){
                    perror("open");
                    exit(1);
                }
                dup2(fd, 2); 
                close(fd); 
            }
            

            if(execvp(newargs[0], newargs) == -1){
                perror(newargs[0]);
            }; 
            
            exit(1); 
        } else {
            int status; 
            waitpid(pid, &status, 0);
        }
    }
}