#include "linkedList.h"

int deleteNode(struct llist *llist, int nodeNum, bool freed){
    if (llist->size<nodeNum){
        printf("Error: Node Doesn't Exist\n");
        return 0;
    } 
    if(llist->head == NULL){
        printf("Error: List Empty\n");
        return 0;
    } 

    struct node *current = llist->head;
    struct node *removeNode;

    if (nodeNum == 0){
        removeNode = llist->head;
        if (llist->head->next != NULL){
            llist->head = current->next;
        } else {
            llist->head = NULL;
        }
        removeNode->next = NULL;
    } else{
        int i = 0; 
        //Find previous node
        for (i = 0; i<nodeNum-1; i++){
            if (current->next == NULL){
                printf("Error: Node Cannot Be Reached\n");
                return 0;
            }
            current = current->next;
        }  
        removeNode = current->next;
        current->next = removeNode->next;  
        removeNode->next = NULL;
    } 

    if (freed){
        pushBack(freedMBList, removeNode);
    }

    llist->size--;
    return 1;
}

//Add to the back of the list
void pushBack(struct llist *llist, struct node *node){
    if (llist->size == 0){
        llist->head = node;
        llist->size++;
    } else{
        struct node *currentNode = llist->head;
        while(currentNode->next != NULL){
            currentNode = currentNode->next;
        }
        currentNode->next = node;
        node->next = NULL;
        llist->size++;
    }
}

void printList(FILE *file, struct llist *llist, bool alloc){
    //Print out linked list
    int nodeNum = 0;
    struct node *currentNode = llist->head;
    if (llist->head->next == NULL){
        if (alloc){
            fprintf(file, "Node %d: Starting Address=%p, Size=%d, Contents:%s", nodeNum, currentNode->wpt, currentNode->wsize, (char *)currentNode->wpt);
        } else {
            fprintf(file, "Node %d: Starting Address=%p, Size=%d", nodeNum, currentNode->wpt, currentNode->wsize);
        }
        
    } else{
        while (currentNode->next != NULL){
            if (alloc){
                fprintf(file, "Node %d: Starting Address=%p, Size=%d, Contents:%s\n", nodeNum, currentNode->wpt, currentNode->wsize, (char *)currentNode->wpt);
            } else {
                fprintf(file, "Node %d: Starting Address=%p, Size=%d\n", nodeNum, currentNode->wpt, currentNode->wsize);
            }
                
                currentNode = currentNode->next;
                nodeNum++;
        }
    }  
}

//If nodes are next to each other they can be combined into one node
void combineNodes(struct llist* llist){
    struct node* current;
    struct node* compare;
    current = llist->head;
    while(current->next != NULL){
        compare = current->next;
        while(compare->next != NULL){
            if(compare->wpt == current->wpt+current->wsize){
                current->wsize = current->wsize + compare->wsize;
                current->next = compare->next;
                free(compare);
            }
            compare = compare->next;
        }
        current = current->next;
    }
}