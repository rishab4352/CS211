#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node* next;
} newnode;
newnode* head = NULL;
struct node* tail = NULL;

void insert (int newdata, node **head){         
    node* newnode = malloc(sizeof(node));
    
    //Checks if list is empty
    if (head ==NULL){
        newnode->data = newdata;
        newnode->next =NULL
    }

    while (){
        
    }
}
int main()