#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

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
            execvp(args[0], args); 
            
            exit(1); 
        } else {
            int status; 
            waitpid(pid, &status, 0);
        }
    }
}