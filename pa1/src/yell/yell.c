#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main(int argc, char *argv[]){
    //checks if input has value
    if(argc < 2 || argv[1][0]!='\0'){
        int length = strlen(argv[1]);
        //makes a variable for the size of the array
       char *ptr = argv[1];
        for(int i = 0; i<length; i++){
            printf("%c", toupper(ptr[i]));
           //for loop to go through the array and uppercase every layer and prints
        }
      
        printf("!!\n");
    }
    else{
        //returns 0 if there is no value
        return 0;
    }
}