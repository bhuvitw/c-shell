#include <string.h>


void parser(char s[1024], char* args[513]) {
    char *token; 

    token = strtok(s, " ");

    int j = 0;
    args[j] = token; 
    while(token != NULL) {
        j++; 
        token = strtok(NULL, " "); 
        args[j] = token; 
    }

    return;
}