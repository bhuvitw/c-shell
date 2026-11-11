#include <string.h>

void space_operators(const char *in, char *out, size_t out_size){
    if (out_size == 0) return; 
    size_t o = 0; 
    int in_quotes = 0; 

    for (size_t i = 0; in[i] != '\0'; i++) {
        char c = in[i]; 

        // worst case writes 4 bytes (" >> "), plus 1 for the final '\0'
        if (o + 5 > out_size) break; 

        if (c == '"') {
            in_quotes = !in_quotes; 
            out[o++]= c; 
        }else if (in_quotes) {
            out[o++] = c; 
        } else if ( c == '2' && in[i+1] == '>' && (i == 0 || in[i-1] == ' ')) {
            out[o++] = ' ';
            out[o++] = '2';
            out[o++] = '>';
            out[o++] = ' ';
            i++; 
        } else if ( c == '>' && in[i+1] == '>') {
            out[o++] = ' ';
            out[o++] = '>';
            out[o++] = '>'; 
            out[o++] = ' ';
            i++; 
        } else if (c == '|' || c == '<' || c == '>' || c == '&') {
            out[o++] = ' ';
            out[o++] = c;
            out[o++] = ' ';
        } else {
            out[o++] = c; 
        }
    }
    out[o] = '\0';
}

void parser(char s[1024], char* args[513]) {
    int j = 0; 
    char *p = s;

    while (*p != '\0' && j < 512) {
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