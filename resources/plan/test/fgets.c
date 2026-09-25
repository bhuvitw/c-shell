#include <stdio.h>

int main() {
    FILE *fp; 
    char s[1024]; // big enough for any line this program will encounter
    int linecount = 0; 

    fp = fopen("quotes.txt", "r"); 

    while(fgets(s, sizeof s, fp)!=NULL){
        printf("%d: %s", ++linecount, s); 
    }
    printf("\n");

    fclose(fp); 


}