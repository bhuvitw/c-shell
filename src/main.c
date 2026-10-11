#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>

#include "BUILTIN_H.h"
#include "PARSER_H.h"

int main(){ 
    signal(SIGINT, SIG_IGN); 

    char s[1024]; 
    while(1){
        int bg_status;
        pid_t bg_pid; 
        while((bg_pid = waitpid(-1, &bg_status, WNOHANG)) > 0){
            printf("[Process %d done]\n", bg_pid);
        }

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

        char *cmd = strtok(s, ";"); 

        while (cmd != NULL) {
            char* args[513]; 

            parser(cmd, args); 

            if(args[0] == NULL) {
                cmd = strtok(NULL, ";"); 
                continue; 
            }

            if(builtin(args) == 1){
                cmd = strtok(NULL, ";");
                continue; 
            } 

            int is_bg = 0; 
            int arg_count = 0;
            while( args[arg_count] != NULL){
                arg_count++ ;
            }

            if(arg_count > 0 && strcmp(args[arg_count - 1], "&") == 0) {
                is_bg = 1; 
                args[arg_count - 1] = NULL; 
            }

            int p = 0;
            while(args[p] != NULL && strcmp(args[p], "|") != 0) {
                p++; 
            }

            if(args[p] != NULL) {

                if(p == 0 || args[p+1]== NULL){
                    fprintf(stderr, "csh: syntax error near |\n");
                    cmd = strtok(NULL, ";");
                    continue; 
                }
                args[p] = NULL;
                char **right = &args[p+1];
                int pipefd[2]; 

                if(pipe(pipefd)<1){
                    perror("pipe");
                    cmd = strtok(NULL, ";");
                    continue; 
                }; 
                pid_t pid1 = fork();
                
                if(pid1 < 0){
                    perror("fork");
                    cmd = strtok(NULL, ";");
                    continue; 
                }
                else if(pid1 == 0){
                    if (!is_bg) signal(SIGINT, SIG_DFL);

                    if (dup2(pipefd[1], 1) < 0) {
                        perror("dup2");
                        exit(1); 
                    }
                    close(pipefd[0]);
                    close(pipefd[1]);
                    if(execvp(args[0], args)<0){
                        perror("execvp");
                    }
                    exit(1); 
                }

                pid_t pid2 = fork(); 
                if(pid2 < 0){
                    perror("fork");
                    cmd = strtok(NULL, ";");
                    continue; 
                }
                else if(pid2 == 0){
                    if (!is_bg) signal(SIGINT, SIG_DFL);

                    if (dup2(pipefd[0],0) < 0) {
                        perror("dup2");
                        exit(1); 
                    }
                    close(pipefd[1]);
                    close(pipefd[0]);
                    if(execvp(right[0], right) < 0){
                        perror("execvp"); 
                    }
                    exit(1);
                } 

                close(pipefd[0]);
                close(pipefd[1]);

                if(!is_bg) {
                    int status1, status2;
                    waitpid(pid1, &status1, 0);
                    waitpid(pid2, &status2, 0);

                    if ((WIFSIGNALED(status1) && WTERMSIG(status1) == SIGINT) || 
                        (WIFSIGNALED(status2) && WTERMSIG(status2) == SIGINT)) {
                        printf("\n");
                    }
                } else {
                    printf("[Background pipe started] %d %d\n", pid1, pid2); 
                }
                cmd = strtok(NULL, ";"); 
                continue;
            }


            pid_t pid = fork(); 

            if(pid < 0){
                perror("fork");
                cmd = strtok(NULL, ";");
                continue; 
            } else if (pid == 0){
                if (!is_bg) signal(SIGINT, SIG_DFL);

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
                    if(dup2(fd, 1) < 0) {
                        perror("dup2");
                        exit(1); 
                    }
                    close(fd); 
                }

                if(isTrunc){
                    int fd = open(truncateFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd < 0){
                        perror("open");
                        exit(1);
                    }
                    if (dup2(fd, 1) < 0) {
                        perror("dup2");
                        exit(1); 
                    } 
                    close(fd); 
                }

                if(isError){
                    int fd = open(errorFilePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if(fd < 0){
                        perror("open");
                        exit(1);
                    }
                    if(dup2(fd, 2) < 0) {
                        perror("dup2");
                        exit(1); 
                    } 
                    close(fd); 
                }
                

                if(execvp(newargs[0], newargs) == -1){
                    perror(newargs[0]);
                }; 
                
                exit(1); 
            } else {
                if(!is_bg){
                    int status; 
                    waitpid(pid, &status, 0);

                    if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT) {
                        printf("\n");
                    }
                } else {
                    printf("[Background process started] %d\n", pid);
                }
                
                cmd = strtok(NULL, ";");
            }
        }
    }
}