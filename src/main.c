#include<stdio.h>

int main(){ 
    char s[1024]; 
    while(1){
        printf("csh> ");
        char *argument = fgets(s,sizeof s, stdin); 
        printf("You entered: %s\n", argument); 
    }
}