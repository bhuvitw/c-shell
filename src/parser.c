#include <string.h>

void parser(char s[1024], char* args[513]) {
    int j = 0; 
    char *p = s;

    while (*p != '\0') {
        //1. Skip any leading spaces
        while (*p == ' ') {
            p++; 
        }

        if (*p == '\0'){
            break;
        }

        if(*p == '"'){
            p++; 
            args[j]= p; 
            j++; 

            while (*p != '\0' && *p != '"') {
                p++; 
            }

            if(*p == '"'){
                *p  = '\0';
                p++; 
            }
        }

        else {
            args[j] = p; // Mark the start of the token 
            j++; 

            while (*p != '\0' && *p != ' ') {
                p++; 
            }

            if(*p == ' ') {
                *p = '\0';
                p++; 
            }
        }
    }

    args[j] = NULL; 


}