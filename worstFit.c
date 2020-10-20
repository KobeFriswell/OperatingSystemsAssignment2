#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "linkedList.c"

#define MAX 1000

void delete500();
void add1000(FILE *file);
void addWorstFit(FILE *file);

int totalMem = 0;
bool valid = true;

int main(int argc, char* argv[]){
    FILE *file;

    if(argc < 2){
        printf("Filename not specified\n");
        return EXIT_FAILURE;
    }

    char const* const filename = argv[1];


    file = fopen(filename, "r");
    if (file == NULL){
        printf("Failed to open file: %s", filename);
        return 1;
    }

    //Create both Linked Lists
    allocMBList = (struct llist*)malloc(sizeof(struct llist));
    freedMBList = (struct llist*)malloc(sizeof(struct llist));
    
    add1000(file);

    while (valid){
        delete500();
        addWorstFit(file);
        combineNodes(freedMBList);
    }
    
    FILE *output;
    output = fopen("output.txt", "w");
    if (output == NULL){
        printf("ERROR!");
        exit(1);
    }

    fprintf(output, "Total Memory Allocated: %d\n", totalMem);
    printList(output, allocMBList, true);
    printList(output, freedMBList, false);

    fclose(file);

    return EXIT_SUCCESS;
}

void delete500(){
    //Loop 500 times to remove random nodes 500 times
    int x;
    int n = 500;
    time_t t;
    srand((unsigned) time(&t));
    for (x = 0; x < n; x++){
        allocMBList->size = allocMBList->size - deleteNode(allocMBList, rand()%allocMBList->size, true);
    }
}

void add1000(FILE *file){
    char str[MAX];
    allocMBList->size = 0;
    char * token;
    int arraySize = 1000;
    allocMBList->head = NULL;
    if (file){
        int i = 0;
        struct node *newNode;
        while (fgets (str, MAX, file) != NULL){
            token = strtok(str, "\n");
            size_t wsize = strlen(token)+1;

            void *request;
            request = sbrk(wsize);
            totalMem = totalMem + wsize;
            strcpy((char *)request,token);
            
            newNode = (struct node*)malloc(sizeof(struct node));
            newNode->wpt = request;
            newNode->wsize = wsize;
            newNode->next = NULL;
            pushBack(allocMBList, newNode);

            //Break after first 1000
            i++;
            if (i==arraySize){
                break;
            }
        }

        if (fgets (str, MAX, file) == NULL){
            valid = false;
        }
    }
}

void addWorstFit(FILE *file){
    char str[MAX];
    char * token;
    int arraySize = 1000;
    if (file){
        int i = 0;
        struct node *newNode;
        while (fgets (str, MAX, file) != NULL){
            token = strtok(str, "\n");
            size_t wsize = strlen(token)+1;
            
            //Get the first node that satisfies the size and reduce size
            struct node *current = freedMBList->head;
            struct node *chosenNode = freedMBList->head;
            int x = 0;
            int index = 0;
            int biggest = chosenNode->wsize;
            for (x = 0; x < freedMBList->size-1; x++){
                if (current->wsize >= wsize && current->wsize > biggest){
                    chosenNode = current;
                    biggest = chosenNode->wsize;
                    index = x;
                } 
                if (current->next != NULL){
                    current = current->next;
                }
            }
            
            // If correct size node exists modify sizes
            if (chosenNode->wsize>wsize){
                newNode = (struct node*)malloc(sizeof(struct node));
                newNode->wpt = chosenNode->wpt;
                strcpy((char *)newNode->wpt,token);
                newNode->wsize = wsize;

                chosenNode->wpt = chosenNode->wpt+wsize;
                chosenNode->wsize = chosenNode->wsize - wsize;

                pushBack(allocMBList, newNode);
            }
            else if (chosenNode->wsize == wsize){
                deleteNode(freedMBList, index, false);
                strcpy((char *)chosenNode->wpt,token);
                pushBack(allocMBList, chosenNode);
            }
            // Else node with right side doesnt exist, create new one
            else{
                struct node *createNode;
                createNode = (struct node*)malloc(sizeof(struct node));

                void *request;
                request = sbrk(wsize);
                totalMem = totalMem + wsize;
                strcpy((char *)request,token);

                createNode->wpt = request;
                createNode->wsize = wsize;
                createNode->next = NULL;
                pushBack(allocMBList, createNode);
            }

            //Break after first 1000
            i++;
            if (i==arraySize){
                break;
            }
        }
        if (fgets (str, MAX, file) == NULL){
            valid = false;
        }
    }
}