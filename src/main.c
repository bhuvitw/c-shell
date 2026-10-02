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
            while(args[i] != NULL && strcmp(args[i], ">") != 0 && strcmp(args[i], ">>") != 0){
                newargs[i] = args[i]; 
                i++; 
            }
            newargs[i] = NULL; 
            

            if(args[i]!=NULL) {
                if(strcmp(args[i], ">") == 0){
                    int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC,  0644);
                    if(fd < 0){
                        perror("open");
                        exit(1); 
                    }
                    dup2(fd, 1); 
                    close(fd); 
                }else if(strcmp(args[i], ">>") == 0){
                    int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND,  0644);
                    if(fd < 0){
                        perror("open");
                        exit(1); 
                    }
                    dup2(fd, 1); 
                    close(fd);
                }else if(strcmp(args[i+1+1], "2>") == 0){
                    
                }
            }
            

            execvp(newargs[0], newargs); 
            
            exit(1); 
        } else {
            int status; 
            waitpid(pid, &status, 0);
        }
    }
}