#include<stdio.h>
#include<strings.h>
#include<stdlib.h>


void push(char stack[], char element, int *top){
   stack[*top] = element;    
   *top= *top+1;
}

char pop(char stack[],int *top){
    
    if (*top == 0){
        return 0;
    }
    *top=*top-1;
    char popped =stack[*top];
    return popped;
}
char peek(char stack[],int *top){
    return stack[*top-1];
}
char check_matching(char current, char popped){
    if (current == ')' && popped == '('){
        return 1; 
    }
    else if (current == '}' && popped == '{'){
        return 1;
    }
    else if (current == ']' && popped == '['){
        return 1;   
    }
    else{
        return 0;
    }
}
char getcounter(char car){
    if (car == '('){
        return ')';
    }
    if (car == '{'){
         return '}';
    }
    if (car == '['){
         return ']';
    }
    exit(0);

}
int bracketBalenced(char *arr){
    int i = 0;
    
   
    char stack[100];
    int top=0;
    int *ptr = &top;

    while(arr[i] != '\0'){
        char current = arr[i];
        if (current == '(' || current =='{' || current=='['){
            push(stack, current, ptr);

        }
        else if(current == ')' || current == '}' || current == ']'){
            char poppedbrace = peek(stack,ptr);
            if(!check_matching(current,poppedbrace)){
                printf("%d: %c", i, current);
                return EXIT_FAILURE;
            }
            pop(stack,ptr);
        }
    i++;
    }
    if (top == 0){
        return EXIT_SUCCESS;
    }
    printf("open: ");
    while(top != 0){
        char topbrace = pop(stack,ptr);
        printf("%c", getcounter(topbrace));
    }
    return EXIT_FAILURE;
}
int main(int argc, char **argv){
    char *value = argv[1];
    char check = bracketBalenced(value);
    return check;
}

