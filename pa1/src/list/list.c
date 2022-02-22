#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// create a struct to represent the node in linked list

typedef struct node{
       int data;
       struct node *next;
}node;


// function declaration
node* insert( node* head,int element){
    node* newNode = (node*)malloc(sizeof(node));
    
    if(newNode != NULL){
        newNode->data = element;
        newNode->next = NULL;
        if(head ==NULL){
            head = newNode;
        }
        else{
            node *current =head;
            node *prev= NULL;
            while (current != NULL){
                if (current->data >newNode->data){
                    if (prev==NULL){
                        newNode->next = head;
                        head = newNode;
                    }else{
                        newNode->next = current;
                        prev->next= newNode;
                    }
                    return head;
                }
                prev = current;
                current= current->next;
            }
            prev->next= newNode;
        }
    }
    return head;
}

void sort(node* head){
    node *current = head, *ref = NULL;
    int temp;
    if(head == NULL){
        return;
    } else{
        while (current !=NULL){
            ref = current ->next;
            
            while (ref != NULL){
                if(current->data > ref->data ){
                temp = current -> data;
                current ->data = ref ->data;
                ref -> data = temp;
                }
                ref = ref ->next;
            }
            current = current ->next;
        }
    
    }
}
 node* deleteElement(struct node *head,int v){
    if(head != NULL){
        if(head->data ==v){
            struct node *temp=head;
            head = head->next;
            free(temp);
        }
        else{
            struct node *current= head;
            struct node* prev= NULL;
            while(current != NULL){
                if(current->data ==v){
                    prev->next = current->next;
                    free(current);
                    return head;
                }
                prev = current;
                current = current->next;
            }
        }
    }
    return head;
}
 void deleteList(struct node* head){
    struct node *temp ;
    while(head != NULL){
        temp=head;
        temp = head ->next;
        free(temp);
        
    }
   
}
void print(struct node* nodee){
    while(nodee != NULL){
        printf(" %d ", nodee->data);
        nodee = nodee -> next;
    }
}
int length(struct node* head){
    int count = 0;
    struct node *var = head;
       while(var != NULL){
             count++;
             var = var->next;
       }
       return count;
}

int main(void){
    struct node* head = NULL;
    int data;
    char first;
    do{
        fflush(stdout);
        scanf("%c %d", &first, &data);
        if((first) != 'i' || (first) != 'd'){
            break;
        }
        else if ((first) == 'i'){
            head = insert(head, data) ;  
                
        }
        else{
            head = deleteElement(head,data);   
            
        }
        printf("%d: ", length(head));
        print(head);
    }
    while((first) == 'i' || (first) == 'i');
        deleteList(head);
    
    return EXIT_FAILURE;
} 
