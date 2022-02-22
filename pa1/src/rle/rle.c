#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){
    
    char *a  = (char*)malloc(2*sizeof(char*));
   
    int count = 1;
   
 
    for(int i =1; argv[1][0] !=  '\0'; i++){
       
        if(argv[i]==argv[i-1]){
            count++;
        }
       
        else{
            sprintf(a," %s %d",argv[i],count);
            //printf("%s %d" a,count);
            count = 1;
        }
        
    }
    free (a);
}